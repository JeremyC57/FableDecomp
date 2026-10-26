// CNOVI_CreatedBeetle::Main() -- the beetle spawned in Oakvale's intro: register an is-alive functor over Me, run a frame, then give it five seconds on a script timer and remove it (with effects) unless the script finished first
// Grade: RELOCATION_MATCH -- the closing test is written `if (IsActiveThreadTerminating()) return; Game->RemoveThing(...)`: an early return there keeps a second CTimer-dtor + epilogue copy like retail, where `if (!IsActiveThreadTerminating()) RemoveThing` let VC cross-jump the loop exit into the final DeregisterTimer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DB80C0, 232 bytes (+ int3 padding). __fastcall, `ret`.
// Callees: CScriptThing copy ctor 0x004ABE90 / dtor 0x004AA840 (the functor's member; the functor's inline dtor re-stores its
//   vtable 0x012C2FE8 first), the entity script's functor registration 0x00F35B10 and IsDone 0x00F35B30 (on `this`; manifest
//   labels "CWorldMap::AllocateMapThingUID" / "CCreatureAction_TrollWhackGroundBase::Initialise" are propagated, see landed
//   CHTE_Prisoner::Main 0x00D69F50); the script's own interface (+4) NewScriptFrame 7 (+0x1c) 0x006E7410 and RemoveThing 108
//   (+0x1b0) 0x008910D0; the global g_pGameScriptInterface (0x0143E8F8) RegisterTimer 87 (+0x15c) 0x006E79E0, DeregisterTimer
//   88 (+0x160) 0x006E7690, SetTimer 89 (+0x164) 0x006E76E0, GetTimer 90 (+0x168) 0x006E7710.
// Layout: Ego_r.pdb CNOVI_CreatedBeetle (28 bytes) = CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14,
//   PMasterData +0x18.
// Shape: the functor is a scoped 0x10-byte frame object; the timer index stays in edi; the early exit inside the wait loop
//   deregisters the timer itself (its own copy of the epilogue), the normal path falls through to the same call.
namespace NScript { class CPredicateBase; }
class CScriptThing { public:                                  // 12 bytes
    unsigned char _body[12 - 4];
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    virtual ~CScriptThing();                                   // 0x004AA840
};
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
struct CScriptThingFunctor_00ef7ab0 : public NScript::CPredicateBase {                  // vtable 0x012C2FE8: an is-alive functor holding a script thing
    CScriptThing m_thing;                              // +0x04
    CScriptThingFunctor_00ef7ab0(const CScriptThing& thing) : m_thing(thing) { }
    ~CScriptThingFunctor_00ef7ab0() { }
};
class CGameScriptInterface { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual long RegisterTimer();   // slot 87 (+0x15c)
    virtual void DeregisterTimer(long timer);   // slot 88 (+0x160)
    virtual void SetTimer(long timer, long time);   // slot 89 (+0x164)
    virtual long GetTimer(long timer) const;   // slot 90 (+0x168)
    virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098();
    virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106();
    virtual void v107();
    virtual void RemoveThing(const CScriptThing& thing, bool withEffects, bool b) const;   // slot 108 (+0x1b0)
};
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
class CTimer { public:                                        // 4 bytes: an index into the global script timers
    long TimerIndex;
    CTimer(long time) { TimerIndex = g_pGameScriptInterface->RegisterTimer(); g_pGameScriptInterface->SetTimer(TimerIndex, time); }
    ~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
    long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
};
struct CQ_NewOakValeIntroScript;
struct CQ_SunnyvaleMasterData;
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

struct CNOVI_CreatedBeetle : public CEntityScriptBase {
    void*                     __vftable;               // +0x00
    CGameScriptInterface*     Game;                    // +0x04
    CScriptThing              Me;                      // +0x08
    CQ_NewOakValeIntroScript* ParentClass;             // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;             // +0x18
    void Main();
};

void CNOVI_CreatedBeetle::Main()
{
    {
        CScriptThingFunctor_00ef7ab0 functor(Me);
        SetPredicate(functor);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CTimer timer(5);
    while (timer.GetTime() != 0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveThing(Me, true, true);
}