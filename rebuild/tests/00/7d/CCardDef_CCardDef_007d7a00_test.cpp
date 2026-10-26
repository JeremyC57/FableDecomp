#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <new>
static int g_resCalls;
#pragma pack(push, 1)
class CResource { public: virtual void v00() {} unsigned char _body[0x18]; CResource() { ++g_resCalls; } };
struct __declspec(novtable) CDefinitionBase : CResource {
    long m_defIndex; long m_reserved; unsigned char m_flags : 3;
    CDefinitionBase() : CResource(), m_defIndex(0), m_flags(0) { }
};
class CDefString { public: long m_index; CDefString() : m_index(-1) { } };
struct __declspec(novtable) CCardDefBase : CDefinitionBase {
    CDefString m_name; long* m_first; long* m_last; long* m_end; unsigned char m_pad35; unsigned char m_flag;
    CCardDefBase() : CDefinitionBase(), m_name(), m_first(0), m_last(0), m_end(0), m_flag(0) { }
};
class CCardDef : public CCardDefBase { public: virtual void v00() {} CCardDef(); };
#pragma pack(pop)
CCardDef::CCardDef() : CCardDefBase() { }
int main()
{
    unsigned char raw[sizeof(CCardDef)]; memset(raw, 0xFF, sizeof(raw));
    CCardDef* d = new (raw) CCardDef();
    bool ok = g_resCalls == 1 && d->m_defIndex == 0 && d->m_flags == 0 && (raw[0x24] & 0xF8) == 0xF8 && d->m_name.m_index == -1
        && d->m_first == 0 && d->m_last == 0 && d->m_end == 0 && d->m_flag == 0 && d->m_pad35 == 0xFF
        && offsetof(CCardDef, m_name) == 0x25 && offsetof(CCardDef, m_flag) == 0x36;
    if (!ok) { printf("007d7a00_TEST FAIL\n"); return 1; }
    printf("007d7a00_TEST PASS\n"); return 0;
}