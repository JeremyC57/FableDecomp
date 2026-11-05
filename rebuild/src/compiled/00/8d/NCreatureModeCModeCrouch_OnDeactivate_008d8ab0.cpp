// NCreatureMode::CModeCrouch::OnDeactivate() -- clear both byte flags of the interface-0x36 component, then mark the mode inactive
// Leaf: ?OnDeactivate@CModeCrouch
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D8AB0, 79 bytes, `ret`; vtable 0x01267B34 slot 6.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined out-parameter GetInterface, type 0x36 = bit 22 of the +0x24 dword).
// Name: FableWin.pdb `?OnDeactivate@CModeCrouch@NCreatureMode@@UAEXXZ` (slot 6).
// Layout: CModeBase Active +0x18; the 0x36 component's flags +0x0f (raised by CModeCrouch::OnActivate 0x008D89C0) and +0x10.
// Shape: the inline CModeBase::OnDeactivate (`Active = false`) after the lookup block; VC7.1 shares one zero register for
//   the three stores and duplicates the short tail into both paths.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CTCInterface36 {                        // interface type 0x36 (two byte flags, cleared again by OnDeactivate)
    unsigned char _pad00[0x0f];
    bool          m_crouching;                 // +0x0f
    bool          m_flag10;                    // +0x10
};

struct CModeBase {                             // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing*       ParentThing;                 // +0x04
    unsigned char _body[0x10];                 // WeaponBasedFilter, FramesBeforeRemovingSelf
    bool          Active;                      // +0x18
    void OnDeactivate() { Active = false; }
};

namespace NCreatureMode {
struct CModeCrouch : CModeBase {               // vtable 0x01267B34
    void*                SneakAnimName;        // +0x1c CCharString
    unsigned char        Filter[0x14];         // +0x20 CAnimFilterOf2Groups
    void OnDeactivate();
};
}

void NCreatureMode::CModeCrouch::OnDeactivate()
{
    CTCInterface36* tc;
    if (ParentThing->GetTC(TCI_STEALTH, &tc)) {
        tc->m_crouching = false;
        tc->m_flag10 = false;
    }
    CModeBase::OnDeactivate();
}