cmake_minimum_required(VERSION 2.8)
#set (OpenCV_INCLUDE_DIRS '/usr/local/include/')
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set (SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/")
#link_directories('/usr/local/lib/')
set (OpenCV_LIBS opencv_core opencv_highgui opencv_imgproc)
project( DisplayImage )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( DisplayImage "${SRC}DisplayImage.cpp" )
add_executable( ScanImage "${SRC}ScanImage.cpp"  )
add_executable( MaskOperation "${SRC}MaskOperation.cpp"  )
add_executable( Blend "${SRC}Blend.cpp"  )
add_executable( BrightnessContrast "${SRC}BrightnessContrast.cpp"  )
add_executable( warpTriangle "${SRC}warpTriangle.cpp"  )
add_executable( msm "${SRC}MSM.cpp"  )
add_executable( barcode-QRcodeScanner "${SRC}barcode-QRcodeScanner.cpp"  )
add_executable( msmOrig "${SRC}StateMachineEx.cpp"  )

target_include_directories(
    msm  PUBLIC
    boost_1_69_0/
    )
target_include_directories(
        msmOrig PUBLIC
        boost_1_69_0/
        )


target_link_libraries( DisplayImage ${OpenCV_LIBS} )
target_link_libraries( ScanImage ${OpenCV_LIBS} )
target_link_libraries( MaskOperation ${OpenCV_LIBS} )
target_link_libraries( Blend ${OpenCV_LIBS} )
target_link_libraries( BrightnessContrast ${OpenCV_LIBS} )
target_link_libraries( warpTriangle ${OpenCV_LIBS} )
target_link_libraries( barcode-QRcodeScanner ${OpenCV_LIBS} zbar)
target_link_libraries( msmOrig ${OpenCV_LIBS} )





