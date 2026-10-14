#include <cstdio>
#include <cstring>
#include <new>

struct CThing { unsigned char m_thingKind; unsigned char m_followBit0 : 1; unsigned char m_followBit1 : 1; unsigned char m_followBit2 : 1;
    unsigned char Bits() const { return (unsigned char)(m_followBit0 | (m_followBit1 << 1) | (m_followBit2 << 2)); } };
struct CScriptThing { virtual CThing* GetThing() const { return m_thing; } virtual bool IsValid() const { return m_valid; } CThing* m_thing; bool m_valid; };
struct CGameScriptInterface {
    void Set(const CScriptThing& thing, bool flag) const {
        if (thing.IsValid()) { CThing* object = thing.GetThing(); if (object->m_thingKind == 1) object->m_followBit1 = !flag; } }
};
int main()
{
    CThing thing; std::memset(&thing, 0, sizeof thing); thing.m_thingKind = 1;
    CScriptThing st; st.m_thing = &thing; st.m_valid = true; CGameScriptInterface si;
    si.Set(st, true);  if (thing.Bits() != 0) { std::printf("0088ded0_TEST FAIL\n"); return 1; }
    si.Set(st, false); if (thing.Bits() != 2) { std::printf("0088ded0_TEST FAIL\n"); return 1; }
    thing.m_followBit0 = 1; thing.m_followBit1 = 1; thing.m_followBit2 = 1; unsigned char all = thing.Bits();
    st.m_valid = false; si.Set(st, false); if (thing.Bits() != all) { std::printf("0088ded0_TEST FAIL\n"); return 1; }                  // invalid thing: untouched
    st.m_valid = true; thing.m_thingKind = 2; si.Set(st, false); if (thing.Bits() != all) { std::printf("0088ded0_TEST FAIL\n"); return 1; }   // not a creature: untouched
    std::printf("0088ded0_TEST PASS\n");
    return 0;
}