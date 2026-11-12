// CCreatureAction_PickUpJugToFill::PerformAction() -- when the jug is alive: put it in the creature's carry slot named by the jug's carryable def (+0x30), add creature mode 0x2b and clear the "filled" byte of the jug's interface-0xBB component
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Leaf: ?PerformAction@CCreatureAction_PickUpJugToFill@@QAEXXZ
// agentNF25b (2026-11-12): on engine/CDefClassBase_GetSubDef.h / CCarryableDef.h (GetSubDef<CCarryableDef> const, no-inline opt-in); the
//   +0x30 slot is Ego_r CCarryableDef::PassiveCarrySlot, read into a named local before the call (the earlier made-up inline getter
//   GetFillCarrySlot() gave the same bytes; the plain inline read did not).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084A350, length 321 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PJug +0xa8, PCreature +0x08), CVectorMap::LowerBound 0x0040F020 (inlined
//   untested GetInterface: 0x46 CTCCarrying, bit 6 of +0x28; 0x31 CTCCreatureModeManager, bit 17 of +0x24; 0xBB the jug component,
//   bit 27 of +0x34), CDefClassBase::GetSubDef<CCarryableDef> 0x005CC10E, CTCCarrying::PutThingInCarrySlot 0x006A1F80,
//   CTCCreatureModeManager::AddMode(unsigned long) 0x0073AE10.
// Layout: retail CCreatureActionBase is 0xa8 bytes: the jug pointer is this class's first member (+0xa8); CThing +0x70 the def,
//   +0x91 dead bit; CCarryableDef +0x30 the carry slot used here; the jug component's byte +0x10.
// Shape: the landed CCreatureAction_PickUpGoods 0x00848730 model: IS_THING_ALIVE on PJug, every lookup unchecked (the miss path
//   reads the out word), the carryable def a CDefPointer local whose release has its null test folded (the def was dereferenced),
//   the GetSubDef and LowerBound same-TU stand-ins carry retail's LTCG knowledge (def in ebx, key and def share one frame word).
#include "fable_compiler.h"
class CThing;
class CThingObject;
#include "engine/C3DVector.h"
class CTCBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCARRYABLEDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE   // agentNF25b: the GetSubDef<CCarryableDef> template body (0x005CC10E) kept out of line, visible to the TU
#include "fable_string.h"
#include "engine/CDefPointer.h"
#include "engine/CCarryableDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad54[0x1c];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char _pad74[0x90 - 0x74];
    signed char m_player;                             // +0x90
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPos() const;             // 0x004C73D0
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingObject : public CThing { public: };
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CTCCarrying { public:
    void PutThingInCarrySlot(CThingObject* thing, long slot, bool immediately);   // 0x006A1F80
};
class CTCCreatureModeManager { public:
    bool AddMode(unsigned long mode);                 // 0x0073AE10
};
struct CTCJugComponent {                              // interface type 0xBB
    unsigned char _pad[0x10];
    unsigned char Filled;                             // +0x10
};
class CCreatureActionBase { public:                          // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0xa8 - 0x10];
};
class CCreatureAction_PickUpJugToFill : public CCreatureActionBase { public:
    CBaseIntelligentPointer PJug;                     // +0xa8
    void PerformAction();
};

void CCreatureAction_PickUpJugToFill::PerformAction()
{
    if (PJug.GetPItem() && !(((CThing*)PJug.GetPItem())->m_flags91 & 1)) {
        CTCCarrying* carrying;
        ((CThing*)PCreature.GetPItem())->GetInterface(&carrying, 0x46);
        CDefPointer<const CCarryableDef> carryDef;
        CThing* jug = ((CThing*)PJug.GetPItem());                    // named: the GetPItem call runs before the out-pointer push
        jug->m_pDef->GetSubDef(&carryDef);
        long slot = carryDef->PassiveCarrySlot;                      // named: read before the GetPItem call (was a made-up inline getter)
        carrying->PutThingInCarrySlot((CThingObject*)PJug.GetPItem(), slot, true);
        CTCCreatureModeManager* modes;
        ((CThing*)PCreature.GetPItem())->GetInterface(&modes, 0x31);
        modes->AddMode(0x2b);
        CTCJugComponent* jugComponent;
        ((CThing*)PJug.GetPItem())->GetInterface(&jugComponent, 0xbb);
        jugComponent->Filled = 0;
    }
}