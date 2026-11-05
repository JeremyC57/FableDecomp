// CGameCameraManager::IsObservedThingInMelee() -- true when the observed thing is alive, has its combat component (interface type 0x36) and that component's melee target index (+0x34) is set (>= 0)
// Leaf: ?IsObservedThingInMelee@CGameCameraManager@@QAE_NXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound) re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006994F0, length 108 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times, on PObservedThing), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x004365B0 (the inlined GetInterface for type 0x36 = bit 22 of the bitset word at +0x24).
// Layout: retail CGameCameraManager with PDB names: PObservedThing +0x100 (PDB +0x110). The thing: dead flag +0x91, interface bitset
// +0x20, interface map +0x44.
// Shape: the IS_THING_ALIVE idiom, then the out-parameter GetInterface template (out pointer left uninitialised: it shares the
// lookup key's frame word) as the condition; the success arm returns `mov al,1`, every failure joins the `xor al,al` exit.
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCCombat { public:                          // interface type 0x36
    unsigned char _pad[0x34];
    long          m_meleeTarget;            // +0x34
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CGameCameraManager { public:
    void*                   __vftable;      // +0x00
    unsigned char           _pad04[0xfc];   // +0x04
    CIntelligentPointer<CThing> PObservedThing; // +0x100
    bool IsObservedThingInMelee();
};

bool CGameCameraManager::IsObservedThingInMelee()
{
    if (PObservedThing.GetPItem() && !PObservedThing.GetPItem()->IsDead()) {
        const CTCCombat* combat;
        if (PObservedThing.GetPItem()->PeekTC(TCI_STEALTH, &combat)) {
            if (combat->m_meleeTarget >= 0)
                return true;
        }
    }
    return false;
}