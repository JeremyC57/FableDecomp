// CTCBob::CTCBob(CThing* owner) -- CTCBase(owner), this class's vtable, then tell the owner thing how to behave in cutscenes (behaviour 2)
// Leaf: ??0CTCBob
//
// Retail 0x00787750, 35 bytes (`ret 4`). Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (the chunk
// label "CEngineSubPrimitive2DClockSprite" is propagated), CThing::SetCutscenePauseBehaviour(
// ECutsceneBehaviour) @ 0x004C78A0 on the parent thing. Vtable 0x0126B76C = CTCBob (rtti-vtables.tsv).
// Shape: a constructor body call on ParentThing (+0x04); VC7.1 schedules the vtable store between the
// `mov ecx,[esi+4] ; push 2` argument setup and the call. Rhset/Pos/ForceVisible are not touched
// here (OnInitialActivate 0x00787780 fills them), so no member initialisers.

class CThing
{ public:
    void SetCutscenePauseBehaviour(long behaviour);  // 0x004C78A0 (ECutsceneBehaviour)
};

enum { CUTSCENE_BEHAVIOUR_2 = 2 };

class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Forward, Up; };

#pragma pack(push, 1)
class CTCBase
{ public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    bool    ToBeRemoved;                            // +0x08
    bool    IsInGlobalUpdateTCs;                    // +0x09
    unsigned char _pad_0x0a[0x2];                   // +0x0a
    CTCBase(CThing* owner);                         // 0x00686800
};

class CTCBob : public CTCBase                             // vtable 0x0126B76C
{ public:
    CRightHandedSet Rhset;                          // +0x0c
    C3DVector       Pos;                            // +0x24
    bool            ForceVisible;                   // +0x30

    virtual void v00();
    CTCBob(CThing* owner);
};
#pragma pack(pop)

CTCBob::CTCBob(CThing* owner)
    : CTCBase(owner)
{
    ParentThing->SetCutscenePauseBehaviour(CUTSCENE_BEHAVIOUR_2);
}