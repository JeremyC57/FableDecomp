// CTCBoastingArea::OnHeroEnteringArea(CThing& hero) -- unless a mini game is running, open the hero's boast UI and, when the hero has a boasting crowd control, tell it the hero entered the podium area
// Leaf: ?OnHeroEnteringArea@CTCBoastingArea@@AAEXAAVCThing@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Naming wave 18: the global stand-in g_pGameUI (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007827D0, length 138 bytes (+6 int3 padding). Private; `ret 4` (`this` unused).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (the inlined GetInterface, twice), CDrawQuestInfo::GetMiniGameType
// @ 0x00644D54 on the game UI's quest info (singleton @ 0x013B8790, +0x1C8), CTCBoastUI::RequestActivate @ 0x0056A860, the crowd
// control's 0x00783BF0 (catalog "OnHeroApproachPodiumArea"; named OnHeroEnterPodiumArea after the PDB member list).
// Shape: nested ifs (the crowd test inside the boast-UI block, so &interfaces stays in esi for both lookups); each component
// is used straight from the GetInterface out pointer.
// Layout: retail CThing component bitset +0x20 (0xE8 = CTCBoastUI, bit 8 of the dword at +0x3C; 0x73 = CTCBoastingCrowdControl,
// bit 19 of +0x2C) and interface map +0x44.

class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCBoastUI { public:
    void RequestActivate();                     // 0x0056A860
    void Deactivate();                          // 0x0056A880
};
class CTCBoastingCrowdControl { public:
    void OnHeroEnterPodiumArea();               // 0x00783BF0 (catalog "OnHeroApproachPodiumArea")
    void OnHeroLeavePodiumArea();               // 0x00783C20 (catalog "OnHeroAttackCrowdMember")
};
struct CDrawQuestInfo {
    long GetMiniGameType();                     // 0x00644D54 (EMiniGameType)
};
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    unsigned char _pad00[0x1c8];
    CDrawQuestInfo* m_pQuestInfo;               // +0x1c8
};

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CTCBoastingArea : public CTCBase {
private:
    void OnHeroEnteringArea(CThing& hero);
    void OnHeroLeavingArea(CThing& hero);
};

void CTCBoastingArea::OnHeroEnteringArea(CThing& hero)
{
    if (CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().m_pQuestInfo->GetMiniGameType() == 0) {
        CTCBoastUI* boastUI;
        if (hero.GetTC(TCI_BOAST_UI, &boastUI)) {
            boastUI->RequestActivate();
            CTCBoastingCrowdControl* crowd;
            if (hero.GetTC(TCI_BOASTING_CROWD_CONTROL, &crowd))
                crowd->OnHeroEnterPodiumArea();
        }
    }
}