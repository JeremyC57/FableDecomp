#ifndef FABLE_ENGINE_CTCBETTING_H
#define FABLE_ENGINE_CTCBETTING_H
#ifdef FABLE_RETAIL_CTCBETTING
#include "engine/CTCBase.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
class CTCHeroStats;class CEnginePrimitiveHandle;
// Original Ego_r339f4 public CTCBase base; native007e57c0 with 12-byte vectors.
class CTCBetting:public CTCBase {
public:
 CWideString GetPlayersMoney();
 CWideString GetText(unsigned long);
 void GetHeroStats(CTCHeroStats**);
private:
 bool Active;
 std::vector<CEnginePrimitiveHandle> BetHandles,MoneyHandles;
 long FrameBetWasLastChanged,FrameBetChangeStarted,Mult,FrameToStart,PreviousDonation;
};
FABLE_STATIC_ASSERT(sizeof(CTCBetting)==0x3c);
#else
class CTCBetting;
#endif
#endif
