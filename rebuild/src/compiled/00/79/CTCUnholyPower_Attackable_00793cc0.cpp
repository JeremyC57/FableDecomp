// CTCUnholyPower::Attackable(CThingCreatureBase* creature) -- a creature may be hit when it is an enemy of the caster, or always while a live z-target is friendly
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00793CC0, length 94 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3); AreThingsEnemies(CThing*, CThing*) @ 0x0076D2F0
// (`__fastcall` bool, x2). Called from CTCUnholyPower::FrameUpdate @ 0x00795300 (vtable 0x0126BFDC slot 9); the chunk label "CTCDivineWrath" is propagated from the byte-identical 0x0078E7C0.
// Name and signature from ghidra_out/egor_signatures.tsv (`?Attackable@CTCUnholyPower@@AAE_NPAVCThingCreatureBase@@@Z`).
// PDB CTCUnholyPower (same layout up to +0x7a): PTarget (CIntelligentPointer<CThingCreatureBase>) +0x71, IsZTargetting +0x79; the retail
// CWillButtonHelperTimer at +0x1c is 4 bytes smaller (12-byte vector), so they sit at +0x6d / +0x75 -- the target
// pointer really is unaligned (`lea edi,[esi+6Dh]`), hence pack(1).
// Shape: the inline "thing dead" test (null item or dead bit +0x91) and the z-target flag guard the first
// AreThingsEnemies; its false result jumps to the shared `return true` (a label at the end); every other path
// falls to the second AreThingsEnemies whose true result jumps there too and whose false result is returned as
// is (`return enemies` with the bool named: retail has no `xor al,al`). ParentThing is a named reference before
// each call (`mov ebx,[esi+4]` ahead of the GetPItem call; `mov ecx,[esi+4]` before `mov edx,[creature]`).
// Without the label the first `return true` is laid out inline and the second test becomes `setne`.
struct CThingCreatureBase;

struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_flags91;                                // +0x91 bit 0: Dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
};

struct CTCEnemy {
    static bool __fastcall AreThingsEnemies(const CThing& a, const CThing& b);   // 0x0076D2F0
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {                            // sizeof 8: vptr + PData
    virtual void v00();
    void* PData;                                            // +0x04 CBaseObjectPointer*
    CThing* GetPItem() const;                               // 0x00A01B50
};

struct CTCUnholyPower {
    virtual void v00();
    CThing*                 ParentThing;                    // +0x04 (CTCBase)
    unsigned char           _body[0x6d - 0x08];             // ToBeRemoved .. CutSceneCancel (+0x6c)
    CBaseIntelligentPointer PTarget;                        // +0x6d CIntelligentPointer<CThingCreatureBase>
    bool                    IsZTargetting;                  // +0x75
    bool Attackable(CThingCreatureBase* creature);
};
#pragma pack(pop)

bool CTCUnholyPower::Attackable(CThingCreatureBase* creature)
{
    if (PTarget.GetPItem() != 0 && !PTarget.GetPItem()->IsDead() && IsZTargetting) {
        CThing& parent = *ParentThing;
        if (!CTCEnemy::AreThingsEnemies(parent, *PTarget.GetPItem()))
            goto attackable;
    }
    {
        CThing& parent = *ParentThing;
        bool enemies = CTCEnemy::AreThingsEnemies(parent, *reinterpret_cast<CThing*>(creature));
        if (!enemies)
            return enemies;
    }
attackable:
    return true;
}