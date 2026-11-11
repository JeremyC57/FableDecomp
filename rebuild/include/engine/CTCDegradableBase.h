#ifndef FABLE_ENGINE_CTCDEGRADABLEBASE_H
#define FABLE_ENGINE_CTCDEGRADABLEBASE_H
// NEW (agentNF10, row 0x008EC870 CTCDegradableBase::OnCreate): the base of the degradable components. RTTI .?AVCTCDegradableBase@@ :
// CTCBase (ghidra_out/rtti_bases.tsv); vtable 0x0126A6A4 slot 4 OnCreate 0x008EC870. Members: Ego_r.pdb CTCDegradableBase
// (struct_layouts_egor.tsv, 0x1C, no STL member: the same offsets in retail; OnCreate stores LastWorldFrameHit `[esi+0Ch]`,
// PCurrentDegradableInfo `[esi+14h]`, PParentThingPhysical `[esi+18h]` and fills PDefinition at +0x10). Methods: the Ego_r.pdb
// spellings the rows define (?OnCreate@CTCDegradableBase@@MAEXXZ: protected virtual). Needs FABLE_RETAIL_CTCBASE and
// FABLE_RETAIL_DEF_CLASS_BASE.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
class CDegradableDef;
class CDegradableInfo;
class CThingPhysical;
class CTCDegradableBase : public CTCBase {
protected:
    virtual void OnCreate();                                             // slot 4 0x008EC870
private:
    long                              LastWorldFrameHit;                 // +0x0c
    CDefPointer<const CDegradableDef> PDefinition;                       // +0x10
    const CDegradableInfo*            PCurrentDegradableInfo;            // +0x14
    CThingPhysical*                   PParentThingPhysical;              // +0x18
};
FABLE_STATIC_ASSERT(sizeof(CTCDegradableBase) == 0x1c);
#endif
