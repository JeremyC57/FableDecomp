// CAIStateGroup_OpinionReactionRoot::TryAndLookOnAwarenessUpdate() -- one look-at-position step towards the thing reacted to.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009830C0, length 67 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (ThingReactingTo at +0x6b), CThing::GetPosition
// @ 0x004C73D0 (manifest "CVertexBufferWin32::DoSizeof" is propagated), CAIStateGroupBase::AIAction_LookAtPos
// (EAIActionRepeat, C3DVector, long, float) @ 0x0097D580 (repeat 1, flags 0x21, turn speed 0.0027777f).
// Shape: the thing is fetched into a named local first (the bare expression calls GetPItem after the pushes), the two literals pushed, then the by-value C3DVector (user copy constructor)
// is built in its argument slot (`sub esp,0Ch ; mov ecx,esp` + three dword moves) from the returned position,
// then the repeat mode. `xor ecx,ecx ; cmp eax,2 ; sete cl ; mov al,cl` is a plain `return r == 2;`.
// The pointer member is unaligned at +0x6b (engine header, pack 1), so the class is modelled packed.
class CThing;

class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

class CThing { public:
    const C3DVector& GetPos() const;    // 0x004C73D0
};
class CThingPos : public CThing { public:
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];                   // +0x4..+0x13
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float turnSpeed);   // 0x0097D580
};

class CTCTalk;

#pragma pack(push, 1)
class CAIStateGroup_OpinionReactionRoot : public CAIStateGroupBase { public:
    CTCTalk* PTCTalk;                           // +0x14
    long CurrentReactionType;                   // +0x18
    bool ReactionFlags[79];                     // +0x1c
    CIntelligentPointer<CThingPos> ThingReactingTo;    // +0x6b CIntelligentPointer<CThing>
    bool TryAndLookOnAwarenessUpdate();
};
#pragma pack(pop)

bool CAIStateGroup_OpinionReactionRoot::TryAndLookOnAwarenessUpdate()
{
    CThingPos* pThing = ThingReactingTo.GetPItem();   // fetched before the literal pushes (a named local)
    return AIAction_LookAtPos(1, pThing->GetPos(), 0x21, 0.0027777f) == 2;
}