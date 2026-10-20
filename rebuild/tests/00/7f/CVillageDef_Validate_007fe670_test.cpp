// Behaviour test for 007fe670: body copied from src/007fe670.cpp (CVillageDef::Validate), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CValidateDefinitionInitBase {};
struct CCharString {};
enum EWeaponClass { WEAPON_CLASS_NONE = 0, WEAPON_CLASS_1 = 1 };
struct random_access_iterator_tag {
    random_access_iterator_tag() { }
};
static int g_findCalls = 0; static EWeaponClass g_findValue = WEAPON_CLASS_NONE;
const EWeaponClass* __fastcall find_weapon_class(const EWeaponClass* first, const EWeaponClass* last, const EWeaponClass& value, const random_access_iterator_tag& tag)
{
    ++g_findCalls; g_findValue = value;
    for (const EWeaponClass* it = first; it != last; ++it)
        if (*it == value)
            return it;
    return last;
}
static int g_eqCalls = 0; static const char* g_eqText = 0;
struct CDefString {
    long TablePos;
    bool operator==(const char* text) const { ++g_eqCalls; g_eqText = text; return false; }
};
struct CWeaponClassVector {
    EWeaponClass* _M_start;
    EWeaponClass* _M_finish;
    EWeaponClass* _M_end_of_storage;
    const EWeaponClass* begin() const { return _M_start; }
    const EWeaponClass* end() const { return _M_finish; }
};
struct CVillageDef {
    virtual void v00() {}
    unsigned char      _pad04[0x24];
    CWeaponClassVector m_weaponClasses;
    CDefString         m_name;
    virtual void Validate(const CValidateDefinitionInitBase& init, const CCharString& name) const;
};
void CVillageDef::Validate(const CValidateDefinitionInitBase&, const CCharString&) const
{
    const EWeaponClass* last = m_weaponClasses.end();
    if (find_weapon_class(m_weaponClasses._M_start, last, WEAPON_CLASS_1, random_access_iterator_tag()) != last)
        m_name == "";
}
int main()
{
    EWeaponClass classes[3] = { WEAPON_CLASS_NONE, WEAPON_CLASS_NONE, WEAPON_CLASS_NONE };
    CVillageDef d; d.m_weaponClasses._M_start = classes; d.m_weaponClasses._M_finish = classes + 3; d.m_weaponClasses._M_end_of_storage = classes + 3;
    CValidateDefinitionInitBase init; CCharString name;
    // 1. no WEAPON_CLASS_1 entry: find runs, the name compare does not
    d.Validate(init, name); CHECK(g_findCalls == 1); CHECK(g_findValue == WEAPON_CLASS_1); CHECK(g_eqCalls == 0);
    // 2. a WEAPON_CLASS_1 entry: the name is compared against ""
    classes[1] = WEAPON_CLASS_1; d.Validate(init, name); CHECK(g_findCalls == 2); CHECK(g_eqCalls == 1); CHECK(g_eqText != 0 && g_eqText[0] == 0);
    // 3. empty vector: nothing but the find
    d.m_weaponClasses._M_finish = classes; d.Validate(init, name); CHECK(g_findCalls == 3); CHECK(g_eqCalls == 1);
    if (fails == 0) printf("007fe670_TEST PASS\n"); else printf("007fe670_TEST FAIL\n");
    return fails;
}