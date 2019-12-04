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
const double colorCoeff = 0.8;
const int offsetZStep = 20;
const int expansionCoeffStep = 5;

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
    , offsetX(200)
    , offsetY(200)
    , offsetZ(200)
    , expansionCoeff(50)
{
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

void RenderArea::fillCubeVertices() {
    cubeVertices.clear();
    cubeVertices.reserve(8);
    cubeVertices.emplace_back(expansionCoeff * 0 + offsetX, expansionCoeff * 0 + offsetY, expansionCoeff * -3 + offsetZ);
    cubeVertices.emplace_back(expansionCoeff * 0 + offsetX, expansionCoeff * 0 + offsetY, expansionCoeff * 3 + offsetZ);
    cubeVertices.emplace_back(expansionCoeff * std::sqrt(6) + offsetX, expansionCoeff * -std::sqrt(2) + offsetY, expansionCoeff * -1 + offsetZ);
    cubeVertices.emplace_back(expansionCoeff * 0 + offsetX, expansionCoeff * -2 * std::sqrt(2) + offsetY, expansionCoeff * 1 + offsetZ);
    cubeVertices.emplace_back(expansionCoeff * std::sqrt(6) + offsetX, expansionCoeff * std::sqrt(2) + offsetY, expansionCoeff * 1 + offsetZ);
    cubeVertices.emplace_back(expansionCoeff * -std::sqrt(6) + offsetX, expansionCoeff * -std::sqrt(2) + offsetY, expansionCoeff * -1 + offsetZ);
    cubeVertices.emplace_back(expansionCoeff * 0 + offsetX, expansionCoeff * 2 * std::sqrt(2) + offsetY, expansionCoeff * -1 + offsetZ);
    cubeVertices.emplace_back(expansionCoeff * -std::sqrt(6) + offsetX, expansionCoeff * std::sqrt(2) + offsetY, expansionCoeff * 1 + offsetZ);

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

    fillCubeVertices();

    forEachPoint(cubeVertices[0], cubeVertices[2], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, edgeColor, 0); });
    forEachPoint(cubeVertices[0], cubeVertices[5], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, edgeColor, 0); });
    forEachPoint(cubeVertices[0], cubeVertices[6], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, edgeColor, 0); });
    forEachPoint(cubeVertices[1], cubeVertices[3], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, edgeColor, 0); });
    forEachPoint(cubeVertices[1], cubeVertices[4], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, edgeColor, 0); });
    forEachPoint(cubeVertices[1], cubeVertices[7], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, edgeColor, 0); });
    forEachPoint(cubeVertices[2], cubeVertices[3], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, edgeColor, 0); });
    forEachPoint(cubeVertices[2], cubeVertices[4], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, edgeColor, 0); });
    forEachPoint(cubeVertices[3], cubeVertices[5], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, edgeColor, 0); });
    forEachPoint(cubeVertices[4], cubeVertices[6], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, edgeColor, 0); });
    forEachPoint(cubeVertices[5], cubeVertices[7], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, edgeColor, 0); });
    forEachPoint(cubeVertices[6], cubeVertices[7], [&] (double x, double y, double z, double) { updateCubePoints(x, y); updateDepth(x, y, z, edgeColor, 0); });

    fillPlane(cubeVertices[3], cubeVertices[2], cubeVertices[0], cubeVertices[5]);
    fillPlane(cubeVertices[2], cubeVertices[4], cubeVertices[6], cubeVertices[0]);
    fillPlane(cubeVertices[0], cubeVertices[6], cubeVertices[7], cubeVertices[5]);
    fillPlane(cubeVertices[3], cubeVertices[1], cubeVertices[7], cubeVertices[5]);
    fillPlane(cubeVertices[2], cubeVertices[4], cubeVertices[1], cubeVertices[3]);
    fillPlane(cubeVertices[1], cubeVertices[4], cubeVertices[6], cubeVertices[7]);

    const int r = 100;
    const int centerX = 200;
    const int centerY = 200;
    const int centerZ = 200;
    for (int x = -r; x <= r; x++) {
        const double absY = std::sqrt(r * r - x * x);
        const int maxY = static_cast<int>(absY + 0.5);
        const int minY = -maxY;
        for (int y = minY; y <= maxY; y++) {
            const double z = -std::sqrt(r * r - x * x - y * y) + centerZ;
            updateDepth(x + centerX, y + centerY, z, globeColor, colorCoeff);
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

int RenderArea::fitColorToBounds(int color) {
    if (color < 0) {
        return 0;
    }
    if (color > 255) {
        return 255;
    }
    return color;
}

void RenderArea::updateCubePoints(double x, double y)  {
    const int column = static_cast<int>(x + .5);
    const int row = static_cast<int>(y + .5);
    if (column < 0 || column >= defaultWidth || row < 0 || row >= defaultHeight) {
        return;
    }
    cubePoints[row][column] = true;
}

void RenderArea::updateDepth(double x, double y, double z, const QColor& color, double colorCoeff) {
    const int column = static_cast<int>(x + .5);
    const int row = static_cast<int>(y + .5);
    if (column < 0 || column >= defaultWidth || row < 0 || row >= defaultHeight) {
        return;
    }
    if (depths[row][column] > z) {
        colors[row][column] = QColor(
                    fitColorToBounds(color.red() - z * colorCoeff),
                    fitColorToBounds(color.green() - z * colorCoeff),
                    fitColorToBounds(color.blue() - z * colorCoeff)
                );
        depths[row][column] = z;
    }
}

void RenderArea::mousePressEvent(QMouseEvent *event) {
    if (cubeContains(event->x(), event->y())) {
        dragStarted = true;
        prevPosition.setX(event->x());
        prevPosition.setY(event->y());
        update();
    }
}

void RenderArea::mouseMoveEvent(QMouseEvent *event) {
    if (dragStarted == false) {
        return;
    }
    const int diffX = event->x() - prevPosition.x();
    const int diffY = event->y() - prevPosition.y();
    offsetX += diffX;
    offsetY += diffY;
    prevPosition.setX(event->x());
    prevPosition.setY(event->y());
    update();
}

void RenderArea::mouseReleaseEvent(QMouseEvent *event) {
    dragStarted = false;
    update();
}
bool RenderArea::cubeContains(const int x, const int y) {
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
        for (int x = p1.x; ; x += sign) {
            const double y = (x - p1.x) * (p2.y - p1.y) * 1. / (p2.x - p1.x) + p1.y;
            const double t = (x - p1.x) * 1. / distanceX;
            const double z = p1.z + t * (p2.z - p1.z);
            mapper(x, y, z, t);
            if (x == p2.x) {
                break;
            }
        }
    } else {
        int sign;
        if (p1.y > p2.y) {
            sign = -1;
        } else {
            sign = 1;
        }
        for (int y = p1.y; ; y += sign) {
            const double x = (y - p1.y) * 1. * (p2.x - p1.x) / (p2.y - p1.y) + p1.x;
            const double t = (y - p1.y) * 1. / distanceY;
            const double z = p1.z + t * (p2.z - p1.z);
            mapper(x, y, z, t);
            if (y == p2.y) {
                break;
            }
        }
    }
}

void RenderArea::fillPlane(const Point3D& p1, const Point3D& p2, const Point3D& p3, const Point3D& p4) {
    Point3D points[] = {p1, p2, p3, p4};
    size_t minXIndex = 0, maxXIndex = 0;
    for (size_t i = 1; i < 4; i++) {
        if (points[i].x < points[minXIndex].x) {
            minXIndex = i;
        }
        if (points[i].x > points[maxXIndex].x) {
            maxXIndex = i;
        }
    }
    const Point3D& nextPoint = points[(minXIndex + 1) % 4];
    const Point3D& nextNextPoint = points[(minXIndex + 2) % 4];
    const Point3D& prevPoint = points[(minXIndex - 1 + 4) % 4];
    const Point3D& prevPrevPoint = points[(minXIndex - 2 + 4) % 4];
    Line3D a(points[minXIndex], prevPoint);
    Line3D b(points[minXIndex], nextPoint);
    Line3D a_star(a.p2, prevPrevPoint);
    Line3D b_star(b.p2, nextNextPoint);

    const int minX = points[minXIndex].x;
    const int maxX = points[maxXIndex].x;

    for (int x = minX; x <= maxX; x++) {
        const Point3D minYPoint = std::min(
                    findPointInLine(b, x, defaultHeight),
                    findPointInLine(b_star, x, defaultHeight),
                    [&] (const Point3D point1, const Point3D& point2) { return point1.y < point2.y; }
        );
        const Point3D maxYPoint = std::max(
                    findPointInLine(a, x, 0),
                    findPointInLine(a_star, x, 0),
                    [&] (const Point3D point1, const Point3D& point2) { return point1.y < point2.y; }
        );
        for (int y = minYPoint.y; y <= maxYPoint.y; y++) {
            double t = (y - minYPoint.y) * 1. / (maxYPoint.y - minYPoint.y);
            double z = minYPoint.z + t * (maxYPoint.z - minYPoint.z);
            updateCubePoints(x, y);
            updateDepth(x, y, z, cubeColor, colorCoeff);
        }
    }
}

Point3D RenderArea::findPointInLine(const Line3D& line, const int x, const int defaultValue) {
    if (x < std::min(line.x1, line.x2) || x > std::max(line.x1, line.x2)) {
        return Point3D(x, defaultValue, std::numeric_limits<double>::max());
    } else if (line.dx == 0) {
        double t = (x - line.x1) * 1. / line.dx;
    } else {
        double t = (x - line.x1) * 1. / line.dx;
        double z;
        if (std::fabs(line.dz) < 0.000001) {
            z = line.z1;
        } else {
            z = t * line.dz + line.z1;
        }
        return Point3D(x, static_cast<int>(t * line.dy + line.y1 + 0.5), z);
    }
}

void RenderArea::sizeIncClicked() {
    expansionCoeff += expansionCoeffStep;
    update();
}

void RenderArea::sizeDecClicked() {
    expansionCoeff -= expansionCoeffStep;
    update();
}

void RenderArea::closenessIncClicked() {
    offsetZ -= offsetZStep;
    update();
}

void RenderArea::closenessDecClicked() {
    offsetZ += offsetZStep;
    update();
}
