/*
 * Autor: Aleksander Szpakiewicz-Szatan 
 */

#ifdef _WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <stdint.h>
#include <cinttypes>
#include <chrono>
#include <thread>
#define _USE_MATH_DEFINES
#include <cmath>

#include <cstdbool>

#include <AL/al.h>
#include <AL/alc.h>

#include <soundManager.hpp>

using namespace std;
void getErrorType(int error,char* errortype);

int main(int argc, char** argv) 
{ 
	ALboolean enumeracja;
	const ALCchar *nazwaDomyslnegoUrzadzenia = argv[1];
	ALCdevice *urzadzenie;
	ALCcontext *kontekst;
	double czestotliwosc=1200.0;

	ALuint bufor, zrodlo;
	ALfloat orientacjaSluchacza[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };
	ALfloat polozenieZrodla[] = { 5.0f, 0.0f, 5.0f};
	ALfloat predkoscZrodla[]= {-0.8f, 0.0f, -1.0f};
	ALboolean petla = AL_TRUE;
	ALfloat dt=1e3;	//Gęstość próbkowania aktualizacji położenia ruchomej próbki w mikrosekundach, domyślnie 1000us
	ALint stanZrodla;
	ALfloat modulacja=1.0;


	enumeracja = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
	if (enumeracja == AL_FALSE)
		fprintf(stderr, "Enumeracja urządzeń nie jest obsługiwana.\n");

	podajListeUrzadzenAudio(alcGetString(NULL, ALC_DEVICE_SPECIFIER));	//Podaj listę urządzeń audio, na Linuxie najpewniej będzie to wyłącznie OpenALSoft

	//Dodaj kod pozwalający wybrać urządzenie z listy

	if (!nazwaDomyslnegoUrzadzenia)	//wybierz urządzenie domyślne jako urządzenie
		nazwaDomyslnegoUrzadzenia = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

	urzadzenie = alcOpenDevice(nazwaDomyslnegoUrzadzenia);	//Otwórz urządzenie, na Linuxie najpewniej będzie to OpenALSoft
	if (!urzadzenie) 
	{
		fprintf(stderr, "Nie można otworzyć domyślnego urządzenia.\n");
		return -1;
	}

	fprintf(stdout, "Device: %s\n", alcGetString(urzadzenie, ALC_DEVICE_SPECIFIER));

	alGetError();

	kontekst = alcCreateContext(urzadzenie, NULL);
	if (!alcMakeContextCurrent(kontekst)) 
	{
		fprintf(stderr, "failed to make default kontekst\n");
		return -1;
	}
	if(testBledu("make default kontekst")!=AL_NO_ERROR)return -1;

	/* Ustaw słuchacza w pozycji (0,0,0) */
	alListener3f(AL_POSITION, 0,0,0);
    alListener3f(AL_VELOCITY, 0,0,0);
	alListenerfv(AL_ORIENTATION, orientacjaSluchacza);

	alGenSources((ALuint)1, &zrodlo);
	if(testBledu("Ustawienie źródła")!=AL_NO_ERROR)return -1;

	alSourcef(zrodlo, AL_PITCH, modulacja);
	alSourcef(zrodlo, AL_GAIN, 1);
	alSourcefv(zrodlo, AL_POSITION, polozenieZrodla);
	alSourcefv(zrodlo, AL_VELOCITY, predkoscZrodla);
	alSourcei(zrodlo, AL_LOOPING, petla);

	alGenBuffers(1, &bufor);
	if(testBledu("przygotowanie bufora")!=AL_NO_ERROR)return -1;

	uint32_t liczbaProbek=4410; 
	int16_t* dane=(int16_t*)calloc(sizeof(int16_t),liczbaProbek);
	double omega=0.0;

	//uint32_t x=0x935148CF;	//pozostałość kodu służacego do generowania pseudolosowego szumu białego
	for(uint32_t ii=0; ii<=liczbaProbek; ii++)
    {
		/*x^=x<<13;				//pozostałość kodu służacego do generowania pseudolosowego szumu białego
		x^=x>>17;
		x^=x<<5;
		data[ii] = x>>16;*/
		
		omega=2.0*M_PI*((double)ii)/liczbaProbek*czestotliwosc/10/8;	//Oblicz wartość argumentu by uzyskać falę o zadanej częstotliwości
		if(omega>M_PI/4.0)	//Przerób falę sinusoidalną na piłokształtną (choć z zaokrąglonymi zębami)
		{
			omega-=(M_PI/4.0)*floor(4.0*omega/M_PI);
		}
		dane[ii]=32768.0*sin(omega);
    }
	
	alBufferData(bufor, AL_FORMAT_MONO16, dane, 2*liczbaProbek, 44100);
	free(dane);
	if(testBledu("wczytanie danych do bufora")!=AL_NO_ERROR)return -1;

	alSourcei(zrodlo, AL_BUFFER, bufor);
	if(testBledu("przypisanie bufora do źródła")!=AL_NO_ERROR)return -1;

	alSourcePlay(zrodlo);
	if(testBledu("odtworzenie dźwięku")!=AL_NO_ERROR)return -1;

	alGetSourcei(zrodlo, AL_SOURCE_STATE, &stanZrodla);
	if(testBledu("sprawdzenie czy źródło działa")!=AL_NO_ERROR)return -1;
	modulacja=-0.339836909;	//Początkowa wartość modulacji
	while (stanZrodla == AL_PLAYING) 
	{
		alGetSourcei(zrodlo, AL_SOURCE_STATE, &stanZrodla);
		
		if(testBledu("sprawdzenie czy źródło działa")!=AL_NO_ERROR)return -1;
		
		std::this_thread::sleep_for(std::chrono::microseconds((uint32_t)dt)); //wykonuj operacje w pętli ze stałą częstotliwością
		
		polozenieZrodla[0]+=predkoscZrodla[0]*(dt*1e-6);	
		polozenieZrodla[1]+=predkoscZrodla[1]*(dt*1e-6);
		polozenieZrodla[2]+=predkoscZrodla[2]*(dt*1e-6);	
		
		alSourcef(zrodlo, AL_PITCH, 1.33+0.5*sin(modulacja));	//moduluj częstotliwość sinusoidalnie (od 0,83 do 1,83), zakomentowanie tej linii sprawi, że próbka będzie cały czas odtwarzana ze stałą częstotliwością
		modulacja+=dt*4e-6;										//Jak szybko ma się modulować fala nośna
		
		alSourcefv(zrodlo, AL_POSITION, polozenieZrodla);
		if(testBledu("ustawienie położenia źródła")!=AL_NO_ERROR)return -1;
		
		//podaj w konsoli koordynaty źródła oraz mnożnik częstotliwości (słuchacz jest w (0,0,0), podstawowa częstotliwość 1200Hz// 
		fprintf(stdout,"x= %lf, y= %lf, z= %lf, modulacja=%lf\n",polozenieZrodla[0], polozenieZrodla[1], polozenieZrodla[2], 1.33+0.5*sin(modulacja));
		
		if(polozenieZrodla[0]<-5.0)		//Gdy źródło się oddali
		{
			alSourceStop(zrodlo);
		}
	}

	//Posprzątaj
	alDeleteSources(1, &zrodlo);
	alDeleteBuffers(1, &bufor);
	urzadzenie = alcGetContextsDevice(kontekst);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(kontekst);
	alcCloseDevice(urzadzenie);

	fprintf(stdout,"OK\n");

	return 0; 
}


