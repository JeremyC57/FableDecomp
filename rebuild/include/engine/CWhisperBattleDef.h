#ifndef FABLE_ENGINE_CWHISPERBATTLEDEF_H
#define FABLE_ENGINE_CWHISPERBATTLEDEF_H
#include "engine/CParentDefClassBase.h"
class CWhisperBattleDef : public CParentDefClassBase {
public:
    static CCharString FABLE_FASTCALL StatGetTypeName() { return CCharString("CWhisperBattleDef"); }
    long NumGetHitsBeforeSommersault;
    long NumPhase2Potions;
};
FABLE_STATIC_ASSERT(offsetof(CWhisperBattleDef,NumPhase2Potions)==0x40);
#endif
