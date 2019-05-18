CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)

project(TSR-Analysis)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

FIND_PACKAGE(Qt5Core)
FIND_PACKAGE(Qt5Gui)
FIND_PACKAGE(Qt5Widgets)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_BINARY_DIR}
	include
)

SET( MOCS 
	include/mainWindow.h
	include/methodInfoWindow.h
	include/progressWindow.h
	include/settings.h
	include/settingsWindow.h
	include/compareInfoWindow.h
	include/difficultSignsWindow.h
	include/confusionMatrix.h
	include/pictureWidget.h
)

#QT5_WRAP_CPP( MOC_SRCS ${MOCS} )
qt5_wrap_ui( UI_HDRS 
	gui/mainWindow.ui 
	gui/methodInfoWindow.ui
	gui/progressWindow.ui
	gui/settingsWindow.ui
	gui/compareInfoWindow.ui
	gui/difficultSignsWindow.ui
	gui/confusionMatrix.ui
	gui/pictureWidget.ui
)

SET( RCRS
	resources/signs.qrc
)

QT5_ADD_RESOURCES( RCC_SRCS ${RCRS})


SET( INCS 
	${MOCS}
    	include/TSRDefs.h
    	include/CMProxyModel.h
    	include/CMTableModel.h
)

SET( SRCS 
	src/main.cpp
	src/mainWindow.cpp
	src/methodInfoWindow.cpp
	src/progressWindow.cpp
	src/settings.cpp
	src/settingsWindow.cpp
	src/compareInfoWindow.cpp
	src/difficultSignsWindow.cpp
	src/confusionMatrix.cpp
	src/pictureWidget.cpp
    src/CMProxyModel.cpp
    src/CMTableModel.cpp
) 

ADD_EXECUTABLE( ${PROJECT_NAME}
	${MOC_SRCS}	
	${SRCS} 
	${INCS}
	${UI_HDRS}
    ${RCC_SRCS}
)

QT5_USE_MODULES( ${PROJECT_NAME}
	Core
	Gui
	Widgets
)

TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${QT_LIBRARIES} )