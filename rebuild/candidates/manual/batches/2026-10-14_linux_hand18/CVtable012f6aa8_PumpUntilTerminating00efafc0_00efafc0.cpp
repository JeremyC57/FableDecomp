// CVtable_012f6aa8::PumpUntilTerminating_00efafc0() -- a script body: pump the game interface (at +0x40) until this script's active thread is being terminated
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EFAFC0, length 37 bytes.
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (called directly on `this`: this class
// derives from CScriptBase, whose +0x2c thread pointer the callee reads), game-interface vtable slot 7
// (+0x1c, Pump) through the pointer at +0x40.
// The class is a script-module CScriptBase-derived script whose vtable (0x012F6AA8, no RTTI in that
// module: slot 0 the deleting destructor, slot 1 "CGlobal_WatchForHeroDeathScript::Main" @ 0x00EFAFF0,
// slot 2 this body) names it; the manifest's "CCOE_AltarEvil::Main" label is propagated from a
// byte-alike. Same shape as 0x00E6A7A0 with a non-virtual predicate: `while (!IsActiveThreadTerminating())`
// rotated into test + do/while with the `lea esp,[esp]` alignment pad.
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

struct CScriptBase_Retail {
    virtual void v00();
    unsigned char _pad[0x3c];                               // +0x04 .. +0x40 (the active thread is at +0x2c)
    unsigned char IsActiveThreadTerminating();              // 0x00CB7940
};

struct CVtable_012f6aa8 : CScriptBase_Retail {
    CGameScriptInterface_Calls* m_pInterface;               // +0x40
    void PumpUntilTerminating_00efafc0();
};

void CVtable_012f6aa8::PumpUntilTerminating_00efafc0()
{
    while (!IsActiveThreadTerminating())
        m_pInterface->Pump();
}
