// NScript::CV_MurderTwistFinalScript::CMurderTwistAssassinGuard::Main() -- the final murder-twist assassin's henchman: alive filter on Me, take scripted control of Me through MyEntityHandle, then by the parent's AssassinState ask the hero to talk to the assassin (states 0 and 1), thank him (state 3) or attack him (state 2), then idle until the thread ends
// Grade: DIFFER(362v370) -- retail keeps two dead values this form cannot reproduce: a dword 0 stored at entry into an extra 4-byte frame slot below the CIsThingAlive temporary (sub esp,0x14 v 0x10; the shape of a conditional-temporary flag that nothing tests) and the DoesAssassinTrustHero result copied to bl (`mov bl,al`) but never read; with them the Me / MyEntityHandle registers swap (retail edi/ebx, ours ebx/edi) and the loop-alignment `mov edi,edi` moves. The near-identical retail twin 0x00EB3F40 (another entity Main of CV_MurderTwistFinalScript, not landed) has the same two dead values, so they come from the shared source form (a macro?). Tried (compile-only probes): braced named predicate, identical if/else arms on the bool, (b || !b) guards, a ternary loop condition, a duplicated loop per bool value, unreachable code after the idle loop, `false &&` guards with a CCharString temporary, the QFE 4035 compiler.
// Leaf: ?Main@CMurderTwistAssassinGuard@CV_MurderTwistFinalScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB55D0, 370 bytes, thiscall, plain `ret`.
// Identity (proven): slot 1 (Main) of the entity-script vtable 0x012F036C that the landed CMurderTwistAssassinGuard::Alloc
//   0x00EB5450 stores [dtor 0x00EB6340, Main 0x00EB55D0, Init 0x00EB5500 (landed CMurderTwistAssassinGuard::Init),
//   0x00EB54F0, OnPersist 0x00EB69D0 (HasBeenSpokenTo key)]; the row is referenced only from that slot. Ego_r.pdb
//   ?Main@CMurderTwistAssassinGuard@CV_MurderTwistFinalScript@NScript@@EAEXXZ. The three member calls are the class's three
//   private methods (Ego_r.pdb DoAskingHeroToTalkToAssassin / DoSayingThankYouToHero / AttackHero, AAEXXZ): each has this site
//   as its only caller; 0x00EB5D50 is the landed DoSayingThankYouToHero (it sets AssassinState 2), and 0x00EB6200 (landed under
//   the propagated label CMurderTwistSheriff::Init) gives Me the hero as best enemy target and makes Me killable and
//   damageable: AttackHero. Catalog label: sub_00eb55d0.
// Callees: CScriptThing copy 0x004ABE90 (the predicate's member), ~CIsThingAlive 0x00E241B0 (vtable 0x012C32F0),
//   CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30, the handle's IsValid 0x00CD23B9 /
//   Release 0x00CD2770, CV_MurderTwistFinalScript::DoesAssassinTrustHero 0x00EB03A0 (landed), DoAskingHeroToTalkToAssassin
//   0x00EB5750, DoSayingThankYouToHero 0x00EB5D50, AttackHero 0x00EB6200; CGameScriptInterface slots 7 NewScriptFrame, 8
//   StartScriptingEntity (ghidra_out/cgsi_slots_aligned.tsv).
// Layout: Ego_r.pdb CMurderTwistAssassinGuard (0x34, same in retail): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass
//   +0x14, PMasterData +0x18, MyEntityHandle +0x1c, NextLine +0x2c, HasBeenSpokenTo +0x30, PreemptiveAttackInProgress +0x31.
//   CV_MurderTwistFinalScript (retail = Ego_r.pdb - 0x14): SheriffState +0x48, AssassinState +0x4c.
// Shape: the landed entity Mains (CGUARD1::Main 0x00D6A220 predicate scope, rotated StartScriptingEntity wait); the loops
//   compare AssassinState with the plain enum values (enumerator names not recovered).
class CCharString;
class CCPPointerInfo;
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
};
class CBaseClass { public:                              // vtable 0x01231710
    virtual ~CBaseClass();                              // 0x0099A2E0
};
class CScriptThing : public CBaseClass { public:        // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThing> PImp;                 // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    virtual ~CScriptThing();                            // 0x004AA840
};
class CBaseObject { public:                             // vtable 0x0129A7D8
    virtual ~CBaseObject();
    void* m_pIntelligentPointer;                        // +0x04
};
class CScriptGameResourceObjectBase : public CBaseObject { public:   // vtable 0x0126008C
};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;   // +0x08
    bool IsValid() const;                               // 0x00CD23B9
    void Release();                                     // 0x00CD2770
};
enum EScriptAIPriority { };
class CGameScriptInterfaceBase { public:          // slots: ghidra_out/cgsi_slots_aligned.tsv (CGameScriptInterface vtable 0x01260F0C)
    virtual void g000(); virtual void g001(); virtual void g002(); virtual void g003(); virtual void g004(); virtual void g005(); virtual void g006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& handle, EScriptAIPriority priority);   // slot 8 (+0x20)
};
namespace NScript {
class CPredicateBase {
public:
};
class CIsThingAlive : public CPredicateBase {           // vtable 0x012C32F0 (Ego_r 16 bytes)
public:
    CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
    virtual ~CIsThingAlive();                           // 0x00E241B0
    CScriptThing Thing;                                 // +0x04
};
}
class CEntityScriptBase {                               // 20 bytes (Ego_r)
public:
    virtual ~CEntityScriptBase();
    void SetPredicate(const NScript::CPredicateBase& predicate);   // 0x00F35B10
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
    CGameScriptInterfaceBase* Game;                     // +0x04
    CScriptThing Me;                                    // +0x08
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_MurderTwistFinalScript {
public:
    enum ESheriffState { };
    enum EAssassinState { };
    unsigned char  _pad[0x48];                          // CLevelScriptBase part (Game +0x40, PMasterData +0x44)
    ESheriffState  SheriffState;                        // +0x48
    EAssassinState AssassinState;                       // +0x4c
    bool DoesAssassinTrustHero();                       // 0x00EB03A0
    class CMurderTwistAssassinGuard;
};
class CV_MurderTwistFinalScript::CMurderTwistAssassinGuard : public CEntityScriptBase {
public:
    CV_MurderTwistFinalScript* ParentClass;             // +0x14
    CQ_SunnyvaleMasterData*    PMasterData;             // +0x18
    CScriptGameResourceObjectScriptedThingBase MyEntityHandle;   // +0x1c
    unsigned long              NextLine;                // +0x2c
    bool                       HasBeenSpokenTo;         // +0x30
    bool                       PreemptiveAttackInProgress;   // +0x31
private:
    void AttackHero();                                  // 0x00EB6200
    void DoAskingHeroToTalkToAssassin();                // 0x00EB5750
    void DoSayingThankYouToHero();                      // 0x00EB5D50
    virtual void Main();                                // slot 1 (+0x04)
};

void CV_MurderTwistFinalScript::CMurderTwistAssassinGuard::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (MyEntityHandle.IsValid())
        MyEntityHandle.Release();
    while (!Game->StartScriptingEntity(Me, MyEntityHandle, EScriptAIPriority(4))) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    bool trustsHero = ParentClass->DoesAssassinTrustHero();
    while (ParentClass->AssassinState == 0 || ParentClass->AssassinState == 1) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (IsActiveThreadTerminating())
            return;
        DoAskingHeroToTalkToAssassin();
    }
    if (IsActiveThreadTerminating())
        return;
    while (ParentClass->AssassinState == 3) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        DoSayingThankYouToHero();
    }
    if (IsActiveThreadTerminating())
        return;
    if (ParentClass->AssassinState == 2) {
        if (IsActiveThreadTerminating())
            return;
        AttackHero();
    }
    do
        Game->NewScriptFrame();
    while (!IsActiveThreadTerminating());
}
}   // namespace NScript
