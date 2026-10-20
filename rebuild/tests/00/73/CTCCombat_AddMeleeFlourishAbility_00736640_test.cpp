// Behaviour test for 00736640: body copied from src/00736640.cpp (CTCCombat::AddMeleeFlourishAbility), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_destroyed = 0;
struct CMeleeCombatAbilityDef { virtual void Destroy() { ++g_destroyed; } long RefCount; };
struct CDefPointer {
    CMeleeCombatAbilityDef* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other) { CMeleeCombatAbilityDef* object = other.Object; Object = object; if (object) ++object->RefCount; }
};
static CMeleeCombatAbilityDef g_def; static bool g_found = true; static int g_getDef = 0; static long g_getDefIndex = 0;
struct CDefinitionManager {
    bool GetDef(long defIndex, CDefPointer* def) { ++g_getDef; g_getDefIndex = defIndex; if (!g_found) return false; def->Object = &g_def; ++g_def.RefCount; return true; }
};
struct __false_type { __false_type() {} };
inline void* operator new(unsigned int, void* where) { return where; }
static int g_overflow = 0;
struct CDefPointerVector {
    CDefPointer* _M_start; CDefPointer* _M_finish; CDefPointer* _M_end_of_storage;
    void _M_insert_overflow(CDefPointer* pos, const CDefPointer& x, const __false_type&, unsigned int fillLen, bool atEnd)
    { ++g_overflow; CHECK(pos == _M_finish && fillLen == 1 && atEnd); new (_M_finish) CDefPointer(x); ++_M_finish; ++_M_end_of_storage; }
    void push_back(const CDefPointer& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                new (_M_finish) CDefPointer(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
static CDefinitionManager g_manager; static int g_update = 0;
struct CTCCombat {
    void* __vftable; unsigned char _pad_0x04[0x28]; CDefPointerVector AbilityList;
    CDefinitionManager* GetDefinitionManager() { return &g_manager; }
    void UpdateUsableMeleeCombatAbilities() { ++g_update; }
    void AddMeleeFlourishAbility(long defIndex);
};
void CTCCombat::AddMeleeFlourishAbility(long defIndex)
{
    CDefPointer def;
    GetDefinitionManager()->GetDef(defIndex, &def);
    CMeleeCombatAbilityDef* object = def.Object;
    AbilityList.push_back(def);
    UpdateUsableMeleeCombatAbilities();
    if (object) {
        if (--object->RefCount == 0)
            object->Destroy();
    }
}
int main()
{
    g_def.RefCount = 1;
    unsigned char storage[sizeof(CDefPointer) * 4]; memset(storage, 0, sizeof storage);
    CTCCombat tc; memset(&tc, 0, sizeof tc);
    tc.AbilityList._M_start = (CDefPointer*)storage; tc.AbilityList._M_finish = tc.AbilityList._M_start; tc.AbilityList._M_end_of_storage = tc.AbilityList._M_start + 1;
    tc.AddMeleeFlourishAbility(42);
    CHECK(g_getDef == 1 && g_getDefIndex == 42 && g_update == 1 && g_overflow == 0);
    CHECK(tc.AbilityList._M_finish - tc.AbilityList._M_start == 1 && tc.AbilityList._M_start[0].Object == &g_def);
    CHECK(g_def.RefCount == 2 && g_destroyed == 0);              // the local released, the vector's copy keeps one
    tc.AddMeleeFlourishAbility(43);                                // vector full: overflow path
    CHECK(g_overflow == 1 && g_update == 2 && g_def.RefCount == 3);
    g_found = false; tc.AddMeleeFlourishAbility(44);               // def not found: null pushed, no release
    CHECK(g_update == 3 && tc.AbilityList._M_start[2].Object == 0 && g_def.RefCount == 3 && g_destroyed == 0);
    g_found = true; g_def.RefCount = 0; tc.AbilityList._M_finish = tc.AbilityList._M_start; tc.AbilityList._M_end_of_storage = tc.AbilityList._M_start;
    g_overflow = 0;
    // a def whose only reference was the lookup: with the vector copy counted it survives; simulate the count reaching zero on release
    tc.AddMeleeFlourishAbility(45);
    CHECK(g_def.RefCount == 1 && g_destroyed == 0 && g_overflow == 1);
    if (fails == 0) printf("00736640_TEST PASS\n"); else printf("00736640_TEST FAIL (%d)\n", fails);
    return fails;
}