cmake_minimum_required(VERSION 3.5)

project(statki)

include_directories(Inc)

set(SOURCE_FILES
    Src/bot.cpp
    Src/gui.cpp
    Src/obsluga_plikow.cpp
    Src/plansza.cpp
    Src/rozgrywka.cpp
    main.cpp
)

add_executable(statki ${SOURCE_FILES})

install(TARGETS statki RUNTIME DESTINATION bin)
