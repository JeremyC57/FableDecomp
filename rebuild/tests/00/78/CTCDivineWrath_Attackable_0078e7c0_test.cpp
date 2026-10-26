// CTCDivineWrath::Attackable(CThingCreatureBase* creature) -- a creature may be hit when it is an enemy of the caster, or always while a live z-target is friendly
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078E7C0, length 94 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3); AreThingsEnemies(CThing*, CThing*) @ 0x0076D2F0
// (`__fastcall` bool, x2). Called from CTCDivineWrath::FrameUpdate @ 0x0078FD60 (vtable 0x0126BE84 slot 9).
// Name and signature from ghidra_out/egor_signatures.tsv (`?Attackable@CTCDivineWrath@@AAE_NPAVCThingCreatureBase@@@Z`).
// PDB CTCDivineWrath: PTarget (CIntelligentPointer<CThingCreatureBase>) +0x71, IsZTargetting +0x79; the retail
// CWillButtonHelperTimer at +0x1c is 4 bytes smaller (12-byte vector), so they sit at +0x6d / +0x75 -- the target
// pointer really is unaligned (`lea edi,[esi+6Dh]`), hence pack(1).
// Shape: the inline "thing dead" test (null item or dead bit +0x91) and the z-target flag guard the first
// AreThingsEnemies; its false result jumps to the shared `return true` (a label at the end); every other path
// falls to the second AreThingsEnemies whose true result jumps there too and whose false result is returned as
// is (`return enemies` with the bool named: retail has no `xor al,al`). ParentThing is a named reference before
// each call (`mov ebx,[esi+4]` ahead of the GetPItem call; `mov ecx,[esi+4]` before `mov edx,[creature]`).
// Without the label the first `return true` is laid out inline and the second test becomes `setne`.
class CThingCreatureBase;

class CThing { public:
    unsigned char _pad[0x91];
    unsigned char m_flags91;                                // +0x91 bit 0: Dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
};

class CTCEnemy { public:
    static bool __fastcall AreThingsEnemies(const CThing& a, const CThing& b);   // 0x0076D2F0
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {                            // sizeof 8: vptr + PData
    virtual void v00();
    void* PData;                                            // +0x04 CBaseObjectPointer*
    CThing* GetPItem() const;                               // 0x00A01B50
};

class CTCDivineWrath { public:
    virtual void v00();
    CThing*                 ParentThing;                    // +0x04 (CTCBase)
    unsigned char           _body[0x6d - 0x08];             // ToBeRemoved .. CutSceneCancel (+0x6c)
    CBaseIntelligentPointer PTarget;                        // +0x6d CIntelligentPointer<CThingCreatureBase>
    bool                    IsZTargetting;                  // +0x75
    bool Attackable(CThingCreatureBase* creature);
};
#pragma pack(pop)

bool CTCDivineWrath::Attackable(CThingCreatureBase* creature)
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
// ---- harness ----
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_calls = 0; static const CThing* g_firstA = 0;
bool __fastcall CTCEnemy::AreThingsEnemies(const CThing& a, const CThing& b)
{
    if (g_calls++ == 0) g_firstA = &a;
    return a._pad[0] != b._pad[0];             // different teams are enemies
}
void CBaseIntelligentPointer::v00() { }
CThing* CBaseIntelligentPointer::GetPItem() const { return (CThing*)PData; }
void CTCDivineWrath::v00() { }
static CThing* MakeThing(unsigned char team, bool dead)
{
    static CThing pool[8]; static int n = 0;
    CThing* t = &pool[n++]; memset(t, 0, sizeof *t); t->_pad[0] = team; t->m_flags91 = dead ? 1 : 0;
    return t;
}
static bool Run(CTCDivineWrath& c, CThing* creature) { g_calls = 0; g_firstA = 0; return c.Attackable((CThingCreatureBase*)creature); }
int main()
{
    CThing* hero = MakeThing(1, false);
    CThing* ally = MakeThing(1, false);
    CThing* foe = MakeThing(2, false);
    CThing* deadAlly = MakeThing(1, true);
    CTCDivineWrath c; memset(((char*)&c) + 4, 0, sizeof c - 4);
    c.ParentThing = hero;
    CHECK(Run(c, foe) == true && g_calls == 1 && g_firstA == hero);       // no z-target: enemy test only
    CHECK(Run(c, ally) == false && g_calls == 1);
    c.PTarget.PData = ally; c.IsZTargetting = true;                        // live friendly z-target: always attackable
    CHECK(Run(c, ally) == true && g_calls == 1 && g_firstA == hero);
    c.PTarget.PData = foe;                                                 // enemy z-target: falls to the enemy test
    CHECK(Run(c, ally) == false && g_calls == 2);
    CHECK(Run(c, foe) == true && g_calls == 2);
    c.PTarget.PData = deadAlly;                                            // dead z-target is ignored
    CHECK(Run(c, ally) == false && g_calls == 1);
    c.PTarget.PData = ally; c.IsZTargetting = false;                       // not z-targetting
    CHECK(Run(c, ally) == false && g_calls == 1);
    if (fails == 0) printf("0078e7c0_TEST PASS\n");
    return fails;
}