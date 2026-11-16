// CTCFishingRod::CTCFishingRod(CThing* parent) -- CTCBase(parent), this vtable, the def pointer cleared, then filled from the parent's def class (GetSubDef<CFishingRodDef>)
// Leaf: ??0CTCFishingRod@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase (which bound ??0CTCBase@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as the PDBs spell it (??0CTCFishingRod@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E2450, length 46 bytes.
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800, CDefClassBase::GetSubDef<CFishingRodDef>(CDefPointer<..>*) @ 0x007E2390
// (on the def class at +0x70 of the parent thing; the bool result is dropped). Vtable 0x01270104 = CTCFishingRod
// (rtti-vtables.tsv); the landed factory 0x004D5F65 allocates 0x10 bytes for it.
// Layout: PDB CTCFishingRod (16 bytes): PDef +0xc (CDefPointer<CFishingRodDef const>).
// Shape: the def pointer's address is formed once (`lea eax,[esi+0Ch]`, before the vtable store) and serves both the
// inline constructor's zero store (`mov dword ptr [eax],0`) and the GetSubDef argument; ParentThing is re-read for the
// def class chain. No EH frame: the module is built without /EHsc.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CDefPointeeBase;

struct CDefPointer {                             // CDefPointer<CFishingRodDef const>
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) { }
};

class CDefClassBase { public:
    bool GetSubDef_007e2390(CDefPointer* out);            // 0x007E2390 (GetSubDef<CFishingRodDef>)
};

class CThing { public:
    unsigned char  _pad[0x70];
    CDefClassBase* m_pDefClass;                  // +0x70
};

class CTCFishingRod : public CTCBase { public:
    CDefPointer PDef;                            // +0x0c
    CTCFishingRod(CThing& parent);
};

CTCFishingRod::CTCFishingRod(CThing& parent)
    : CTCBase(parent), PDef()
{
    ParentThing->m_pDefClass->GetSubDef_007e2390(&PDef);
}