/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "renderarea.h"

#include <QPainter>
#include <QMouseEvent>
#include <list>
#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>

const int defaultWidth = 1200;
const int defaultHeight = 600;

RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
    , pointsPen(Qt::black, 1)
    , convexHullPen(Qt::red, 1)
    , polygonPen(Qt::black, 1)
    , xDistribution(defaultWidth / 4, 3 * defaultWidth / 4)
    , yDistribution(defaultHeight / 4, 3 * defaultHeight / 4)
{
    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::white);
    setPalette(pal);
    setAutoFillBackground(true);
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize RenderArea::sizeHint() const
{
    return QSize(defaultWidth, defaultHeight);
}

void RenderArea::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.setBrush(Qt::NoBrush);

    painter.setPen(pointsPen);
    painter.drawPoints(&points[0], points.size());
    if (points.size() > 2) {
        painter.setPen(convexHullPen);
        painter.drawPolygon(&convexHull[0], convexHull.size());
    }
}

void RenderArea::generatePoints(size_t num) {

    points.clear();
    points.reserve(num);
    for (size_t i = 0; i < num; i++) {
        points.push_back(QPoint(xDistribution(generator), yDistribution(generator)));
    }
    buildConvexHull();
    update();
}

void RenderArea::clearPoints() {
    points.clear();
    update();
}

size_t RenderArea::getPointsNum() {
    return points.size();
}

void RenderArea::mousePressEvent(QMouseEvent *event) {
    QRect updateRect;

    QPoint curPoint(event->x(), event->y());
    points.push_back(curPoint);
    buildConvexHull();
    std::cout << "built convex hull: " << convexHull.size() << std::endl;
    buildUpdateRect(updateRect);

    if (updateRect.isValid()) {
        update(updateRect);
    }
    return;
}

void RenderArea::buildUpdateRect(QRect &rect) {
    int minX = std::numeric_limits<int>::max();
    int minY = std::numeric_limits<int>::max();
    int maxX = std::numeric_limits<int>::min();
    int maxY = std::numeric_limits<int>::min();
    for (const QPoint& clickedPoint : points) {
        minX = std::min(minX, clickedPoint.x());
        minY = std::min(minY, clickedPoint.y());
        maxX = std::max(maxX, clickedPoint.x());
        maxY = std::max(maxY, clickedPoint.y());
    }
    rect.setCoords(minX, minY, maxX, maxY);
    rect.adjust(-1, -1, 1, 1);
}

void RenderArea::buildConvexHull() {
    if (points.size() < 3) {
        return;
    }
    convexHull.clear();

    QPoint minXPoint = *std::min_element(points.begin(), points.end(), RenderArea::compareX);
    std::sort(points.begin(), points.end(), [&] (const QPoint& p1, const QPoint& p2) {
        const double tan1 = RenderArea::tan(p1, minXPoint);
        const double tan2 = RenderArea::tan(p2, minXPoint);
        if (std::abs(tan1 - tan2) < 0.0000001) {
            return RenderArea::dist(p1, minXPoint) < RenderArea::dist(p2, minXPoint);
        } else {
            return tan1 < tan2;
        }
    });
    std::queue<QPoint> pointQueue;
    for (const QPoint& point : points) {
        pointQueue.push(point);
    }

    convexHull.push_back(minXPoint);
    QPoint v = pointQueue.front();
    pointQueue.pop();
    QPoint w = pointQueue.front();
    pointQueue.pop();
    while (true) {
        if (isRightTurn(*convexHull.rbegin(), v, w)) {
            v = *convexHull.rbegin();
            convexHull.pop_back();
        } else {
            convexHull.push_back(v);
            v = w;
            if (pointQueue.empty()) {
                break;
            }
            w = pointQueue.front();
            pointQueue.pop();
        }
    }
    convexHull.push_back(v);
}

bool RenderArea::isRightTurn(const QPoint &p1, const QPoint &p2, const QPoint& p3) {
    return ((p2.x() - p1.x()) * (p3.y() - p1.y()) - (p3.x() - p1.x()) * (p2.y() - p1.y())) <= 0;
}

bool RenderArea::compareX(const QPoint &p1, const QPoint &p2) {
    return p1.x() < p2.x() || (p1.x() == p2.x() && p1.y() < p2.y());
}

double RenderArea::tan(const QPoint &p1, const QPoint &p2) {
    if (p1.x() == p2.x()) {
        return std::numeric_limits<double>::max();
    } else {
        return (p1.y() - p2.y()) * 1. / (p1.x() - p2.x());
    }
}

double RenderArea::dist(const QPoint &p1, const QPoint &p2) {
   return (p1.x() - p2.x()) * (p1.x() - p2.x()) + (p1.y() - p2.y()) * (p1.y() - p2.y());
}
