#ifndef FABLE_ENGINE_CTCHASTE_H
#define FABLE_ENGINE_CTCHASTE_H
#include "engine/CTCBase.h"
// TPI0x2f115 original fields; native00667ff5 writes PauseFrames+14.
#ifdef FABLE_RETAIL_CTCHASTE
// Retail opt-in (chunk 755, row 0x006A02B0 CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::FinishAction): Ego_r.pdb
// CTCHaste::Multipliers is std::map<void*,float> (+0x18, the STLport three-word tree); SetActionSpeedMultiplier(void*, float)
// (Ego_r 0x0051BF50 / FableWin ?SetActionSpeedMultiplier@CTCHaste@@QAEXPAXM@Z) is expanded in that row: map::insert (the
// out-of-line _Rb_tree::insert_unique 0x00580D21) of one value_type temporary, then the private
// ?UpdateActionSpeedMultiplier@CTCHaste@@AAEXXZ 0x00800020. Plain inline: an out-of-line copy exists in Ego_r.
#include "stlport/fable_map.h"
class CTCHaste : public CTCBase {
private:
    float Multiplier;bool ToTerminate;long PauseFrames;
    std::map<void*,float> Multipliers;                                   // +0x18
    void UpdateActionSpeedMultiplier();                                  // 0x00800020
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_HASTE; }
    void SetOnHitPause(long frames) { PauseFrames=frames; }
    void SetActionSpeedMultiplier(void* key, float multiplier)
    {
        Multipliers.insert(std::map<void*,float>::value_type(key, multiplier));
        UpdateActionSpeedMultiplier();
    }
};
#else
class CTCHaste : public CTCBase {
private:
    float Multiplier;bool ToTerminate;long PauseFrames;
    unsigned char Multipliers[12]; // std::map<void*,float>, retail STLport.
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_HASTE; }
    void SetOnHitPause(long frames) { PauseFrames=frames; }
};
#endif
FABLE_STATIC_ASSERT(sizeof(CTCHaste)==0x24);

#endif
