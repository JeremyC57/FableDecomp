// CTCProjectileTrap::CTCProjectileTrap(CThing* thing) -- CTCTrapBase(thing), shot / idle timers -1, no shots this round, own vtable, an empty target intelligent pointer
// Leaf: ??0CTCProjectileTrap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00787DDC, length 54 bytes; size TU (unaligned entry, pass-through
// `push dword ptr [esp+0Ch]`, `or dword ptr [..],-1` / `and dword ptr [..],0` immediates): `#pragma optimize("s", on)`.
// Callees: CTCTrapBase::CTCTrapBase(CThing*) @ 0x0078918D, CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10.
// Vtables 0x0126B90C = CTCProjectileTrap, 0x01238C6C = CIntelligentPointer (rtti-vtables.tsv).
// Layout = Ego_r.pdb CTCProjectileTrap (0x80) less 0xc bytes: CTCTrapBase holds three std::vectors (HitThings, ImmuneThings,
// TriggerThings) that are 12 bytes in retail, so CTCTrapBase is 0x60 and ShotTimer +0x60, IdleTimer +0x64,
// NumShotsThisRound +0x68, PTarget (CIntelligentPointer<CThing>) +0x6c. engine/CTCTrapBase.h follows the PDB offsets, so the
// base is modelled locally as an opaque 0x60-byte polymorphic base.
// Shape: the three scalar initialisers are scheduled BEFORE the derived vtable store (plain init-list members in this
// module), the intelligent pointer after it (out-of-line base constructor + inline vtable store through `edi`).
#pragma optimize("s", on)
class CThing;

struct CTCTrapBase_Calls {                                  // retail 0x60 bytes
    virtual void v00();
    unsigned char _body[0x5c];                              // +0x04 CTCBase, HitThings .. TrapType
    CTCTrapBase_Calls(CThing* thing);                       // 0x0078918D
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {                            // 8 bytes: vtable + item pointer
    virtual void v00();
    void* PData;                                            // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
};
struct CIntelligentPointer : CBaseIntelligentPointer {      // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() { }
};
#pragma pack(pop)

struct CTCProjectileTrap : CTCTrapBase_Calls {              // vtable 0x0126B90C
    long                ShotTimer;                          // +0x60
    long                IdleTimer;                          // +0x64
    long                NumShotsThisRound;                  // +0x68
    CIntelligentPointer PTarget;                            // +0x6c
    virtual void v00();
    CTCProjectileTrap(CThing* thing);
};

CTCProjectileTrap::CTCProjectileTrap(CThing* thing)
    : CTCTrapBase_Calls(thing)
    , ShotTimer(-1)
    , IdleTimer(-1)
    , NumShotsThisRound(0)
    , PTarget()
{
}