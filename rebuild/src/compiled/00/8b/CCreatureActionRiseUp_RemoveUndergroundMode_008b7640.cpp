// CCreatureAction_RiseUp::RemoveUndergroundMode() -- take the creature out of mode 30 through its mode-manager component (interface type 0x31) and refresh the mini-map display
// Leaf: ?RemoveUndergroundMode@CCreatureAction_RiseUp@@QAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B7640, length 96 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x31 = bit 17 of the
// bitset dword at +0x24), CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0 (landed naming), the game UI singleton at 0x013B8790: GetMiniMapDisplay(0) @ 0x00437CE0
// then CMiniMapDisplay::Refresh @ 0x0082B7C0 (the manifest's labels of those two are propagated). Shape: the mode manager is read from the frame slot even when absent (as 0x00849250).
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCCreatureModeManager { public:
    void RemoveMode(unsigned long mode);    // 0x0073ABA0
};
class CMiniMapDisplay { public:
    void Refresh();                         // 0x0082B7C0
};
struct CGameUI {
    CMiniMapDisplay* GetMiniMapDisplay(long which);   // 0x00437CE0
};
extern CGameUI* g_pGameUI_013b8790;         // 0x013B8790

class CCreatureAction_RiseUp { public:
    virtual void v00();
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;      // +0x08
    void RemoveUndergroundMode();
};

void CCreatureAction_RiseUp::RemoveUndergroundMode()
{
    CTCCreatureModeManager* pModes;
    PCreature.Get()->GetTC(TCI_ENTITY_MODE_MANAGER, &pModes);
    pModes->RemoveMode(30);
    g_pGameUI_013b8790->GetMiniMapDisplay(0)->Refresh();
}