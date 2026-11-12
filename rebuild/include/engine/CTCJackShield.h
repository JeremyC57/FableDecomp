#ifndef FABLE_ENGINE_CTCJACKSHIELD_H
#define FABLE_ENGINE_CTCJACKSHIELD_H
// Original types: Ego_r and FableWin TPI; named donor calls and native field accesses.
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
class CTCJackShield:public CTCBase {
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return (ETCInterfaceType)36;}
 void EnableShield(bool);
 CIntelligentPointer<CThing> PEmitter;
};
FABLE_STATIC_ASSERT(sizeof(CTCJackShield)==20);
#endif
