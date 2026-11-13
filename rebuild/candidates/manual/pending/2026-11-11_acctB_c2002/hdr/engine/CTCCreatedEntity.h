#ifndef FABLE_ENGINE_CTCCREATEDENTITY_H
#define FABLE_ENGINE_CTCCREATEDENTITY_H
// New shared header (w22 agent2002, row 0x007B90F0 CTCTavernTable::OnInitialActivate): the created-entity component
// (TCI_CREATED_ENTITY 50). RTTI .?AVCTCCreatedEntity@@ : CTCBase. Members: Ego_r.pdb (struct_layouts_egor.tsv, sizeof 0x14):
// PCreator (CIntelligentPointer<const CThing>, byte-packed) +0x0C. Methods: the Ego_r.pdb / FableWin.pdb member set; SetPCreator
// (?SetPCreator@CTCCreatedEntity@@QAEXABVCThing@@@Z) has no out-of-line copy in Fable.exe: OnInitialActivate builds a
// CIntelligentPointer<const CThing> temporary (vtable 0x01237F1C, SetPItem) and assigns it to +0x0C (GetPItem, SetPItem) in place
// (0x007B94A8..0x007B94E0). Needs FABLE_RETAIL_CTCBASE and FABLE_RETAIL_CINTELLIGENTPOINTER (+ _ASSIGN for the assignment shape).
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#ifndef FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#endif
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ETCInterfaceType.h"
class CCharString;
class CThing;

class CTCCreatedEntity : public CTCBase {
public:
    CTCCreatedEntity(CThing&);                                           // ??0CTCCreatedEntity@@QAE@AAVCThing@@@Z
    static CTCBase* __fastcall Alloc(CThing&);                           // ?Alloc@CTCCreatedEntity@@SIPAVCTCBase@@AAVCThing@@@Z
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_CREATED_ENTITY; }   // (inline)
    static CCharString __fastcall StatGetClassTypeName();                // ?StatGetClassTypeName@CTCCreatedEntity@@SI?AVCCharString@@XZ
    virtual ETCInterfaceType GetInterfaceType() const;                   // ?GetInterfaceType@CTCCreatedEntity@@UBE?AW4ETCInterfaceType@@XZ
    virtual CCharString GetClassTypeName() const;                        // ?GetClassTypeName@CTCCreatedEntity@@UBE?AVCCharString@@XZ
    void SetPCreator(const CThing& creator) { PCreator = &creator; }     // ?SetPCreator@CTCCreatedEntity@@QAEXABVCThing@@@Z (inline)
private:
    CIntelligentPointer<const CThing> PCreator;                          // +0x0C
public:
    static void CheckRetailLayout() { FABLE_STATIC_ASSERT(offsetof(CTCCreatedEntity, PCreator) == 0x0c); }
};
FABLE_STATIC_ASSERT(sizeof(CTCCreatedEntity) == 0x14);
#endif
