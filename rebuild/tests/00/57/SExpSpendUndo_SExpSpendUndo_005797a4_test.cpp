// SExpSpendUndo::SExpSpendUndo() 0x005797A4 -- behaviour test: four lists constructed in member order, Levels and the eight
// stat words zeroed (the list model counts its constructions; the stat words start as garbage).
#include <cstdio>
#include <cstring>
#include <new>
static long g_lists = 0;
struct SAllocator { SAllocator() { } };
struct CList { void* m_pNode; explicit CList(const SAllocator& = SAllocator()) { m_pNode = (void*)(size_t)++g_lists; } };
struct SExpSpendUndo {
    CList Generals, Strengths, Skills, Wills;
    long Levels, General, Strength, Skill, Will, aGeneral, aStrength, aSkill, aWill;
    SExpSpendUndo() : Levels(0)
    {
        General = Strength = Skill = Will = 0;
        aGeneral = aStrength = aSkill = aWill = 0;
    }
};

int main() {
    unsigned char raw[sizeof(SExpSpendUndo)];
    std::memset(raw, 0x5a, sizeof raw);
    SExpSpendUndo* d = new (raw) SExpSpendUndo;
    bool ok = g_lists == 4 && d->Generals.m_pNode == (void*)1 && d->Wills.m_pNode == (void*)4 && d->Levels == 0
        && d->General == 0 && d->Strength == 0 && d->Skill == 0 && d->Will == 0
        && d->aGeneral == 0 && d->aStrength == 0 && d->aSkill == 0 && d->aWill == 0;
    std::printf(ok ? "005797a4_TEST PASS\n" : "005797a4_TEST FAIL\n");
    return ok ? 0 : 1;
}
