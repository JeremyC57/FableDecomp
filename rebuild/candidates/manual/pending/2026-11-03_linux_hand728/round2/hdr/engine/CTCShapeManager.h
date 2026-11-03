#ifndef FABLE_ENGINE_CTCSHAPEMANAGER_H
#define FABLE_ENGINE_CTCSHAPEMANAGER_H
#include "engine/CTCBase.h"
#include "engine/C2DBoxF.h"
#ifdef FABLE_RETAIL_CTCSHAPEMANAGER
#include "engine/ETCInterfaceType.h"
#endif
// Ego_r TPI0x2cd5b; original CTCBase and vector reduction put complete native extent at30.
class CTCShapeManager : public CTCBase {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
    bool IsPosInAnyShape(const C3DVector&) const;
#ifdef FABLE_RETAIL_CTCSHAPEMANAGER
    // Retail opt-in (chunk 728, row 0x0074B770 CTCCreatureGenerator::InitialiseCreatureGenerator): the inline static type query every
    // CTC defines (StatGetInterfaceType, no out-of-line copy for this class in either PDB); retail GetInterfaceType returns 0x44 and
    // GetTC<CTCShapeManager> tests UsingComponents bit 0x44 (`test byte ptr [eax+28h],10h`) and compares the found key with 44h.
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SHAPE_MANAGER; }
#endif
private:
    unsigned char PersistantShapes[12]; // vector<CCountedPointer<NShape::CShape> >
    unsigned char SelectedShape[4]; // Original STLport vector iterator
    bool Activated,NewShapeActivated,LeavingShape;
    C2DBoxF ClosedShapeBounds;
};
FABLE_STATIC_ASSERT(sizeof(CTCShapeManager)==0x30);
#endif
