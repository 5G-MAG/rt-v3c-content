if(NOT ANDROID)

GET_FILENAME_COMPONENT(MYNAME ${CMAKE_CURRENT_LIST_DIR} NAME)

project(${MYNAME} C CXX)

set(CORE_SRC
	src/vpccchunkcreator.cpp
	src/sequence.cpp
	src/groupofframes.cpp
    include/groupofframes.h
    include/sequence.h
    include/vpccinputparser.h
)

include_directories( ${CMAKE_SOURCE_DIR}/vpcclibcommon/include
                     ${TMC2_DIR}/source/lib/PccLibBitstreamCommon/include
                     ${TMC2_DIR}/source/lib/PccLibBitstreamWriter/include
                     ${TMC2_DIR}/source/lib/PccLibBitstreamReader/include
                     ${TMC2_DIR}/source/lib/PccLibCommon/include
                     ${TMC2_DIR}/source/lib/PccLibDecoder/include
                     ${TMC2_DIR}/dependencies/PccLibHevcParser/include
)

link_directories( ${TMC2_DIR}/lib/${BUILD_MODE} )

set( LIBS PccLibCommon
          PccLibBitstreamCommon
          PccLibBitstreamWriter
          PccLibBitstreamReader
          PccLibHevcParser
          TLibCommon
          TLibDecoder
          PccLibDecoder
          PccLibVideoDecoder
          PccLibColorConverter
)

# Executable
add_executable(${MYNAME} ${CORE_SRC})
target_link_libraries(${MYNAME} PRIVATE ${LIBS})
target_include_directories(${MYNAME} PRIVATE include)

# Install
install(TARGETS ${MYNAME} RUNTIME DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/../../bin/${CMAKE_SYSTEM_NAME}")

endif()
