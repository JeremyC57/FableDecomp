// CAIStateGroup_GoToPub::CAIStateGroup_GoToPub() -- base CAIStateGroupBase(), this vtable, then the five intelligent-pointer members
// Leaf: ??0CAIStateGroup_GoToPub
// Pragma: optimize("s", on) -- the size TU caches the vtable shared by PBar/PTable in ebx (speed stores it twice).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00900173, length 99 bytes.
// Callees: CAIStateGroupBase::CAIStateGroupBase @ 0x008FD255, CBaseIntelligentPointer::CBaseIntelligentPointer
// @ 0x00A01B10 (x5). Vtables (rtti-vtables.tsv): CAIStateGroup_GoToPub 0x0128BEC4, CIntelligentPointer<..>
// 0x01244344 (PPub), 0x0125A858 (PBar and PTable share one instantiation: the vtable is cached in ebx),
// 0x0123E7E0 (PBarman), 0x01238C6C (PTankard).
//
// Shape: the derived vtable is stored after the base constructor and before the members; each member is
// the inline default constructor CIntelligentPointer<T>() : CBaseIntelligentPointer() {} (base call, then
// its own vtable store through the member's address register edi). PDB layout (sizeof 0xA8): the five
// pointers at +0x14..+0x34, then SeatIndex, Pos, RHSet and the bar state (C3DVector / CRightHandedSet have
// no constructor code). The engine has no CAIStateGroup_GoToPub header: local model from the PDB layout.
#pragma optimize("s", on)

class CThing;
class CThingObject;
class CThingBuilding;
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

struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };

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

struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    CIntelligentPointer<CThingBuilding> PPub;       // +0x14
    CIntelligentPointer<CThingObject> PBar;         // +0x1c
    CIntelligentPointer<CThingObject> PTable;       // +0x24
    CIntelligentPointer<CThingAICreature> PBarman;  // +0x2c
    CIntelligentPointer<CThing> PTankard;           // +0x34
    long SeatIndex;                             // +0x3c
    C3DVector Pos;                              // +0x40
    CRightHandedSet RHSet;                      // +0x4c
    long ActionPointIndex;                      // +0x64
    long NextFramePub;                          // +0x68
    long FrameLeavePub;                         // +0x6c
    bool Sitting;                               // +0x70
    long BarActionPointIndex;                   // +0x74
    C3DVector BarPos;                           // +0x78
    CRightHandedSet BarRHSet;                   // +0x84
    long BarIndex;                              // +0x9c
    bool HandsOffMode;                          // +0xa0
    bool UnderScriptControl;                    // +0xa1
    long TimeBeforeAskingForBeer;               // +0xa4
    virtual ~CAIStateGroup_GoToPub();
    CAIStateGroup_GoToPub();
};

CAIStateGroup_GoToPub::CAIStateGroup_GoToPub()
    : CAIStateGroupBase()
{
}
