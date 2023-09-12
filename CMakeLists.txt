cmake_minimum_required(VERSION 3.26)
project(ip-cam-viewer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(IPCAM_QT_PREFIX_PATH "C:/Qt/6.5.2/msvc2019_64" CACHE STRING "Prefix path to Qt 6.5.2 installation")
set(IPCAM_GST_PREFIX_PATH "C:/gstreamer/1.0/msvc_x86_64" CACHE STRING "Prefix path to GStreamer 1.22.5 installation")

set(CMAKE_PREFIX_PATH
    ${IPCAM_QT_PREFIX_PATH}
    ${IPCAM_GST_PREFIX_PATH}
)

find_package(Qt6 COMPONENTS
    Core
    Gui
    Widgets
    REQUIRED
)

# use FindGStreamer.cmake from Qt prefix path
find_package(GStreamer REQUIRED)

add_executable(IpCamViewerApp)
target_link_libraries(IpCamViewerApp PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Widgets
    GStreamer::GStreamer
)

add_subdirectory(Sources)
