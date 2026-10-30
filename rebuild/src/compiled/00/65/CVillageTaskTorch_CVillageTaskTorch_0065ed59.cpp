// CVillageTaskTorch::~CVillageTaskTorch() -- the compiler-generated destructor of CVillageTaskTorch: the member at +0x10 then the member at +0x08 (reverse declaration order), then the inline CVillageTaskBase::~CVillageTaskBase() (its vptr store) and a tail jump to CBaseObject::~CBaseObject()
// Leaf: ??1CVillageTaskTorch@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065ED59, 33 bytes, thiscall, tail `jmp`.
// Identity: CVillageTaskTorch -- proven; evidence: its only caller is the scalar deleting destructor 0x0065ED3D in slot 0 of vtable 0x0125A8AC
//   (RTTI .?AVCVillageTaskTorch@@ : CVillageTaskBase, CBaseObject, CBaseClass); FableWin.pdb names ??1CVillageTaskTorch@@UAE@XZ; members per Ego_r.pdb CVillageTaskTorch (Torch +0x08, CreatureAssignedToTask +0x10).
// Label: the manifest's "~CAIStateGroup_HoverHornetQueen" is propagated across byte-identical bodies (no CAIStateGroup_HoverHornetQueen is involved
//   except at 0x00903383).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 (twice: each CIntelligentPointer member's implicit destructor is its base's, called directly), CBaseObject::~CBaseObject @ 0x0099A430 (tail jmp).
// Shape: no vptr store of CVillageTaskTorch's own (the destructor is implicit: VC7.1 stores the vptr only in user-written destructors);
//   the members' destructors run in reverse order, then CVillageTaskBase's inline user-written destructor stores vtable 0x0125A830
//   (RTTI .?AVCVillageTaskBase@@) and the call to CBaseObject::~CBaseObject() becomes the tail `jmp`. A size-optimised TU (odd addresses, no
//   16-byte alignment): `#pragma optimize("s", on)`. The class is constructed once (MakeOne) so its vtable, scalar
//   deleting destructor and this destructor are emitted.
#pragma optimize("s", on)
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#pragma pack(push, 1)
class CBaseIntelligentPointer {                          // 8 bytes, 1-aligned in retail
public:
    CBaseIntelligentPointer();
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    void* PItem;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
#pragma pack(pop)
class CBaseObjectPointer;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CVillageTaskBase : public CBaseObject {            // 8 bytes, vtable 0x0125A830
public:
    virtual ~CVillageTaskBase() { }
};
class CThing;
class CThingObject;
class CVillageTaskTorch : public CVillageTaskBase {
public:
    CIntelligentPointer<CThing> Torch;                  // +0x08
    CIntelligentPointer<const CThing> CreatureAssignedToTask;   // +0x10
};
void* MakeOne() { return new CVillageTaskTorch; }