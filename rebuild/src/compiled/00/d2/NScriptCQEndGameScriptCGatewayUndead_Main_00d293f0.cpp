// NScript::CQ_EndGameScript::CGatewayUndead::Main() -- keep the script alive only while its own thing lives (SetPredicate(CIsThingAlive(Me))), then idle a script frame at a time until the thread is terminating
// Leaf: ?Main@CGatewayUndead@CQ_EndGameScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D293F0, 104 bytes, thiscall, plain `ret`.
// Identity (proven): slot 1 (Main, the entity-script contract [dtor, Main, Init, ...]) of the entity vtable 0x012CBC74 that the
//   landed CGatewayUndead::Alloc 0x00D29230 stores; FableWin.pdb ?Main@CGatewayUndead@CQ_EndGameScript@NScript@@EAEXXZ (private virtual).
// Callees: CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90 / ~CScriptThing 0x004AA840 (the predicate's member; the
//   CIsThingAlive vtable 0x012C32F0 stored before each), CEntityScriptBase::SetPredicate 0x00F35B10,
//   CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30, CGameScriptInterfaceBase::NewScriptFrame (slot 7, +0x1C).
// Shape (family K067, representative 0x00D293F0): the landed CHTE_Prisoner::Main 0x00D69F50 without the hand-over: the
//   predicate is a scoped 0x10-byte frame object; `NewScriptFrame(); while (!IsActiveThreadTerminating()) NewScriptFrame();`
//   rotates into the test, the aligned loop head and the bottom test.
class CScriptThing {                                    // 12 bytes (Ego_r): vptr, PImp
public:
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    virtual ~CScriptThing();                            // 0x004AA840
    void* PImpData;                                     // +0x04
    void* PImpInfo;                                     // +0x08
};
class CGameScriptInterfaceBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06();
    virtual void NewScriptFrame() const;                // slot 7 (+0x1C)
};
namespace NScript {
class CPredicateBase {
public:
    virtual void v00();
};
class CIsThingAlive : public CPredicateBase {           // vtable 0x012C32F0 (Ego_r 16 bytes)
public:
    CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
    ~CIsThingAlive() { }
    CScriptThing Thing;                                 // +0x04
};
}
class CScriptBase;
class CEntityScriptBase {                               // 20 bytes (Ego_r)
public:
    virtual ~CEntityScriptBase();
private:
    virtual void Main();                                // slot 1
public:
    void SetPredicate(const NScript::CPredicateBase& predicate);   // 0x00F35B10
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
    CGameScriptInterfaceBase* Game;                     // +0x04
    CScriptThing Me;                                    // +0x08
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_EndGameScript {
public:
    class CGatewayUndead : public CEntityScriptBase {
    private:
        virtual void Main();
    public:
        CQ_EndGameScript* ParentClass;                             // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        float RaiseUndeadBarAmount;                     // +0x1C
    };
};
void CQ_EndGameScript::CGatewayUndead::Main()
{
    {
        CIsThingAlive alive(Me);
        SetPredicate(alive);
    }
    Game->NewScriptFrame();
    while (!IsActiveThreadTerminating())
        Game->NewScriptFrame();
}
}