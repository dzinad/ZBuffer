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
#include <cmath>

const int defaultWidth = 1200;
const int defaultHeight = 600;

RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
    , cubePoints(defaultHeight, std::vector<bool>(defaultWidth))
    , depths(defaultHeight, std::vector<double>(defaultWidth))
    , colors(defaultHeight, std::vector<QColor>(defaultWidth))
    , edgeColor(Qt::black)
    , cubeColor(Qt::blue)
    , globeColor(Qt::red)
    , backgroundColor(Qt::white)
    , dragStarted(false)
{
    const int offsetX = 200;
    const int offsetY = 200;
    const int offsetZ = 200;
    const int expansionCoefficient = 50;
    cubeVertices.reserve(8);
    cubeVertices.emplace_back(expansionCoefficient * 0 + offsetX, expansionCoefficient * 0 + offsetY, expansionCoefficient * -3 + offsetZ);
    cubeVertices.emplace_back(expansionCoefficient * 0 + offsetX, expansionCoefficient * 0 + offsetY, expansionCoefficient * 3 + offsetZ);
    cubeVertices.emplace_back(expansionCoefficient * std::sqrt(6) + offsetX, expansionCoefficient * -std::sqrt(2) + offsetY, expansionCoefficient * 1 + offsetZ);
    cubeVertices.emplace_back(expansionCoefficient * 0 + offsetX, expansionCoefficient * -2 * std::sqrt(2) + offsetY, expansionCoefficient * -1 + offsetZ);
    cubeVertices.emplace_back(expansionCoefficient * std::sqrt(6) + offsetX, expansionCoefficient * std::sqrt(2) + offsetY, expansionCoefficient * -1 + offsetZ);
    cubeVertices.emplace_back(expansionCoefficient * -std::sqrt(6) + offsetX, expansionCoefficient * -std::sqrt(2) + offsetY, expansionCoefficient * 1 + offsetZ);
    cubeVertices.emplace_back(expansionCoefficient * 0 + offsetX, expansionCoefficient * 2 * std::sqrt(2) + offsetY, expansionCoefficient * 1 + offsetZ);
    cubeVertices.emplace_back(expansionCoefficient * -std::sqrt(6) + offsetX, expansionCoefficient * std::sqrt(2) + offsetY, expansionCoefficient * -1 + offsetZ);
    QPalette pal(palette());
    pal.setColor(QPalette::Background, backgroundColor);
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
    for (auto&& rowOfDepths : depths) {
        rowOfDepths.assign(defaultWidth, std::numeric_limits<double>::max());
    }
    for (auto&& rowOfColors : colors) {
        rowOfColors.assign(defaultWidth, backgroundColor);
    }
    for (auto&& rowOfCubePoints : cubePoints) {
        rowOfCubePoints.assign(defaultWidth, false);
    }

    QPainter painter(this);

    forEachPoint(cubeVertices[0], cubeVertices[2], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, edgeColor); });
    forEachPoint(cubeVertices[0], cubeVertices[5], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, edgeColor); });
    forEachPoint(cubeVertices[0], cubeVertices[6], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, edgeColor); });
    forEachPoint(cubeVertices[1], cubeVertices[3], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, edgeColor); });
    forEachPoint(cubeVertices[1], cubeVertices[4], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, edgeColor); });
    forEachPoint(cubeVertices[1], cubeVertices[7], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, edgeColor); });
    forEachPoint(cubeVertices[2], cubeVertices[3], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, edgeColor); });
    forEachPoint(cubeVertices[2], cubeVertices[4], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, edgeColor); });
    forEachPoint(cubeVertices[3], cubeVertices[5], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, edgeColor); });
    forEachPoint(cubeVertices[4], cubeVertices[6], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, edgeColor); });
    forEachPoint(cubeVertices[5], cubeVertices[7], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, edgeColor); });
    forEachPoint(cubeVertices[6], cubeVertices[7], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, edgeColor); });

    forEachPoint(cubeVertices[0], cubeVertices[2], cubeVertices[6], cubeVertices[4], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, cubeColor); });
    forEachPoint(cubeVertices[0], cubeVertices[2], cubeVertices[5], cubeVertices[3], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, cubeColor); });
    forEachPoint(cubeVertices[0], cubeVertices[5], cubeVertices[6], cubeVertices[7], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, cubeColor); });
    forEachPoint(cubeVertices[1], cubeVertices[3], cubeVertices[4], cubeVertices[2], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, cubeColor); });
    forEachPoint(cubeVertices[1], cubeVertices[3], cubeVertices[7], cubeVertices[5], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, cubeColor); });
    forEachPoint(cubeVertices[1], cubeVertices[4], cubeVertices[7], cubeVertices[6], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, cubeColor); });

    const int r = 80;
    const int centerX = 200;
    const int centerY = 200;
    const int centerZ = 200;
    for (int x = -r; x <= r; x++) {
        const double absY = std::sqrt(r * r - x * x);
        const int maxY = static_cast<int>(absY + 0.5);
        const int minY = -maxY;
        for (int y = minY; y <= maxY; y++) {
            const double z = -std::sqrt(r * r - x * x - y * y);
            updateDepth(x + centerX, y + centerY, z + centerZ, globeColor);
        }
    }

    QImage image(defaultWidth, defaultHeight, QImage::Format_RGB888);
    for (size_t row = 0; row < defaultHeight; row++) {
        for (size_t column = 0; column < defaultWidth; column++) {
            image.setPixelColor(column, row, colors[row][column]);
        }
    }
    painter.drawImage(0, 0, image);
}

void RenderArea::updateCubePoints(double x, double y)  {
    const int column = static_cast<int>(x + .5);
    const int row = static_cast<int>(y + .5);
    if (column < 0 || column >= defaultWidth || row < 0 || row >= defaultHeight) {
        return;
    }
    cubePoints[row][column] = true;
}

void RenderArea::updateDepth(double x, double y, double z, const QColor& color) {
    const int column = static_cast<int>(x + .5);
    const int row = static_cast<int>(y + .5);
    if (column < 0 || column >= defaultWidth || row < 0 || row >= defaultHeight) {
        return;
    }
//    if (column == 200) {
//        std::cout << "row = " << row << ", z = " << z << ", color = " << color.value() << std::endl;
//    }
    if (depths[row][column] > z) {
        colors[row][column] = color;
        depths[row][column] = z;
    }
}

void RenderArea::mousePressEvent(QMouseEvent *event) {
    std::cout << "mouse pressed" << std::endl;
    if (cubeContains(event->x(), event->y())) {
        std::cout << "cube contains" << std::endl;
        dragStarted = true;
        prevPosition.setX(event->x());
        prevPosition.setY(event->y());
        update();
    }
    std::cout << "cube does not contain" << std::endl;
}

void RenderArea::mouseMoveEvent(QMouseEvent *event) {
    if (dragStarted == false) {
        return;
    }
    const int diffX = event->x() - prevPosition.x();
    const int diffY = event->y() - prevPosition.y();
    for (int i = 0; i < 8; i++) {
        cubeVertices[i].x += diffX;
        cubeVertices[i].y += diffY;
    }
    prevPosition.setX(event->x());
    prevPosition.setY(event->y());
    update();
}

void RenderArea::mouseReleaseEvent(QMouseEvent *event) {
    std::cout << "mouse released" << std::endl;
    dragStarted = false;
    update();
}
bool RenderArea::cubeContains(const int x, const int y) {
    std::cout << "cube containts start: " << x << ", " << y << std::endl;
    if (x < 0 || x >= defaultWidth || y < 0 || y >= defaultHeight) {
        return false;
    }
    return cubePoints[y][x];
}

template<class T>
void RenderArea::forEachPoint(const Point3D& p1, const Point3D& p2, T mapper) {
    const int distanceX = p2.x - p1.x;
    const int distanceY = p2.y - p1.y;
    const int diffX = std::abs(distanceX);
    const int diffY = std::abs(distanceY);
    if (diffX > diffY) {
        int sign;
        if (p1.x > p2.x) {
            sign = -1;
        } else {
            sign = 1;
        }
        for (int x = p1.x; x != p2.x; x += sign) {
            const double y = (x - p1.x) * (p2.y - p1.y) * 1. / (p2.x - p1.x) + p1.y;
            const double t = (x - p1.x) * 1. / distanceX;
            const double z = p1.z + t * (p2.z - p1.z);
            mapper(x, y, z, t);
        }
    } else {
        int sign;
        if (p1.y > p2.y) {
            sign = -1;
        } else {
            sign = 1;
        }
        for (int y = p1.y; y != p2.y; y += sign) {
            const double x = (y - p1.y) * 1. * (p2.x - p1.x) / (p2.y - p1.y) + p1.x;
            const double t = (y - p1.y) * 1. / distanceY;
            const double z = p1.z + t * (p2.z - p1.z);
            mapper(x, y, z, t);
        }
    }
}

template<class T>
void RenderArea::forEachPoint(const Point3D& p1, const Point3D& p2, const Point3D& p3, const Point3D& p4, T mapper) {
    forEachPoint(p1, p2, [&] (double firstSegmentX, double firstSegmentY, double firstSegmentZ, double t) {
        const double secondSegmentX = p3.x + t * (p4.x - p3.x);
        const double secondSegmentY = p3.y + t * (p4.y - p3.y);
        const double secondSegmentZ = p3.z + t * (p4.z - p3.z);
        forEachPoint(Point3D(static_cast<int>(firstSegmentX + .5), static_cast<int>(firstSegmentY + .5), firstSegmentZ),
                     Point3D(static_cast<int>(secondSegmentX + .5), static_cast<int>(secondSegmentY + .5), secondSegmentZ),
                     mapper);
        // TODO
    });
}
