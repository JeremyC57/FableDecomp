#pragma optimize("s", on)
// CTavernGameShoveHaPennyDef::`scalar deleting destructor'(unsigned int) -- destroy the member at +0x1a4 out of line, the CAIStateGroup_HoverWanderAroundThing member at +0 destroyed out of line, then the flag-tested operator delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E2D89.
// Callees: the member destructor @ 0x004DF9EF (the manifest label "CActiveFile::OnReadFinished" is propagated), the base destructor @ 0x004E108C (CAIStateGroup_HoverWanderAroundThing::~CAIStateGroup_HoverWanderAroundThing), operator delete @ 0x00BFE9BC. Shape: no vtable store (the class is
// non-polymorphic: the polymorphic object at +0 is a member, so VC7.1 emits the non-virtual deleting destructor); the member at +0x1a4 is destroyed first. The size-optimised module uses `pop ecx` after the delete. The chunk label
// "CEngineSubPrimitiveText `scalar deleting destructor'" is propagated; the class is named by address.
struct CMember_004df9ef {
    unsigned char _body[0x4];
    ~CMember_004df9ef();                                  // 0x004DF9EF
};

struct CAIStateGroup_HoverWanderAroundThing {                                  // polymorphic base of 0x1a4 bytes
    virtual void v00();
    unsigned char _pad[0x1a0];
    ~CAIStateGroup_HoverWanderAroundThing();                                   // 0x004E108C
};

struct CHoverWanderAroundThingGroup_004e2d89 {                                   // non-polymorphic: the polymorphic CAIStateGroup_HoverWanderAroundThing is its FIRST MEMBER, not a base (no vtable store, members destroyed in reverse order)
    CAIStateGroup_HoverWanderAroundThing m_base;                                 // +0x00
    CMember_004df9ef m_member;                            // +0x1a4
    __forceinline ~CHoverWanderAroundThingGroup_004e2d89() { }   // forced: the size-optimised TU would otherwise emit it out of line
};

// The delete-expression that makes the compiler emit the (non-virtual) scalar deleting destructor.
void Destroy_004e2d89(CHoverWanderAroundThingGroup_004e2d89* object)
{
    delete object;
}