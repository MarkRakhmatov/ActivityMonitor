cmake_minimum_required (VERSION 3.8)

project ("ActivityMonitor")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# dependencies
set(THIRD_PARTY_DIR "${PROJECT_SOURCE_DIR}/../../ThirdPartyLibs")

# wxWidgets setup
set(WX_WIDGETS_DIR "${THIRD_PARTY_DIR}/wxWidgets")
set(wxWidgets_ROOT_DIR "${WX_WIDGETS_DIR}/wxWidgets-3.1.4")
set(wxWidgets_LIB_DIR "${WX_WIDGETS_DIR}/wxWidgetsBuild/static/x64/lib/vc_x64_lib")
set(wxWidgets_INCLUDE_DIRS "${wxWidgets_ROOT_DIR}/include")
set(wxWidgets_WIN_INCLUDE_DIRS "${wxWidgets_LIB_DIR}/mswu")

# Include sub-projects.
add_subdirectory ("src/ActivityMonitor")
add_subdirectory ("src/ActivityMonitorLib")
add_subdirectory ("src/GeneralUtils")
add_subdirectory ("src/WindowsUtils")


target_include_directories(ActivityMonitor PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_include_directories(ActivityMonitor PUBLIC "${wxWidgets_INCLUDE_DIRS}")
target_include_directories(ActivityMonitor PUBLIC "${wxWidgets_WIN_INCLUDE_DIRS}")
target_include_directories(ActivityMonitor PUBLIC "${JSON_SRC_DIR}")