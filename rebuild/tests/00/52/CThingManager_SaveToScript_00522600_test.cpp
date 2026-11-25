// Behaviour test for 0x00522600 CThingManager::SaveToScript: writes "Version " (retail 0x01245BA4) + the serialise version + ";\r\n"
// (0x012353B8), then saves (mode 2) every thing on the given map that is serialisable and not forced out.
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); the callees are defined
// under the row's own spellings over one text buffer, so the test asserts the exact script text; a mutated literal fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
class CCharString { public: void* m_pData; ~CCharString(); const CCharString& operator+=(const CCharString& other); };
CCharString __fastcall GFIntToCharString(long value);
CCharString __fastcall operator+(const char* left, const CCharString& right);
CCharString __fastcall operator+(const CCharString& left, const char* right);
class CThing { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual bool IsToBeSerialised();                           // slot 22 (+0x58)
    unsigned char _pad04[0x8e - 0x04]; short CurrentMapNumber; // +0x8e
    bool Serialise, ForceNo; int Id;
    bool IsForceNoSerialise() const;
};
void CThing::v00() {} void CThing::v01() {} void CThing::v02() {} void CThing::v03() {} void CThing::v04() {} void CThing::v05() {}
void CThing::v06() {} void CThing::v07() {} void CThing::v08() {} void CThing::v09() {} void CThing::v10() {} void CThing::v11() {}
void CThing::v12() {} void CThing::v13() {} void CThing::v14() {} void CThing::v15() {} void CThing::v16() {} void CThing::v17() {}
void CThing::v18() {} void CThing::v19() {} void CThing::v20() {} void CThing::v21() {}
bool CThing::IsToBeSerialised() { return Serialise; }
bool CThing::IsForceNoSerialise() const { return ForceNo; }
struct CStringVector;
class CThingManager { public: void SaveToScript(CCharString& script, long mapNumber, CStringVector* names); };
static char g_script[256]; static char g_bufs[4][64]; static int g_buf = 0; static char g_saved[64]; static int g_bad = 0; static CStringVector* g_names = (CStringVector*)0x1234;
static CCharString Make(const char* a, const char* b) { char* d = g_bufs[g_buf++ & 3]; strcpy(d, a); strcat(d, b); CCharString r; r.m_pData = d; return r; }
CCharString::~CCharString() { }
const CCharString& CCharString::operator+=(const CCharString& other) { strcat((char*)m_pData, (const char*)other.m_pData); return *this; }
CCharString __fastcall GFIntToCharString(long value) { char b[16]; sprintf(b, "%ld", value); return Make(b, ""); }
CCharString __fastcall operator+(const char* left, const CCharString& right) { return Make(left, (const char*)right.m_pData); }
CCharString __fastcall operator+(const CCharString& left, const char* right) { return Make((const char*)left.m_pData, right); }
void WriteThingToScript(CCharString& script, CThing* thing, long mode, CStringVector* names)
{ char b[8]; sprintf(b, "%d", thing->Id); strcat(g_saved, b); if (&script == 0 || mode != 2 || names != g_names) g_bad = 1; }
void* GrowVectorStorage_00436ac0(unsigned long count) { return malloc(count * sizeof(void*)); }
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
struct Node { Node* next; Node* prev; CThing* data; };
int main()
{
    static CThing t[4]; Node sentinel, n[4];
    Node* prev = &sentinel;
    for (int i = 0; i < 4; ++i) { t[i].Id = i + 1; t[i].CurrentMapNumber = 3; t[i].Serialise = true; t[i].ForceNo = false; n[i].data = &t[i]; prev->next = &n[i]; prev = &n[i]; }
    prev->next = &sentinel;
    t[1].CurrentMapNumber = 4; t[2].ForceNo = true; t[3].Serialise = false;   // 2: other map, 3: forced out, 4: not serialisable
    // (one eligible thing: the row's local vector model regrows into fresh storage holding only the pushed element, so a second
    //  eligible thing would test that stand-in, not the row)
    static double mgrRaw[0x100 / 8]; unsigned char* mgr = (unsigned char*)mgrRaw;
    *(Node**)(mgr + 0x18) = &sentinel; *(long*)(mgr + 0xd8) = 12;
    CCharString script; script.m_pData = g_script;
    ((CThingManager*)mgr)->SaveToScript(script, 3, g_names);
    bool ok = !g_bad && strcmp(g_script, "Version 12;\r\n") == 0 && strcmp(g_saved, "1") == 0;
    if (ok) printf("00522600_TEST PASS\n");
    else printf("FAIL bad=%d script=%s saved=%s\n", g_bad, g_script, g_saved);
    return ok ? 0 : 1;
}
