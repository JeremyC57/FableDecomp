// CAIStateGroup_HoverWanderAroundThing::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CAIStateGroup_HoverWanderAroundThing inlined (the member's destructor, then the base chain down to the first user-written destructor), and operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCAIStateGroup_HoverWanderAroundThing@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090292E, 44 bytes, thiscall, `ret 4`; reached only through vtable slot 0.
// Identity (proven): CAIStateGroup_HoverWanderAroundThing::`scalar deleting destructor'; evidence: vtable 0x0128D1BC slot 0 (RTTI .?AVCAIStateGroup_HoverWanderAroundThing@@ : CAIStateGroup_HoverMovementBase : CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase : CBaseClass)
// Label: the manifest's "`vector_deleting_destructor'" is propagated across byte-identical bodies (family J065: five deleting destructors of five unrelated classes).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (PThing, +0x1C: the implicit ~CIntelligentPointer<const CThingGameObject>), CAIStateGroup_EnemyCreatureBase::~CAIStateGroup_EnemyCreatureBase 0x008FD769 (after the inline user-written ~CAIStateGroup_HoverMovementBase stores 0x0128A1DC), operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CAIStateGroup_HoverWanderAroundThing / CAIStateGroup_EnemyCreatureBase / CAIStateGroupBase members at their PDB offsets.
// Shape: no vtable store for CAIStateGroup_HoverWanderAroundThing itself -- VC7.1 resets the vptr only in user-written destructors (agent952's finding, 0x006DF8C0); the stored
//   vtable 0x0128A1DC belongs to the first user-written (inline) base destructor. The deleting destructor is emitted with the vtable, i.e. in a TU
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
class CCharString {                                      // 4 bytes: the shared string data pointer
public:
    CCharString();
    ~CCharString();                                      // 0x0099EAE0
    void* PStringData;
};
class CAIBrain;
class CThingCreatureBase;
class CThingGameObject;
enum EScriptAIPriority { };                              // Ego_r.pdb (enumerators not needed)
class CAIStateGroupBase : public CBaseClass {            // Ego_r.pdb 20 bytes
public:
    virtual ~CAIStateGroupBase();
    CAIBrain*         PBrain;                            // +0x04
    EScriptAIPriority ScriptAIPriority;                  // +0x08
    CCharString       Name;                              // +0x0C
    bool              Terminated;                        // +0x10
    unsigned char     Priority;                          // +0x11
    bool              Enabled;                           // +0x12
};
class CAIStateGroup_EnemyCreatureBase : public CAIStateGroupBase {   // Ego_r.pdb 28 bytes
public:
    virtual ~CAIStateGroup_EnemyCreatureBase();          // 0x008FD769
    CIntelligentPointer<CThingCreatureBase> PTargetThing;   // +0x14
};
class CAIStateGroup_HoverMovementBase : public CAIStateGroup_EnemyCreatureBase {   // Ego_r.pdb 28 bytes, vtable 0x0128A1DC
public:
    CAIStateGroup_HoverMovementBase();
protected:
    virtual ~CAIStateGroup_HoverMovementBase() { }       // user-written, inline (FableWin.pdb ??1...@@MAE@XZ): stores 0x0128A1DC, then the base out of line
};
class CAIStateGroup_HoverWanderAroundThing : public CAIStateGroup_HoverMovementBase {   // vtable 0x0128D1BC, Ego_r.pdb 36 bytes
public:
    CIntelligentPointer<const CThingGameObject> PThing;  // +0x1C
};
void* MakeOne()
{
    return new CAIStateGroup_HoverWanderAroundThing;
}
void KillOne(CAIStateGroup_HoverWanderAroundThing* p) { p->~CAIStateGroup_HoverWanderAroundThing(); }