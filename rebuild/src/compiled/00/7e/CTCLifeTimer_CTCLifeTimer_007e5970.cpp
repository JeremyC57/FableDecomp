// CTCLifeTimer::CTCLifeTimer(CThing* owner) -- construct the life timer component: CTCBase(owner), FramesLeft -1, FadeOutBeforeKill false, PerformOnDie true, then force the owner to update every frame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E5970, length 50 bytes.
// Leaf: ??0CTCLifeTimer
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CThing::SetForceFrameUpdate(bool) @ 0x004C7C60. Vtable: CTCLifeTimer
// @ 0x012706A4 (RTTI). Layout: PDB CTCLifeTimer (0x14): the CTCBase owner at +4, FramesLeft +0x0c, FadeOutBeforeKill +0x10,
// PerformOnDie +0x11. No engine header exists; local model.
// Shape: the initialisers follow the vtable store in declaration order, the owner pointer and the call's `push 1` are
// hoisted above them; the argument is loaded into eax and pushed (speed TU).
class CThing { public:
    void SetForceFrameUpdate(bool force);          // 0x004C7C60
};

class CTCBase { public:
    CTCBase(CThing* owner);                        // 0x00686800
    virtual ~CTCBase();                            // 0x00686830
    CThing* m_pThing;                              // +0x04
    long    m_baseWord;                            // +0x08
};

class CTCLifeTimer : public CTCBase { public:
    long FramesLeft;                               // +0x0c
    bool FadeOutBeforeKill;                        // +0x10
    bool PerformOnDie;                             // +0x11
    CTCLifeTimer(CThing* owner);
    virtual void v01();
};

CTCLifeTimer::CTCLifeTimer(CThing* owner)
    : CTCBase(owner), FramesLeft(-1), FadeOutBeforeKill(false), PerformOnDie(true)
{
    m_pThing->SetForceFrameUpdate(true);
}