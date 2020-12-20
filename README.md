Różnorodne testy możliwości OpenAL.

Obecnie odtwarza dźwięk (piłokształtna fala 1200Hz modulowana od 83% częstotliwości do 183% częstotliwości) przesuwający się w trójwymiarowej płaszczyźnie po czym się wyłącza.

Do kompilacji wymaga nagłówków biblioteki openAL (w Debianie 10 libopenal-dev). 

Kompilacja: make

Kompilacja wersji na system Windows: 
make main.exe
wymaga umieszczenia plików OpenAL32.dll i OpenAL32.lib w katalogu ./lib, a plik main.exe może wymagać biblioteki libwinpthread-1.dll

Przed kompilacją może być wymagane utworzenie katalogów:
./obj/lin
./bin/lin
oraz (dla kompilacji wersji dla systemu Windows):
./obj/win
./obj/lin