cmake_minimum_required (VERSION 3.8)

add_executable(ActivityMonitor WIN32 main.cpp "ActivityMonitorApp.h" "ActivityMonitorApp.cpp" "MainFrame.h" "MainFrame.cpp")


target_link_libraries(ActivityMonitor ${wxWidgets_LIB_DIR}/wxmsw31ud.lib)
target_link_libraries(ActivityMonitor ${wxWidgets_LIB_DIR}/wxexpatd.lib)
target_link_libraries(ActivityMonitor ${wxWidgets_LIB_DIR}/wxjpegd.lib)
target_link_libraries(ActivityMonitor ${wxWidgets_LIB_DIR}/wxpngd.lib)
target_link_libraries(ActivityMonitor ${wxWidgets_LIB_DIR}/wxregexud.lib)
target_link_libraries(ActivityMonitor ${wxWidgets_LIB_DIR}/wxscintillad.lib)
target_link_libraries(ActivityMonitor ${wxWidgets_LIB_DIR}/wxtiffd.lib)
target_link_libraries(ActivityMonitor ${wxWidgets_LIB_DIR}/wxzlibd.lib)