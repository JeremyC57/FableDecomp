// CAIStateGroup_GoToPub::~CAIStateGroup_GoToPub() -- the compiler-generated (implicit) destructor, kept out of line: ~CIntelligentPointer +0x34, ~CIntelligentPointer +0x2C, ~CIntelligentPointer +0x24, ~CIntelligentPointer +0x1C, ~CIntelligentPointer +0x14, then the base destructor out of line
// Leaf: ??1CAIStateGroup_GoToPub@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FDCCA, 51 bytes (+ int3 padding), thiscall, no return value; reached from the scalar deleting destructors listed below.
// Identity (proven): CAIStateGroup_GoToPub::~CAIStateGroup_GoToPub(); evidence: its callers are the scalar deleting destructors of CAIStateGroup_GoToPub 0x009001D6 (slot 0 of vtable 0x0128BEC4, RTTI .?AVCAIStateGroup_GoToPub@@ : CAIStateGroupBase : CBaseClass), CAIStateGroup_GameMaster 0x00900341 (vtable 0x0128BFC4, derived from CAIStateGroup_GoToPub); the destroyed offsets +0x14/+0x1C/+0x24/+0x2C/+0x34 are Ego_r.pdb CAIStateGroup_GoToPub's class-typed members, and the tail jump is ~CAIStateGroupBase 0x008FD288.
// Label: the manifest's "_Dest_val<std::allocator<CEditLevelMerger::CConflict>,CEditLevelMerger::CConflict>" is propagated across byte-identical bodies (family J072: five implicit destructors of
//   five classes, identical up to member offsets and callees; retail has no identical-code folding, so each address is one function).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (the implicit ~CIntelligentPointer<T>); CAIStateGroupBase::~CAIStateGroupBase 0x008FD288 (tail jmp; PDB-spelled ??1CAIStateGroupBase@@UAE@XZ).
// Layout: Ego_r.pdb CAIStateGroup_GoToPub / CAIStateGroupBase members at their PDB offsets (no containers: retail = Ego_r).
// Shape: no vtable store -- VC7.1 resets the vptr only in user-written destructors (agent952's finding, 0x006DF8C0); the class-typed
//   members are destroyed in reverse declaration order, then the base destructor CAIStateGroupBase::~CAIStateGroupBase (a tail `jmp`). KillOne's explicit destructor call is a modelling
//   device: it makes VC7.1 emit the implicit destructor, which it keeps out of line.
class CBaseClass {                                       // RTTI root, vtable 0x01231710
public:
    CBaseClass();                                        // 0x0099A2F0
    virtual ~CBaseClass();                               // 0x0099A300
};
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
class CThing;
class CThingBuilding;
class CThingObject;
class CThingAICreature;
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
class CAIStateGroup_GoToPub : public CAIStateGroupBase {   // vtable 0x0128BEC4, Ego_r.pdb 0xA8 bytes
public:
    CAIStateGroup_GoToPub();                             // FableWin.pdb ??0CAIStateGroup_GoToPub@@QAE@XZ
    CIntelligentPointer<CThingBuilding>   PPub;          // +0x14
    CIntelligentPointer<CThingObject>     PBar;          // +0x1C
    CIntelligentPointer<CThingObject>     PTable;        // +0x24
    CIntelligentPointer<CThingAICreature> PBarman;       // +0x2C
    CIntelligentPointer<CThing>           PTankard;      // +0x34
    long            SeatIndex;                           // +0x3C
    C3DVector       Pos;                                 // +0x40
    CRightHandedSet RHSet;                               // +0x4C
    long            ActionPointIndex;                    // +0x64
    long            NextFramePub;                        // +0x68
    long            FrameLeavePub;                       // +0x6C
    bool            Sitting;                             // +0x70
    long            BarActionPointIndex;                 // +0x74
    C3DVector       BarPos;                              // +0x78
    CRightHandedSet BarRHSet;                            // +0x84
    long            BarIndex;                            // +0x9C
    bool            HandsOffMode;                        // +0xA0
    bool            UnderScriptControl;                  // +0xA1
    long            TimeBeforeAskingForBeer;             // +0xA4
};
void KillOne(CAIStateGroup_GoToPub* p) { p->CAIStateGroup_GoToPub::~CAIStateGroup_GoToPub(); }