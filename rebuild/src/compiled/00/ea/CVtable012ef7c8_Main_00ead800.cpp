// CVtable_012ef7c8::Main() -- a script action body: register an "is thing alive" wait condition on the action's thing, pump the interface once, then keep pumping until the script thread is being terminated
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EAD800, length 91 bytes.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90 (the condition's thing copy,
// counted-pointer addref), WaitFor @ 0x00F35B10 (`this->GetScriptBase()` via vtable slot 3, then
// 0x00CB7920 with the condition; manifest auto-name "CWorldMap::AllocateMapThingUID" is wrong),
// CIsThingAlive::~CIsThingAlive @ 0x00E241B0, IsFinished @ 0x00F35B30 (GetScriptBase()->
// IsActiveThreadTerminating, named as in the landed CAT_BanditContactEnd::Main), game-interface vtable
// slot 7 (+0x1c, Pump) through the pointer at +4.
// CIsThingAlive (PDB, 16 bytes) = {vptr 0x012C32F0, CScriptThing Thing +4}; it is a stack temporary
// built inline (vtable store + out-of-line CScriptThing copy), passed by address and destroyed out
// of line once the wait is registered. The action's own CScriptThing is at +8. The vtable
// 0x012EF7C8 (script module, no RTTI; slot 1 = this Main) names the class; the manifest's
// "CCOE_AltarEvil::Main" label is propagated. The first Pump is unconditional; the loop after it is a
// rotated `while (!IsFinished())`.
struct CGameScriptInterface_Calls {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void Pump();                                    // slot 7 (+0x1c)
};

struct CScriptThing_Calls {
    void* __vftable;
    void* PImp_Data;
    void* PImp_Count;
    CScriptThing_Calls(const CScriptThing_Calls& other);    // 0x004ABE90
};

struct CIsThingAlive {                                      // vtable 0x012C32F0
    virtual void v00();
    CScriptThing_Calls Thing;                               // +4
    CIsThingAlive(const CScriptThing_Calls& thing) : Thing(thing) { }
    ~CIsThingAlive();                                       // 0x00E241B0
};

struct CVtable_012ef7c8 {
    virtual void v00();
    CGameScriptInterface_Calls* m_pInterface;               // +0x04
    CScriptThing_Calls          m_thing;                    // +0x08
    void WaitFor(CIsThingAlive* condition);                 // 0x00F35B10
    unsigned char IsFinished();                             // 0x00F35B30
    void Main();
};

void CVtable_012ef7c8::Main()
{
    {
        CIsThingAlive condition(m_thing);
        WaitFor(&condition);
    }
    m_pInterface->Pump();
    while (!IsFinished())
        m_pInterface->Pump();
}