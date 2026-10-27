// CTCMeleeTrap::CTCMeleeTrap(CThing&) -- the CTCTrapBase base, this class's vtable, then PlayingContinuous = false, StrikeAnimation = false
// Leaf: ??0CTCMeleeTrap@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00787DBC, 32 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): stores vtable 0x0126B884 (RTTI .?AVCTCMeleeTrap@@ : CTCTrapBase ...); FableWin.pdb ??0CTCMeleeTrap@@QAE@AAVCThing@@@Z; only caller
//   CTCMeleeTrap::Alloc 0x004D4987 (landed); the stores are Ego_r.pdb's CTCMeleeTrap members (retail = Ego_r.pdb - 0xC).
// Callees: CTCTrapBase::CTCTrapBase(CThing&) 0x0078918D.
// Shape (family K091, representative 0x0044C4D5): the argument forwarded to the base, the vtable store, the two member
//   initialisers in declaration order; size TU (the pass-through `push dword ptr [esp+8]`).
#pragma optimize("s", on)
class CThing;
class CTCTrapBase {                                            // retail 0x60 bytes (members not touched here)
public:
    CTCTrapBase(CThing& arg);                                   // 0x0078918D
    virtual ~CTCTrapBase();
    unsigned char _body[0x60 - 4];
};
class CTCMeleeTrap : public CTCTrapBase {
public:
    CTCMeleeTrap(CThing& arg);
    bool      PlayingContinuous;                   // +0x60
    bool      StrikeAnimation;                     // +0x61
};
CTCMeleeTrap::CTCMeleeTrap(CThing& arg)
    : CTCTrapBase(arg), PlayingContinuous(false), StrikeAnimation(false)
{
}