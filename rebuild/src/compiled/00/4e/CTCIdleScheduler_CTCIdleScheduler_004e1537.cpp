// CTCIdleScheduler::CTCIdleScheduler(CThing* parent) -- CTCBase(parent), own vtable, the two idle-state maps default-constructed, then Reset() clears the maps and zeroes Preferred / FrameLastChecked
// Leaf: ??0CTCIdleScheduler
// Retail: Fable.exe @ 0x004E1537, 47 bytes (unaligned, no padding: a size-optimised module).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, map<EIdleStateGroup, CAIStateGroupBase*>::map() @ 0x004DFAA7,
// map<EIdleStateGroup, long>::map() @ 0x004DFA89 (both out-of-line frame-pointer template constructors, labelled
// "CDisplayManager::CopyBackBufferToTexture" by propagation), CTCIdleScheduler::Reset @ 0x008018B0 (labelled
// "LTextBinTree_RebuildWalkerArray"; it clears both maps through their erase helpers and zeroes +0x24 / +0x28).
// Label: the manifest's "CTattooDef::CTattooDef" is wrong -- the vtable stored, 0x012426C4, is CTCIdleScheduler in
// rtti-vtables.tsv, and the PDB CTCIdleScheduler (IdleStates +0xc, IdleStatesWeight, Preferred, FrameLastChecked)
// fits the member constructors at +0xc / +0x18 and the zeroed +0x24 / +0x28 (retail maps are 0xc bytes, PDB 0x10).
// Pragma: #pragma optimize("s", on) -- `push dword ptr [esp+8]` for the forwarded argument is the /Os shape.
// Shape: the derived vtable store follows the base call and precedes the member constructors; the Reset call is the body.
#pragma optimize("s", on)
struct CThing;
struct CAIStateGroupBase;

struct CTCBase_Calls {                             // CTCBase, 0xc bytes
    virtual void v00();
    unsigned char _body[0x8];                      // ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9
    CTCBase_Calls(CThing* parent);                 // 0x00686800
};

struct CIdleStateMap {                             // map<EIdleStateGroup, CAIStateGroupBase*> (retail 0xc bytes)
    void*         _Myhead;                         // +0x00
    unsigned long _Mysize;                         // +0x04
    unsigned long _Extra;                          // +0x08
    CIdleStateMap();                               // 0x004DFAA7
};

struct CIdleWeightMap {                            // map<EIdleStateGroup, long> (retail 0xc bytes)
    void*         _Myhead;                         // +0x00
    unsigned long _Mysize;                         // +0x04
    unsigned long _Extra;                          // +0x08
    CIdleWeightMap();                              // 0x004DFA89
};

struct CTCIdleScheduler : CTCBase_Calls {          // vtable 0x012426C4
    CIdleStateMap  IdleStates;                     // +0x0c
    CIdleWeightMap IdleStatesWeight;               // +0x18
    long           Preferred;                      // +0x24 EIdleStateGroup
    long           FrameLastChecked;               // +0x28
    virtual void v00();
    CTCIdleScheduler(CThing* parent);
    void Reset();                                  // 0x008018B0
};

CTCIdleScheduler::CTCIdleScheduler(CThing* parent)
    : CTCBase_Calls(parent)
{
    Reset();
}