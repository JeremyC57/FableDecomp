// CTCCreatureModeManager::CModeBase::FrameUpdate() -- counts FramesBeforeRemovingSelf down and, when it reaches zero, asks the owner's mode manager to remove this mode
// Leaf: ?FrameUpdate@CModeBase@CTCCreatureModeManager@@UAEXXZ
// NF167 (2026-11-20, q11669): leaf respelt QAE -> UAE in place on the row's own nested CModeBase model: Ego_r.pdb / FableWin.pdb
//   ?FrameUpdate@CModeBase@CTCCreatureModeManager@@UAEXXZ (public virtual, CModeBase vtable 0x012673CC slot 3; engine/CTCCreatureModeManager.h
//   FABLE_RETAIL_CMODEBASE_VIRTUALS); 9 landed referencers (0x008D6C80 0x008D6D40 0x008D8EA0 0x008DF9C0 0x008E0A10 0x008E16A0 0x008E20F0
//   0x008E24F0 0x008E8830) already bind the UAE spelling.
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073AC50, 112 bytes (padding included).
// Shape: a positive counter (+0x14) is decremented and re-read; at zero the owner's (ParentThing,
// +4) creature-mode manager (interface 0x31: present bit 17 of the +0x24 word, LowerBound
// 0x0040F020 with the key in the `push ecx` frame word) is fetched with the
// `T* p = 0; if (HasTC(k)) p = GetTC(k);` shape (the preloaded `xor esi,esi`), then
// RemoveMode(GetModeType()) (0x0073ABA0; GetModeType is vtable slot 1).
class CTCBase;

#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCCreatureModeManager { public:                 // interface type 0x31 component

    class CModeBase { public:
        virtual void v00();
        virtual long GetModeType();             // slot 1
        CThing* ParentThing;                    // +4
        unsigned char WeaponBasedFilter[0xc];   // +8 CAnimFilterOfGroup
        long FramesBeforeRemovingSelf;          // +0x14
        bool Active;                            // +0x18
        virtual void FrameUpdate();             // slot 3 (declared after slot 1 here; the row emits no vtable)
    };
public:
    void RemoveMode(unsigned long);   // 0x0073ABA0
};

void CTCCreatureModeManager::CModeBase::FrameUpdate()
{
    if (FramesBeforeRemovingSelf > 0)
        --FramesBeforeRemovingSelf;
    if (FramesBeforeRemovingSelf == 0) {
        CTCCreatureModeManager* modes = 0;
        if (ParentThing->HasTC(TCI_ENTITY_MODE_MANAGER))
            modes = (CTCCreatureModeManager*)((CTCCreatureModeManager*)&ParentThing->GetTC(TCI_ENTITY_MODE_MANAGER));
        modes->RemoveMode(GetModeType());
    }
}