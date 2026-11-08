// CTCDivineWrath::SetPTarget() -- clear the spell's target, then take the thing the caster is targeting (interface 8) when it is alive, of a targetable type and the caster is of the right type; Z-targeting follows the caster's player
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078E820, 220 bytes (plain `ret`).
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10
// (CIntelligentPointer vtable 0x0123E7D8), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0, the targeting
// component's virtual slot 33 (+0x84, the targeted thing; as CTCCarriedActionUseTrophy 0x007EDEE0), CIntelligentPointer<const
// CThing>::CIntelligentPointer(const CThing*) @ 0x00668B60 (out of line), CThing::PeekPlayer @ 0x004C7A10.
// Layout: Ego_r.pdb CTCDivineWrath PTarget +0x71 / IsZTargetting +0x79 sit at retail +0x6d / +0x75 (4 bytes lower; the pointer
// classes are `pack(1)`); retail CThing ThingTypeFlags +0x6c (PDB +0x7c), dead bit +0x91; the player's Z-targeting byte +0x20a
// (named descriptively).
// Shape: two assignments through a converting operator= (`SetPItem(other.GetPItem())`): from an inline CIntelligentPointer(0)
// temporary and, for the target, from an implicit conversion of a `const CThing*` local (`PTarget = target;` builds the
// out-of-line const-thing pointer temporary and re-addresses it with `lea ecx,[esp+8]`; an explicit
// `CIntelligentPointerConstThing(target)` functional cast reuses the ctor's returned this, `mov ecx,eax`); the targeting
// component is the one-ternary GetTC (straight into ecx for the virtual call).
#include "fable_compiler.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CPlayer { public:
    unsigned char _pad00[0x20a];
    bool ZTargeting;                                    // +0x20a
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad54[0x18];
    unsigned long ThingTypeFlags;                       // +0x6c
    unsigned char _pad70[0x91 - 0x70];
    unsigned char Flags91;                              // +0x91 bit 0 Dead
    FABLE_FORCEINLINE unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    FABLE_FORCEINLINE CTCBase* GetTCSelect(long interfaceType) const
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound(type);
        return (it == map->end() || interfaceType < it->first ? map->end() : it)->second;
    }
public:
    const CPlayer& PeekPlayer() const;   // 0x004C7A10
};
struct CTCTargetSource {                                // the interface-8 component
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32)
#undef V
    virtual CThing* GetTargetThing();                   // slot 33 (+0x84)
};
#pragma pack(push, 1)
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>
    virtual void v00();
    CIntelligentPointerConstThing(const CThing* item);  // 0x00668B60
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {     // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerCreature(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    FABLE_FORCEINLINE CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other) { CThing* item = ((CThing*)other.GetPItem()); SetPItem((CBaseObject*)item); return *this; }
    FABLE_FORCEINLINE CIntelligentPointerCreature& operator=(const CIntelligentPointerConstThing& other) { CThing* item = ((CThing*)other.GetPItem()); SetPItem((CBaseObject*)item); return *this; }
};
#pragma pack(pop)
class CTCDivineWrath { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x6d - 0x08];
    CIntelligentPointerCreature PTarget;                // +0x6d (PDB +0x71)
    bool IsZTargetting;                                 // +0x75 (PDB +0x79)
    void SetPTarget();
};

void CTCDivineWrath::SetPTarget()
{
    PTarget = CIntelligentPointerCreature(0);
    IsZTargetting = false;
    if (ParentThing->HasTC(8)) {
        const CThing* target = ((CTCTargetSource*)ParentThing->GetTCSelect(8))->GetTargetThing();
        if (target && !(target->Flags91 & 1) && (target->ThingTypeFlags & 8) && (ParentThing->ThingTypeFlags & 0x20)) {
            PTarget = target;
            IsZTargetting = ParentThing->PeekPlayer().ZTargeting;
        }
    }
}