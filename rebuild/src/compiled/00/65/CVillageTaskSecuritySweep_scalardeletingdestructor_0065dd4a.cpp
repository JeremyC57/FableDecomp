// CVillageTaskSecuritySweep::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CVillageTaskSecuritySweep inlined (the member's destructor, then the base chain down to the first user-written destructor), and operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCVillageTaskSecuritySweep@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065DD4A, 44 bytes, thiscall, `ret 4`; reached only through vtable slot 0.
// Identity (proven): CVillageTaskSecuritySweep::`scalar deleting destructor'; evidence: vtable 0x0125A868 slot 0 (RTTI .?AVCVillageTaskSecuritySweep@@ : CVillageTaskBase : CBaseObject : CBaseClass)
// Label: the manifest's "`vector_deleting_destructor'" is propagated across byte-identical bodies (family J065: five deleting destructors of five unrelated classes).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (CreatureAssignedToTask, +0x08: the implicit ~CIntelligentPointer<const CThing>), CBaseObject::~CBaseObject 0x0099A430 (after the inline user-written ~CVillageTaskBase stores 0x0125A830), operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CVillageTaskSecuritySweep members at their PDB offsets.
// Shape: no vtable store for CVillageTaskSecuritySweep itself -- VC7.1 resets the vptr only in user-written destructors (agent952's finding, 0x006DF8C0); the stored
//   vtable 0x0125A830 belongs to the first user-written (inline) base destructor. The deleting destructor is emitted with the vtable, i.e. in a TU
//   that constructs the class: MakeOne is that construction (a modelling device); the qualified explicit destructor calls make VC7.1 define the
//   implicit destructors before it generates the deleting destructor, so the chain inlines.
#pragma optimize("s", on)
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CThing;
#pragma pack(push, 1)
class CBaseIntelligentPointer {                          // 8 bytes, 1-aligned in retail
public:
    CBaseIntelligentPointer();
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    void* PData;                                         // CBaseObjectPointer*
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
#pragma pack(pop)
class CBaseObjectPointer;
class CBaseObject : public CBaseClass {                  // Ego_r.pdb 8 bytes, vtable 0x0129A7D8
protected:
    CBaseObject();
    virtual ~CBaseObject();                              // 0x0099A430 (FableWin.pdb ??1CBaseObject@@MAE@XZ)
public:
    CBaseObjectPointer* IntelligentPointer;              // +0x04
};
class CVillageTaskBase : public CBaseObject {            // Ego_r.pdb 8 bytes, vtable 0x0125A830
public:
    CVillageTaskBase();
    virtual ~CVillageTaskBase() { }                      // user-written, inline: stores 0x0125A830, then ~CBaseObject
};
class C3DVector {                                        // Ego_r.pdb 12 bytes
public:
    float X;
    float Y;
    float Z;
};
class CVillageTaskSecuritySweep : public CVillageTaskBase {   // vtable 0x0125A868, Ego_r.pdb 52 bytes
public:
    CIntelligentPointer<const CThing> CreatureAssignedToTask;   // +0x08
    C3DVector PositionToGoTo;                            // +0x10
    float     RadiusAroundThePositionToStay;             // +0x1C
    C3DVector PositionToLookTowards;                     // +0x20
    float     AngleToLookTowards;                        // +0x2C
    bool      HasBecomeInvalid;                          // +0x30
};
void* MakeOne()
{
    return new CVillageTaskSecuritySweep;
}
void KillOne(CVillageTaskSecuritySweep* p) { p->~CVillageTaskSecuritySweep(); }