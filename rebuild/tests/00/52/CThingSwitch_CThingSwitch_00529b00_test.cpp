#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[128] = "";
static void note(const char* s) { std::strcat(g_seq, s); }
class CCharString { public: void* m_pStringData; CCharString() : m_pStringData(0) { note("S"); } };
struct CBaseIntelligentPointer { virtual void v00() { } void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { note("I"); } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() { } CIntelligentPointer() : CBaseIntelligentPointer() { } };
class CThing { public: virtual void v00() { } unsigned char _body[0xbc]; CThing(long type) { char s[4] = { 'T', (char)('0' + type), 0, 0 }; note(s); } };
class CThingGameObject : public CThing { public: virtual void v00() { } CIntelligentPointer m_owner; unsigned char _padc8[0xc]; CThingGameObject() : CThing(9), m_owner() { } };
class CThingSwitch : public CThingGameObject { public: virtual void v00() { } CCharString m_onName; unsigned char _padd8[8]; CCharString m_offName; CThingSwitch() : CThingGameObject(), m_onName(), m_offName() { } };

int main()
{
    CThingSwitch* s = new CThingSwitch; if (std::strcmp(g_seq, "T9ISS") || s->m_owner.m_pItem != 0 || s->m_onName.m_pStringData != 0 || s->m_offName.m_pStringData != 0) { std::printf("00529b00_TEST FAIL\n"); return 1; } delete s;
    std::printf("00529b00_TEST PASS\n");
    return 0;
}