#include <cstdio>
#include <string.h>
// Behaviour test for CCombatActionBase::SetCombatAnimation (retail 0x008574D0): the vector is
// emptied (capacity kept) before the creature appends the animations for the name.
struct CCharString { const char* text; };
struct CAnimationEntry { int id; };
struct CAnimationEntryVector;

static const CAnimationEntry g_entries[3] = { { 1 }, { 2 }, { 3 } };
static int g_queries = 0;
static const CCharString* g_lastName = 0;
static unsigned long g_sizeSeen = 99;

struct CAnimationEntryVector {
    const CAnimationEntry** m_pFirst;
    const CAnimationEntry** m_pLast;
    const CAnimationEntry** m_pEnd;

    static const CAnimationEntry** CopyDown(const CAnimationEntry** src, const CAnimationEntry** srcEnd, const CAnimationEntry** dest)
    {
        if (src == srcEnd)
            return dest;
        unsigned long bytes = (char*)srcEnd - (char*)src;
        return (const CAnimationEntry**)((char*)memmove(dest, src, bytes) + bytes);
    }
    void Erase(const CAnimationEntry** eraseFirst, const CAnimationEntry** eraseLast)
    {
        m_pLast = CopyDown(eraseLast, m_pLast, eraseFirst);
    }
    void clear() { Erase(m_pFirst, m_pLast); }
};

struct CThingCreatureBase {
    bool GetPossibleCombatAnimations(const CCharString& name, CAnimationEntryVector& anims) const
    {
        ++g_queries; g_lastName = &name; g_sizeSeen = (unsigned long)(anims.m_pLast - anims.m_pFirst);
        *anims.m_pLast++ = &g_entries[2];
        return true;
    }
};

struct CCreaturePointer {
    CThingCreatureBase* m_pItem;
    CThingCreatureBase* GetPItem() const { return m_pItem; }
    CThingCreatureBase* operator->() const { return GetPItem(); }
};

struct CCombatActionBase {
    CCreaturePointer      PCreature;
    CAnimationEntryVector CombatAnimations;
    void SetCombatAnimation(const CCharString& name);
};

void CCombatActionBase::SetCombatAnimation(const CCharString& name)
{
    CombatAnimations.clear();
    PCreature->GetPossibleCombatAnimations(name, CombatAnimations);
}

int main()
{
    const CAnimationEntry* storage[8];
    storage[0] = &g_entries[0]; storage[1] = &g_entries[1];
    CThingCreatureBase creature;
    CCombatActionBase action;
    action.PCreature.m_pItem = &creature;
    action.CombatAnimations.m_pFirst = storage;
    action.CombatAnimations.m_pLast = storage + 2;
    action.CombatAnimations.m_pEnd = storage + 8;
    CCharString name; name.text = "Strike";
    action.SetCombatAnimation(name);
    if (g_queries != 1 || g_lastName != &name || g_sizeSeen != 0
        || action.CombatAnimations.m_pFirst != storage || action.CombatAnimations.m_pLast != storage + 1
        || action.CombatAnimations.m_pEnd != storage + 8 || storage[0] != &g_entries[2]) {
        std::printf("008574d0_TEST FAIL\n"); return 1;
    }
    std::printf("008574d0_TEST PASS\n");
    return 0;
}