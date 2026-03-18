project(aptf)
cmake_minimum_required(VERSION 2.8)

#寻找qt4：
find_package(Qt4 REQUIRED)

set(QT_USE_QTNETWORK TRUE) #使用QT4网络模块。
set(QT_USE_QTXML TRUE) #使用QT4 XML模块。




set(CqlHeaders                    ) #需要moc处理的头文件。



qt4_wrap_ui(CqlUic ${CqlUi})
qt4_wrap_cpp(CqlHeadersMoc ${CqlHeaders})

set(DatabaseAccessDirectory ../QmDatabaseAccess) #DatabaseAccess库的包含目录。


include (${QT_USE_FILE}) #包含QT头文件。

add_definitions (${QT_DEFINITIONS})


set(LANG AptF.ts) #翻译文件。
qt4_wrap_translation(TRANSLATIONS ${LANG}) #加入翻译任务。
#QT4_ADD_TRANSLATIONS(TRANSLATIONS ${LANG}) #加入翻译。


aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST} ${CqlHeadersMoc} ${CqlUic} ${TRANSLATIONS}) #最终的可执行程序。


target_link_libraries (${PROJECT_NAME} ${QT_LIBRARIES} wthttp wt boost_signals-mt wtdbo wtdbopostgres wtdbosqlite3 QmDatabaseAccess   qjson) #链接的目标库。
