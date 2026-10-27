// CTCActivationReceptorCreatureGenerator::CTCActivationReceptorCreatureGenerator(CThing&) -- the CTCActivationReceptorBase base, this class's vtable, then TriggerOnActivate = true, ActivateOnActivate = false
// Leaf: ??0CTCActivationReceptorCreatureGenerator@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DE623, 32 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): stores vtable 0x012413EC (RTTI .?AVCTCActivationReceptorCreatureGenerator@@ : CTCActivationReceptorBase ...); FableWin.pdb ??0CTCActivationReceptorCreatureGenerator@@QAE@AAVCThing@@@Z; only caller
//   CTCActivationReceptorCreatureGenerator::Alloc 0x004E0827 (landed); the stores are Ego_r.pdb's CTCActivationReceptorCreatureGenerator members (retail = Ego_r.pdb - 4).
// Callees: CTCActivationReceptorBase::CTCActivationReceptorBase(CThing&) 0x007F7710.
// Shape (family K091, representative 0x0044C4D5): the argument forwarded to the base, the vtable store, the two member
//   initialisers in declaration order; size TU (the pass-through `push dword ptr [esp+8]`).
#pragma optimize("s", on)
class CThing;
class CTCActivationReceptorBase {                                            // retail 0x2C bytes (members not touched here)
public:
    CTCActivationReceptorBase(CThing& arg);                                   // 0x007F7710
    virtual ~CTCActivationReceptorBase();
    unsigned char _body[0x2C - 4];
};
class CTCActivationReceptorCreatureGenerator : public CTCActivationReceptorBase {
public:
    CTCActivationReceptorCreatureGenerator(CThing& arg);
    bool      TriggerOnActivate;                   // +0x2C
    bool      ActivateOnActivate;                  // +0x2D
};
CTCActivationReceptorCreatureGenerator::CTCActivationReceptorCreatureGenerator(CThing& arg)
    : CTCActivationReceptorBase(arg), TriggerOnActivate(true), ActivateOnActivate(false)
{
}