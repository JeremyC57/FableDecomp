// CVtable_012e84a0::Main() -- a script action body: pump the game interface until the script thread is being terminated
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E6A7A0, length 37 bytes.
// Callees: IsFinished @ 0x00F35B30 (`this->GetScriptBase()` through vtable slot 3, then
// CScriptBase::IsActiveThreadTerminating @ 0x00CB7940; named as in the landed CAT_BanditContactEnd::Main
// 0x00F05780, which shares the interface-at-+4 layout and the same callee), game-interface vtable
// slot 7 (+0x1c, Pump) through the pointer at +4.
// The class is a script-module action whose vtable (0x012E84A0, no RTTI in that module: slot 0 the
// deleting destructor, slot 1 this Main) names it; the manifest's "CCOE_AltarEvil::Main" label is
// propagated over the byte-identical bodies of the whole family. The loop is a plain `while
// (!IsFinished())` -- rotated by the compiler into test + do/while with the 4-byte `lea esp,[esp]`
// alignment pad before the loop head.
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

struct CVtable_012e84a0 {
    virtual void v00();
    CGameScriptInterface_Calls* m_pInterface;               // +0x04
    unsigned char IsFinished();                             // 0x00F35B30
    void Main();
};

void CVtable_012e84a0::Main()
{
    while (!IsFinished())
        m_pInterface->Pump();
}