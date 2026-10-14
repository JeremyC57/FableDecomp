// CAIStateGroup_OpinionReactionRoot::TryAndLookOnAwarenessUpdate() -- one look-at-position step towards the thing reacted to.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009830C0, length 67 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (ThingReactingTo at +0x6b), CThing::GetPosition
// @ 0x004C73D0 (manifest "CVertexBufferWin32::DoSizeof" is propagated), CAIStateGroupBase::AIAction_LookAtPos
// (EAIActionRepeat, C3DVector, long, float) @ 0x0097D580 (repeat 1, flags 0x21, turn speed 0.0027777f).
// Shape: the thing is fetched into a named local first (the bare expression calls GetPItem after the pushes), the two literals pushed, then the by-value C3DVector (user copy constructor)
// is built in its argument slot (`sub esp,0Ch ; mov ecx,esp` + three dword moves) from the returned position,
// then the repeat mode. `xor ecx,ecx ; cmp eax,2 ; sete cl ; mov al,cl` is a plain `return r == 2;`.
// The pointer member is unaligned at +0x6b (engine header, pack 1), so the class is modelled packed.
struct CThing;

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

struct CThingPos {
    C3DVector* GetPosition();                   // 0x004C73D0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThingPos* m_pItem;                         // +0x04
    CThingPos* GetPItem() const;                // 0x00A01B50
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];                   // +0x4..+0x13
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float turnSpeed);   // 0x0097D580
};

struct CTCTalk;

#pragma pack(push, 1)
struct CAIStateGroup_OpinionReactionRoot : CAIStateGroupBase {
    CTCTalk* PTCTalk;                           // +0x14
    long CurrentReactionType;                   // +0x18
    bool ReactionFlags[79];                     // +0x1c
    CBaseIntelligentPointer ThingReactingTo;    // +0x6b CIntelligentPointer<CThing>
    bool TryAndLookOnAwarenessUpdate();
};
#pragma pack(pop)

bool CAIStateGroup_OpinionReactionRoot::TryAndLookOnAwarenessUpdate()
{
    CThingPos* pThing = ThingReactingTo.GetPItem();   // fetched before the literal pushes (a named local)
    return AIAction_LookAtPos(1, *pThing->GetPosition(), 0x21, 0.0027777f) == 2;
}