// CTCBoastingArea::OnHeroLeavingArea(CThing& hero) -- close the hero's boast UI when it has one, and tell the hero's boasting crowd control (when any) that the hero left the podium area
// Leaf: ?OnHeroLeavingArea@CTCBoastingArea@@AAEXAAVCThing@@@Z
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00782860, length 121 bytes (+7 int3 padding). Private; `ret 4` (`this` unused).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (the inlined GetInterface, twice), CTCBoastUI::Deactivate @ 0x0056A880 (landed),
// the crowd control's 0x00783C20 (catalog "OnHeroAttackCrowdMember"; named OnHeroLeavePodiumArea after the PDB member list).
// Shape: two independent ifs (each lookup forms &interfaces in esi again); each component used straight from the out pointer.
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
struct CGameUI {
    unsigned char _pad00[0x1c8];
    CDrawQuestInfo* m_pQuestInfo;               // +0x1c8
};
extern CGameUI* g_pGameUI;                      // 0x013B8790

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

void CTCBoastingArea::OnHeroLeavingArea(CThing& hero)
{
    CTCBoastUI* boastUI;
    if (hero.GetTC(TCI_BOAST_UI, &boastUI))
        boastUI->Deactivate();
    CTCBoastingCrowdControl* crowd;
    if (hero.GetTC(TCI_BOASTING_CROWD_CONTROL, &crowd))
        crowd->OnHeroLeavePodiumArea();
}