#ifndef _SOUNDMANAGER_HPP_
#define _SOUNDMANAGER_HPP_
#include <sound.hpp>
#include <AL/al.h>
#include <AL/alc.h>

int testBledu(char const* komunikat);
void podajTypBledu(int kodBledu,char* nazwaBledu);
void podajListeUrzadzenAudio(const ALCchar *urzadzenia);
ALenum doFormatuOpenAL(short kanaly, short rozdzielczosc);

#endif
