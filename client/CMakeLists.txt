set(MODULE_NAME client)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 Qt5
    COMPONENTS
    Widgets
    Network
    Multimedia
    REQUIRED
)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
    Widgets
    Network
    Multimedia
    REQUIRED
)

set(PROJECT_SOURCES
        main.cpp
        client.cpp
        client.h
        login.cpp
        login.h
        talkie.cpp
        talkie.h
        wav.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${MODULE_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ${MODULE_NAME} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${MODULE_NAME} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${MODULE_NAME}
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(${MODULE_NAME}
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Multimedia
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${MODULE_NAME})
endif()

if(WIN32)
    set_property(TARGET ${MODULE_NAME} PROPERTY WIN32_EXECUTABLE true)
    windeployqt(${MODULE_NAME})
endif()
