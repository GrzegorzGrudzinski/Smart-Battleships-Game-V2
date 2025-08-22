cmake_minimum_required(VERSION 3.5)

project(statki)

add_executable(statki main.cpp plansza/plansza.cpp gui/gui.cpp pliki/obsluga_plikow.cpp rozgrywka/rozgrywka.cpp bot/bot.cpp)

install(TARGETS statki RUNTIME DESTINATION bin)
