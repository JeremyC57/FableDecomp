#ifndef FABLE_ENGINE_CTCSINGLEPERSONOCCUPIABLE_H
#define FABLE_ENGINE_CTCSINGLEPERSONOCCUPIABLE_H
// CTCSinglePersonOccupiable (new, chunk 767): RTTI .?AVCTCSinglePersonOccupiable@@, single base CTCBase at +0 (ghidra_out/rtti_bases.tsv;
// vtable 0x0123ED54, interface type TCI_SINGLE_PERSON_OCCUPIABLE = 172). Ego_r.pdb struct_layouts_egor.tsv (20 bytes): POccupier
// (CIntelligentPointer<CThingCreatureBase>) +0x0C. Name set (Ego_r.pdb donor order SetOccupier, GetOccupier, IsOccupied, IsOfType =
// retail 0x0087C390, 0x0087C3E0, 0x0087C3F0, 0x0087C420). Only the members rows use; no virtual overrides.
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
class CThingCreatureBase;
class CTCSinglePersonOccupiable : public CTCBase {
public:
    void SetOccupier(CThingCreatureBase* occupier);                      // 0x0087C390
    CThingCreatureBase* GetOccupier() const;                             // 0x0087C3E0
protected:
    CIntelligentPointer<CThingCreatureBase> POccupier;                   // +0x0c
};
#endif // FABLE_ENGINE_CTCSINGLEPERSONOCCUPIABLE_H
