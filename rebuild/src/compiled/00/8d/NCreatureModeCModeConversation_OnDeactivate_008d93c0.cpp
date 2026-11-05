// NCreatureMode::CModeConversation::OnDeactivate() -- give the talk component back the attitude the mode started with, then mark the mode inactive
// Leaf: ?OnDeactivate@CModeConversation
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D93C0, 69 bytes (+ int3 padding), `ret`; vtable 0x01285990 slot 6.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (non-const instantiation; inlined CTCTalk lookup, interface 15 = bit 15 of
//   the +0x20 dword).
// Name: FableWin.pdb `?OnDeactivate@CModeConversation@NCreatureMode@@UAEXXZ` (slot 6).
// Layout: Ego_r.pdb CModeConversation (0x34, matches retail): InitialAttitude +0x20; CModeBase Active +0x18; the talk
//   component's attitude word +0x18 (written directly: the inline CTCTalk::SetAttitude).
// Shape: out-parameter GetInterface (component read back as `mov eax,[eax+4]`), then the inline CModeBase::OnDeactivate
//   (`Active = false`) as the last statement.
class CCharString;
class CTCBase;

typedef long EConversationAttitude;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCTalk { public:                             // interface 15
    unsigned char _pad00[0x18];
    EConversationAttitude Attitude;          // +0x18
    void SetAttitude(EConversationAttitude attitude) { Attitude = attitude; }
};

class CAnimFilterOfGroup { public:                  // vtable 0x01267568
    virtual void v00();
    void* Group;                             // +4 CCharString
    unsigned long GroupCRC;                  // +8
};

struct CModeBase {                           // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing* ParentThing;                     // +4
    unsigned char _body[0x10];               // WeaponBasedFilter, FramesBeforeRemovingSelf
    bool Active;                             // +0x18
    void OnDeactivate() { Active = false; }
};

namespace NCreatureMode {
struct CModeConversation : CModeBase {       // vtable 0x01285990
    EConversationAttitude CurrentAttitude;   // +0x1c
    EConversationAttitude InitialAttitude;   // +0x20
    CAnimFilterOfGroup Filter;               // +0x24
    bool ResetIdleAnim;                      // +0x30
    void OnDeactivate();
};
}

void NCreatureMode::CModeConversation::OnDeactivate()
{
    CTCTalk* talk;
    if (ParentThing->GetTC(TCI_TALK, &talk))
        talk->SetAttitude(InitialAttitude);
    CModeBase::OnDeactivate();
}