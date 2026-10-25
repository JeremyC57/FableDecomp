// Behaviour test for SExpSpendUndo::~SExpSpendUndo (0x005797FA): the four list<long> members are destroyed, last first.
// The source is copied verbatim; std::list<long>::~list is a recording stub.
#include <stdio.h>
#include <new>
static int g_order[8]; static int g_count = 0; static const void* g_base = 0;
// ---- source (verbatim) ----
// SExpSpendUndo::~SExpSpendUndo() -- the experience-spend undo record's destructor: its four list<long> histories (Wills, Skills, Strengths, Generals) destroyed in reverse order, the first by tail call
// Leaf: ??1SExpSpendUndo@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005797FA, 35 bytes.
// Identity: Ego_r.pdb ??1SExpSpendUndo@@QAE@XZ; struct_layouts_egor SExpSpendUndo = list<long> Generals / Strengths / Skills /
//   Wills (+0/+12/+24/+36 with Dinkumware's 12-byte list; STLport's list is one sentinel pointer, so +0/+4/+8/+0xc here)
//   followed by nine longs (Levels, General .. aWill). Callers: CTCInventoryExperience::BuyAbility 0x005785B3 / BuyStat
//   0x005786B5 (their SExpSpendUndo locals), the pair<const EHeroTrainableStatType / EHeroAbility, SExpSpendUndo>
//   scalar deleting destructors 0x005799DE / 0x005799FD and the four `add ecx,4 ; jmp` pair destructors after this body.
//   Each list member's destructor is std::list<long>::~list 0x0042AB73. The earlier generated CAggregate_005797fa name
//   (one of 13 bodies of this shape) and the manifest's "_Dest_val<...CMacro>" were propagated.
namespace std {
template <class _Tp> class allocator;
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    void* _M_node;                              // +0x00 the sentinel node (STLport _List_base)
    ~list();                                    // 0x0042AB73 for long
};
}

struct SExpSpendUndo {
    std::list<long> Generals;                   // +0x00
    std::list<long> Strengths;                  // +0x04
    std::list<long> Skills;                     // +0x08
    std::list<long> Wills;                      // +0x0c
    long Levels;                                // +0x10
    long General;                               // +0x14
    long Strength;                              // +0x18
    long Skill;                                 // +0x1c
    long Will;                                  // +0x20
    long aGeneral;                              // +0x24
    long aStrength;                             // +0x28
    long aSkill;                                // +0x2c
    long aWill;                                 // +0x30
    ~SExpSpendUndo();
};

SExpSpendUndo::~SExpSpendUndo()
{
}

// ---- stubs ----
template <> std::list<long>::~list() { if (g_count < 8) g_order[g_count] = (int)((const char*)this - (const char*)g_base); ++g_count; }
int main()
{
    static unsigned char raw[sizeof(SExpSpendUndo)];
    SExpSpendUndo* u = new (raw) SExpSpendUndo;
    g_base = u;
    u->~SExpSpendUndo();
    int ok = sizeof(SExpSpendUndo) == 0x34 && g_count == 4 && g_order[0] == 0xc && g_order[1] == 8 && g_order[2] == 4 && g_order[3] == 0;
    printf(ok ? "005797fa_TEST PASS\n" : "005797fa_TEST FAIL\n");
    return ok ? 0 : 1;
}
