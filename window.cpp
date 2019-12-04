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
#include "window.h"

#include <QtWidgets>

Window::Window()
{
    renderArea = new RenderArea;
    sizeLabel = new QLabel("Размер кубика: ");
    closenessLabel = new QLabel("Близость кубика: ");
    sizeIncBtn = new QPushButton("+");
    sizeDecBtn = new QPushButton("-");
    closenessIncBtn = new QPushButton("+");
    closenessDecBtn = new QPushButton("-");

    sizeLayout = new QGridLayout;
    sizeLayout->addWidget(sizeIncBtn, 0, 0);
    sizeLayout->addWidget(sizeDecBtn, 1, 0);

    closenessLayout = new QGridLayout;
    closenessLayout->addWidget(closenessIncBtn, 0, 0);
    closenessLayout->addWidget(closenessDecBtn, 1, 0);

    mainLayout = new QGridLayout;
    mainLayout->addWidget(renderArea, 0, 0, 1, 4);
    mainLayout->addWidget(sizeLabel, 1, 0);
    mainLayout->addLayout(sizeLayout, 1, 1);
    mainLayout->addWidget(closenessLabel, 1, 2);
    mainLayout->addLayout(closenessLayout, 1, 3);

    connect(sizeIncBtn, &QPushButton::clicked, this, &Window::sizeIncClicked);
    connect(sizeDecBtn, &QPushButton::clicked, this, &Window::sizeDecClicked);
    connect(closenessIncBtn, &QPushButton::clicked, this, &Window::closenessIncClicked);
    connect(closenessDecBtn, &QPushButton::clicked, this, &Window::closenessDecClicked);

    setLayout(mainLayout);

    setWindowTitle(tr("Lab 5"));
}

void Window::sizeIncClicked() {
    renderArea->sizeIncClicked();
}

void Window::sizeDecClicked() {
    renderArea->sizeDecClicked();
}

void Window::closenessIncClicked() {
    renderArea->closenessIncClicked();
}

void Window::closenessDecClicked() {
    renderArea->closenessDecClicked();
}

Window::~Window() {
    delete sizeLabel;
    delete closenessLabel;
    delete sizeIncBtn;
    delete sizeDecBtn;
    delete closenessIncBtn;
    delete closenessDecBtn;
    delete sizeLayout;
    delete closenessLayout;
    delete renderArea;
    delete mainLayout;
}
