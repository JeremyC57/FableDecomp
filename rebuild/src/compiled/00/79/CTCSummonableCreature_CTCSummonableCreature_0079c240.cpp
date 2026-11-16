// CTCSummonableCreature::CTCSummonableCreature(CThing* parent) -- thing-component base, null def, not yet summoned, empty summoner pointer, then fetch the summonable-creature sub-def from the parent thing's def
// Leaf: ??0CTCSummonableCreature@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase (which bound ??0CTCBase@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as the PDBs spell it (??0CTCSummonableCreature@@QAE@AAVCThing@@@Z).
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0079C240, length 70 bytes.
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10,
// CDefClassBase::GetSubDef<CSummonableCreatureDef>(CDefPointer<T>*) const @ 0x0079C4B0 (manifest
// "GetSubDef<CAICreatureWillPowerIndicatorDef>" names a byte-identical instantiation).
// Vtables: 0x0126C41C = CTCSummonableCreature, 0x0123E7D8 = CIntelligentPointer (rtti-vtables.tsv).
// Layout: Ego_r.pdb CTCSummonableCreature (0x1c bytes): PDef (CDefPointer<CSummonableCreatureDef const>) +0x0c,
// HasBeenSummoned +0x10, Summoner (CIntelligentPointer<CThingCreatureBase>) at the UNALIGNED +0x11; the thing's counted
// def pointer is at +0x70 (as CTCAISummon::OnInitialActivate 0x007FDE70).
// Shape: vtable first, then the member constructors in order (the def pointer's inline constructor stores an immediate
// zero, the bool initialiser, the intelligent pointer's base call + vtable), then the body's GetSubDef into PDef.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;
class CBaseObject;

class CSummonableCreatureDef;

struct CDefPointerSummonable {                              // CDefPointer<CSummonableCreatureDef const>
    const CSummonableCreatureDef* Object;
    CDefPointerSummonable() : Object(0) { }
};

struct CThingDef {
    bool GetSubDef(CDefPointerSummonable* out) const;       // 0x0079C4B0
};

class CThing { public:
    unsigned char _pad[0x70];
    CThingDef*    PDef;                                     // +0x70 (counted def pointer: object word)
};

#pragma pack(push, 1)
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CThingCreatureBase;

class CTCSummonableCreature : public CTCBase { public:
    CDefPointerSummonable PDef;                             // +0x0c
    bool                  HasBeenSummoned;                  // +0x10
    CIntelligentPointer<CThingCreatureBase>   Summoner;                         // +0x11
    CTCSummonableCreature(CThing& parent);
};
#pragma pack(pop)

CTCSummonableCreature::CTCSummonableCreature(CThing& parent)
    : CTCBase(parent)
    , HasBeenSummoned(false)
{
    ParentThing->PDef->GetSubDef(&PDef);
}