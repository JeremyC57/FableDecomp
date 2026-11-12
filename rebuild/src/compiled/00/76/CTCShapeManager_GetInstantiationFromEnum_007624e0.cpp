// CTCShapeManager::GetInstantiationFromEnum(NShape::EShape type, std::vector<C3DVector>& points) -- build a CBasicShape (type 0) or a CTrack (type 1) owned by this manager and return it in a counted pointer
// Leaf: ?GetInstantiationFromEnum@CTCShapeManager@@QAE?AV?$CCountedPointer@VCShape@NShape@@@@W4EShape@NShape@@AAV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007624E0, 160 bytes, `ret 0Ch` (hidden result, the type, the points).
// Names (agentNF27, 2026-11-12, FAM10 NAME_FIXES follow-up; was NShape::CShapeManager::CreateShape over stand-in types): Ego_r.pdb
//   (0x00773560) + FableWin.pdb ?GetInstantiationFromEnum@CTCShapeManager@@QAE?AV?$CCountedPointer@VCShape@NShape@@@@W4EShape@NShape@@AAV?$vector@VC3DVector@@...@Z;
//   Ego_r keeps it right after ??0CTrack@NShape@@ (0x00772F80) and ??0CBasicShape@NShape@@ (0x00773480), the retail order of its two
//   callees 0x00761F40 / 0x00762440, whose Ego_r / FableWin spellings take (CTCShapeManager&, const std::vector<C3DVector>&).
//   Reset 0x007642C0 is FableWin.pdb ?Reset@?$CCountedPointer@VCShape@NShape@@@@QAEXPAVCShape@NShape@@@Z (stores DeleteData<NShape::CShape>
//   0x00763A10), was the stand-in ?Reset@CShapePointer@NShape@@.
// Callees: operator new @ 0x00BFEA1A, NShape::CTrack::CTrack @ 0x00761F40 (0x28 bytes), NShape::CBasicShape::CBasicShape @ 0x00762440
//   (0x2C bytes), CCountedPointer<NShape::CShape>::Reset @ 0x007642C0, the count block's `__fastcall` DeleteFunc and operator delete
//   @ 0x00BFE9BC (the local's release).
// Shape: Reset gets a same-TU no-escape stand-in body (the result local's address does not escape it, so the return copy reads the
//   block once and folds add-ref + release into the two count stores; the real Reset is the out-of-line COMDAT); the result counted
//   pointer is a local default-constructed to {0, 0}, filled by `Reset(new X(*this, points))` inside a two-case `switch` (the default
//   arm jumps straight to the return), and returned by value through the hidden pointer.
// Layout: engine/NShape_CClosedShapeBase.h (NShape::CShape 0x20: vptr, Owner, Points, SelectedPoints; CClosedShapeBase 0x20);
//   RTTI CBasicShape : CClosedShapeBase, CTrack : CShape; sizes from the `new` (0x2C / 0x28).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCSHAPEMANAGER
#define FABLE_RETAIL_CTCSHAPEMANAGER_GETINSTANTIATIONFROMENUM
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCBase.h"
#include "engine/CTCShapeManager.h"
#include "engine/NShape_CClosedShapeBase.h"

namespace NShape {
enum EShape { SHAPE_BASIC = 0, SHAPE_TRACK = 1 };   // enumerator names descriptive (values from the switch)

class CBasicShape : public CClosedShapeBase {       // 0x2C bytes
public:
    CBasicShape(CTCShapeManager& owner, const std::vector<C3DVector>& points);   // 0x00762440
    virtual bool IsPosInShape(const C3DVector& pos) const;
    unsigned char _own[0x2c - 0x20];
};

class CTrack : public CShape {                     // 0x28 bytes
public:
    CTrack(CTCShapeManager& owner, const std::vector<C3DVector>& points);        // 0x00761F40
    virtual EShape GetType() const;
    virtual void EditorDrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&, const CWorldMap&, const C3DVector&, const CRGBColour&);
    virtual bool IsPosInShape(const C3DVector& pos) const;
    unsigned char _own[0x28 - 0x20];
};
} // namespace NShape

CCPPointerInfo* __fastcall ResetBlock_NoEscape(NShape::CShape* object, NShape::CShape* oldObject, CCPPointerInfo* oldBlock);   // stand-in callee
// Stand-in body (the real Reset is out of line in retail, 0x007642C0): it touches the two words only inline, so VC7.1 knows the
// local's address does not escape the call (the landed 0x008C4170 idiom).
#pragma auto_inline(off)
template <> __declspec(noinline) void CCountedPointer<NShape::CShape>::Reset(NShape::CShape* object)
{
    Info = ResetBlock_NoEscape(object, Data, Info);
    Data = object;
}
#pragma auto_inline(on)

CCountedPointer<NShape::CShape> CTCShapeManager::GetInstantiationFromEnum(NShape::EShape type, std::vector<C3DVector>& points)
{
    CCountedPointer<NShape::CShape> shape;
    switch (type) {
    case NShape::SHAPE_BASIC:
        shape.Reset(new NShape::CBasicShape(*this, points));
        break;
    case NShape::SHAPE_TRACK:
        shape.Reset(new NShape::CTrack(*this, points));
        break;
    }
    return shape;
}
