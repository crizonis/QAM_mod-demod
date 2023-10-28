cmake_minimum_required(VERSION 3.28)
set(BUILD_PATH ./build/stat)
file(MAKE_DIRECTORY ${BUILD_PATH})

project(QAM_mod_demod)



set(SOURCE main.cc)
set(MOD_LIB qam_classes/qam_modulator.cc)
set(DEMOD_LIB qam_classes/qam_demodulator.cc)
set(NOISE_LIB qam_classes/generator_gauss_noise.cc)
set(EXT_LIB other_libs/ext_modules.cc)

add_library(mod STATIC ${MOD_LIB})
add_library(demod STATIC ${DEMOD_LIB})
add_library(noise STATIC ${NOISE_LIB})
add_library(ext STATIC ${EXT_LIB})

add_executable(main ${SOURCE})	

target_link_libraries(main mod demod noise ext)



