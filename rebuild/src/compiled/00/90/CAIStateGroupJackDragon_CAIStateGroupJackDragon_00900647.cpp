// CAIStateGroup_JackDragon::CAIStateGroup_JackDragon() -- base CAIStateGroupBase(), this vtable, then the seven intelligent-pointer members
// Leaf: ??0CAIStateGroup_JackDragon
// Pragma: optimize("s", on) -- the size TU caches the vtable shared by the five CThing pointers in edi
// (the 0x009001xx..0x009006xx state-group constructors are one size-optimised module; see 0x00900173).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00900647, length 119 bytes.
// Callees: CAIStateGroupBase::CAIStateGroupBase @ 0x008FD255, CBaseIntelligentPointer::CBaseIntelligentPointer
// @ 0x00A01B10 (x7). Vtables (rtti-vtables.tsv): CAIStateGroup_JackDragon 0x0128C170, CIntelligentPointer<..>
// 0x0123E7D8 (PEnemy, CThingCreatureBase), 0x01238C6C (the five CThing markers, cached in edi),
// 0x0123E7E0 (PInvisibleBodgeCreature, CThingAICreature).
//
// Shape: derived vtable after the base constructor, then each member's inline default constructor
// CIntelligentPointer<T>() : CBaseIntelligentPointer() {} (its vtable stored through the member address
// in ebx/edi). PDB layout: PEnemy +0x14, PLandedMarker +0x1c, PHoverMarker1/2 +0x24/+0x2c,
// PStrafeMarker1/2 +0x34/+0x3c, PInvisibleBodgeCreature +0x44, then frames/state and the dragon position
// table (no constructor code). No engine header for CAIStateGroup_JackDragon: local model.
#pragma optimize("s", on)

class CThing;
class CThingCreatureBase;
class CThingAICreature;

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
};

template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();                         // one vtable per instantiation
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    void* PBrain;                               // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CAIStateGroupBase();                        // 0x008FD255
};

struct CAIStateGroup_JackDragon : CAIStateGroupBase {
    CIntelligentPointer<CThingCreatureBase> PEnemy;                 // +0x14
    CIntelligentPointer<CThing> PLandedMarker;                      // +0x1c
    CIntelligentPointer<CThing> PHoverMarker1;                      // +0x24
    CIntelligentPointer<CThing> PHoverMarker2;                      // +0x2c
    CIntelligentPointer<CThing> PStrafeMarker1;                     // +0x34
    CIntelligentPointer<CThing> PStrafeMarker2;                     // +0x3c
    CIntelligentPointer<CThingAICreature> PInvisibleBodgeCreature;  // +0x44
    long FrameOfLastSlam;                       // +0x4c
    long FrameWhenLastInRange;                  // +0x50
    long FrameWhenLanded;                       // +0x54
    long FramesInSnapRange;                     // +0x58
    long CurrentState;                          // +0x5c EDragonState
    long GoLeft;                                // +0x60
    unsigned char _rest[0xd4];                  // +0x64 BattleTriggered, flight positions (PDB packs C3DVectors at odd offsets)
    virtual ~CAIStateGroup_JackDragon();
    CAIStateGroup_JackDragon();
};

CAIStateGroup_JackDragon::CAIStateGroup_JackDragon()
    : CAIStateGroupBase()
{
}