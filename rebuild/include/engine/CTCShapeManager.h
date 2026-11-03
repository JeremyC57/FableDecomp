#ifndef FABLE_ENGINE_CTCSHAPEMANAGER_H
#define FABLE_ENGINE_CTCSHAPEMANAGER_H
#include "engine/CTCBase.h"
#include "engine/C2DBoxF.h"
// Ego_r TPI0x2cd5b; original CTCBase and vector reduction put complete native extent at30.
class CTCShapeManager : public CTCBase {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
    bool IsPosInAnyShape(const C3DVector&) const;
private:
    unsigned char PersistantShapes[12]; // vector<CCountedPointer<NShape::CShape> >
    unsigned char SelectedShape[4]; // Original STLport vector iterator
    bool Activated,NewShapeActivated,LeavingShape;
    C2DBoxF ClosedShapeBounds;
};
FABLE_STATIC_ASSERT(sizeof(CTCShapeManager)==0x30);
#endif
