#include <soundManager.hpp>
#include <cstdio>
#include <cstring>
#include <AL/al.h>
#include <AL/alc.h>

int testBledu(char const* komunikat)
{
	int kodBledu = alGetError();		
	if (kodBledu != AL_NO_ERROR) 
	{	
		char nazwaBledu[64];
		podajTypBledu(kodBledu,nazwaBledu);
		fprintf(stderr, "%s; %s(%i)\n",komunikat,nazwaBledu,kodBledu);		
	}
	return kodBledu;	
}

void podajTypBledu(int kodBledu,char* nazwaBledu)
{
	switch(kodBledu)
	{
	case AL_INVALID_NAME:
		sprintf(nazwaBledu,"AL_INVALID_NAME");
		return;
	case AL_INVALID_ENUM:
		sprintf(nazwaBledu,"AL_INVALID_ENUM");
		return;
	case AL_INVALID_VALUE:
		sprintf(nazwaBledu,"AL_INVALID_VALUE");
		return;
	case AL_INVALID_OPERATION:
		sprintf(nazwaBledu,"AL_INVALID_OPERATION");
		return;
	case AL_OUT_OF_MEMORY:
		sprintf(nazwaBledu,"AL_OUT_OF_MEMORY");
		return;
	default:
		sprintf(nazwaBledu,"UNKNOWN_ERROR");
		return;
	}
}

void podajListeUrzadzenAudio(const ALCchar *urzadzenia)
{
	const ALCchar *urzadzenie = urzadzenia, *nastepne = urzadzenia + 1;
	size_t dlugosc = 0;

	fprintf(stdout, "Lista urzadzen:\n");
	fprintf(stdout, "----------\n");
	while (urzadzenie && *urzadzenie != '\0' && nastepne && *nastepne != '\0') 
	{
		fprintf(stdout, "%s\n", urzadzenie);
		dlugosc=strlen(urzadzenie);
		urzadzenie+=(dlugosc+1);
		nastepne+=(dlugosc+2);
	}
	fprintf(stdout, "----------\n");
}

ALenum doFormatuOpenAL(short kanaly, short rozdzielczosc)	//Do wykorzystania przy wczytywaniu dźwięku z plików
{
	bool stereo = (kanaly > 1);

	switch (rozdzielczosc) 
	{
		case 16:
			if (stereo)
				return AL_FORMAT_STEREO16;
			else
				return AL_FORMAT_MONO16;
		case 8:
			if (stereo)
				return AL_FORMAT_STEREO8;
			else
				return AL_FORMAT_MONO8;
		default:
			return -1;
	}
}
