#ifndef FABLE_ENGINE_CANIMINIT_H
#define FABLE_ENGINE_CANIMINIT_H
#include "engine/EAnimUpdateMode.h"
class CAnimFrameInfo{public:float Current,Last;};
class CAnimInit{public:EAnimUpdateMode UpdateMode;unsigned char Type;bool Looping;long Anim;float Speed,Alpha,LastAlpha;CAnimFrameInfo Frame;unsigned long Flags,TypeFlags;long Priority;};
#endif
