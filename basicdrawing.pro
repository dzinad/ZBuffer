QT += widgets
requires(qtConfig(combobox))

HEADERS       = renderarea.h \
                point3d.h \
                window.h
SOURCES       = main.cpp \
                point3d.cpp \
                renderarea.cpp \
                window.cpp
RESOURCES     = basicdrawing.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/painting/basicdrawing
INSTALLS += target
