// CAIStateGroup_TavernServer::~CAIStateGroup_TavernServer() -- the compiler-generated (implicit) destructor, kept out of line: ~CIntelligentPointer +0x34, ~CIntelligentPointer +0x2C, ~CIntelligentPointer +0x24, ~CIntelligentPointer +0x1C, ~CIntelligentPointer +0x14, then the base destructor out of line
// Leaf: ??1CAIStateGroup_TavernServer@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00901750, 51 bytes (+ int3 padding), thiscall, no return value; reached from the scalar deleting destructors listed below.
// Identity (proven): CAIStateGroup_TavernServer::~CAIStateGroup_TavernServer(); evidence: its callers are the scalar deleting destructors of CAIStateGroup_TavernServer 0x00901734 (slot 0 of vtable 0x0128CD14, RTTI .?AVCAIStateGroup_TavernServer@@ : ?$CAIStateGroup_WorkBase@VCAIStateGroupBase@@ : CAIStateGroupBase : CBaseClass); the destroyed offsets +0x14/+0x1C/+0x24/+0x2C/+0x34 are Ego_r.pdb CAIStateGroup_TavernServer's class-typed members, and the tail jump is ~CAIStateGroupBase 0x008FD288.
// Label: the manifest's "_Dest_val<std::allocator<CEditLevelMerger::CConflict>,CEditLevelMerger::CConflict>" is propagated across byte-identical bodies (family J072: five implicit destructors of
//   five classes, identical up to member offsets and callees; retail has no identical-code folding, so each address is one function).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (the implicit ~CIntelligentPointer<T>); CAIStateGroupBase::~CAIStateGroupBase 0x008FD288 (tail jmp; PDB-spelled ??1CAIStateGroupBase@@UAE@XZ).
// Layout: Ego_r.pdb CAIStateGroup_TavernServer / CAIStateGroupBase members at their PDB offsets (no containers: retail = Ego_r).
// Shape: no vtable store -- VC7.1 resets the vptr only in user-written destructors (agent952's finding, 0x006DF8C0); the class-typed
//   members are destroyed in reverse declaration order, then the base destructor CAIStateGroupBase::~CAIStateGroupBase (a tail `jmp`). KillOne's explicit destructor call is a modelling
//   device: it makes VC7.1 emit the implicit destructor, which it keeps out of line.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
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
    CCharString();                                       // 0x0099E4B0
    ~CCharString();                                      // 0x0099EAE0
    void* PStringData;
};
class CThingBuilding;
class CThingObject;
class CAIBrain;
enum EScriptAIPriority { };                              // Ego_r.pdb (enumerators not needed)
class CAIStateGroupBase : public CBaseClass {            // Ego_r.pdb 20 bytes
public:
    CAIStateGroupBase();                                 // FableWin.pdb ??0CAIStateGroupBase@@QAE@XZ
    virtual ~CAIStateGroupBase();                        // 0x008FD288 (user-written: stores its vtable)
    CAIBrain*         PBrain;                            // +0x04
    EScriptAIPriority ScriptAIPriority;                  // +0x08
    CCharString       Name;                              // +0x0C
    bool              Terminated;                        // +0x10
    unsigned char     Priority;                          // +0x11
    bool              Enabled;                           // +0x12
};
class C3DVector {                                        // Ego_r.pdb 12 bytes
public:
    float X;
    float Y;
    float Z;
};
class CRightHandedSet {                                  // Ego_r.pdb 24 bytes
public:
    C3DVector Up;
    C3DVector Forward;
};
template <class T>
class CAIStateGroup_WorkBase : public T { };             // Ego_r.pdb CAIStateGroup_WorkBase<CAIStateGroupBase>, 20 bytes, no members; implicit destructor
class CAIStateGroup_TavernServer : public CAIStateGroup_WorkBase<CAIStateGroupBase> {   // vtable 0x0128CD14, Ego_r.pdb 0x9C bytes
public:
    CAIStateGroup_TavernServer();                        // FableWin.pdb ??0CAIStateGroup_TavernServer@@QAE@XZ
    CIntelligentPointer<CThingObject>   PJug;            // +0x14
    CIntelligentPointer<CThingObject>   PBar;            // +0x1C
    CIntelligentPointer<CThingObject>   PTable;          // +0x24
    CIntelligentPointer<CThingObject>   PTankard;        // +0x2C
    CIntelligentPointer<CThingBuilding> PPub;            // +0x34
    long            JugIndex;                            // +0x3C
    long            ServingActionPointIndex;             // +0x40
    C3DVector       ServingPos;                          // +0x44
    CRightHandedSet ServingRHSet;                        // +0x50
    long            BarActionPointIndex;                 // +0x68
    C3DVector       BarPos;                              // +0x6C
    CRightHandedSet BarRHSet;                            // +0x78
    long            ActionPointIndex;                    // +0x90
    long            SeatIndex;                           // +0x94
    float           AverageWalkSpeed;                    // +0x98
};
void KillOne(CAIStateGroup_TavernServer* p) { p->CAIStateGroup_TavernServer::~CAIStateGroup_TavernServer(); }