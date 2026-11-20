// CAIStateGroup_WanderAroundShops::IsRunnable() -- runnable in work hours when the idle scheduler (if any) prefers this group, a random shop can be picked and is alive, and a pretty thing (the shop itself when it is a shop component holder, else one found near it) is alive
// Leaf: ?IsRunnable@CAIStateGroup_WanderAroundShops@@UAE_NXZ
// NF131 (2026-11-20, q4946): the row-local CAIStateGroupBase model gives way to the retail engine/CAIStateGroupBase.h (FABLE_RETAIL_CAISTATEGROUPBASE,
//   _MEMBERS) and engine/CAIStateGroup_WorkBase.h; the class is re-based on CAIStateGroup_WorkBase<CAIStateGroupBase> (RTTI ghidra_out/rtti_bases.tsv),
//   so the call to 0x0091A6F0 binds ?IsInWorkHours@?$CAIStateGroup_WorkBase@VCAIStateGroupBase@@@@IBE_NXZ (Ego_r.pdb 0x0093E1A0 + FableWin.pdb
//   0x02BA9910: protected const member of the template), was a stand-in spelling.
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00947280, length 346 bytes (plain `ret`, virtual, bool).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 211 = bit 19 of the bitset dword at +0x38),
//          CTCIdleScheduler::IsPreferredIdle(CAIStateGroupBase*) @ 0x00801BE0, CAIStateGroup_WorkBase::IsInWorkHours
//          @ 0x0091A6F0, the shop picker @ 0x00946F90 (manifest "CAIStateGroup_WanderAroundShops_SelectRandomEnemy") and the
//          pretty-thing finder @ 0x00947110 (manifest "..._FindRandomNPC"), CBaseIntelligentPointer ctor 0x00A01B10 /
//          GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (vtable 0x01238C6C = CIntelligentPointer<CThing>).
// Shape: the scheduler lookup is the out-parameter GetInterface whose hit path alone calls IsPreferredIdle; both pointer
//          assignments from a raw CThing* go through the implicit CIntelligentPointer conversion temporary (ctor, SetPItem,
//          inline operator= naming the item: `CThing* item = other.GetPItem(); SetPItem(item);`, dtor); the same-type `PrettyThing = Shop` is the bare inline
//          operator=. IS_THING_ALIVE is the two-GetPItem inline test; the shop's type-144 test (bit 16 of +0x30) is a third
//          GetPItem. Separate `mov al,1` / `xor al,al` exits (nested success path).
// Layout: engine/CAIStateGroup_WanderAroundShops.h (Shop +0x14, PrettyThing +0x1c, NextFrameBuy +0x24) over the landed
//          CAIStateGroupBase model (the header flattens the vptr, so the base is modelled locally).
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#include "engine/CIntelligentPointer.h"


inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}

class CAIStateGroupBase;
class CTCIdleScheduler { public:                       // interface 211
    bool IsPreferredIdle(CAIStateGroupBase* group);   // 0x00801BE0
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIStateGroup_WorkBase.h"
struct CAIStateGroup_WanderAroundShops : CAIStateGroup_WorkBase<CAIStateGroupBase> {
private:
    CThing* FindASimplyDelightfulShop();   // 0x00946F90 (Ego_r.pdb ?FindASimplyDelightfulShop@CAIStateGroup_WanderAroundShops@@AAEPAVCThing@@XZ)
public:
    CIntelligentPointer<CThing> Shop;              // +0x14
    CIntelligentPointer<CThing> PrettyThing;       // +0x1c
    long NextFrameBuy;                          // +0x24
    CThing* SelectPrettyThing(CThing* shop);    // 0x00947110
    virtual bool IsRunnable();
};

bool CAIStateGroup_WanderAroundShops::IsRunnable()
{
    CTCIdleScheduler* scheduler;
    if (PBrain->PCreature->GetTC(TCI_IDLE_SCHEDULER, &scheduler)) {
        if (!scheduler->IsPreferredIdle(this))
            return false;
    }
    if (IsInWorkHours()) {
        Shop = ((CAIStateGroup_WanderAroundShops*)this)->FindASimplyDelightfulShop();
        if (IS_THING_ALIVE(Shop)) {
            if (Shop.Get()->HasTC(TCI_SHOP_KEEPER))
                PrettyThing = Shop;
            else
                PrettyThing = SelectPrettyThing(Shop.Get());
            if (IS_THING_ALIVE(PrettyThing))
                return true;
        }
    }
    return false;
}