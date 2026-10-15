// CTCTrapBase::OnKill() -- base OnKill, then kills the trap's obstruction thing if it is still alive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00787D94, length 40 bytes.
// Callees: CTCBase::OnKill @ 0x006868F0 (the base override called directly; manifest "$E2" is a propagated label),
// IS_THING_ALIVE<CIntelligentPointer<CThingPhysical const>> @ 0x004CC340 (`__fastcall`, the pointer in ecx),
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill(bool) @ 0x004C9B80.
// Shape: `add esi,24h` turns `this` into &PObstruction once (this is dead after the base call); `push 1` is evaluated
// before the object expression `PObstruction.GetPItem()`.
// Layout: CTCTrapBase.h is the PDB layout (PObstruction +0x2C); the retail constructor 0x0078918D puts the
// CIntelligentPointer at +0x24 (12-byte STLport vectors), so a local model is used.
struct CThing {
    void Kill(bool checkLocked);            // 0x004C9B80
};

struct CBaseIntelligentPointer {
    void* __vftable;                        // +0x00
    void* PData;                            // +0x04
    CThing* GetPItem() const;               // 0x00A01B50
};

struct CIntelligentPointer : CBaseIntelligentPointer {
};

bool __fastcall IS_THING_ALIVE(const CIntelligentPointer* pointer);   // 0x004CC340

struct CTCBase {
    virtual void v00();
    unsigned char _pad_0x04[0x8];           // +0x04 owner thing, +0x08
    virtual void OnKill();                  // 0x006868F0
};

struct CTCTrapBase : CTCBase {
    unsigned char       _vectors[0x18];     // +0x0C HitThings, ImmuneThings (12-byte vectors)
    CIntelligentPointer PObstruction;       // +0x24
    virtual void OnKill();
};

void CTCTrapBase::OnKill()
{
    CTCBase::OnKill();
    if (IS_THING_ALIVE(&PObstruction))
        PObstruction.GetPItem()->Kill(true);
}
