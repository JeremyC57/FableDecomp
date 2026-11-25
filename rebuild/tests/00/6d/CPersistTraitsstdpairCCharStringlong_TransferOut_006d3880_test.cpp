// Behaviour test for 0x006D3880 CPersistTraits<std::pair<CCharString, long> >::TransferOut: writes "(first,second)" with the
// separators "(" "," ")" (retail 0x0122E028 / 0x0122E024 / 0x0122E020), the string through its traits and the long through TransferOutLong.
// NF420 (2026-11-25): LINKED test -- it links the row object (the explicit instantiation's member is declared here, not defined); the
// callees are defined under the row's own spellings and append to one text buffer, so the test asserts the exact text; a mutated literal fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
class CPersistContext;
class CCharString { public: void* PStringData; CCharString(); ~CCharString(); const CCharString& operator+=(const char* text); };
void __fastcall TransferOutLong(CPersistContext* context, CCharString* out, long* value);
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CCharString> { CCharString DefaultVal; void TransferOut(CPersistContext& context, CCharString& out, const CCharString& value); };
template <class T1, class T2> struct CPersistTraits<std::pair<T1, T2> > {   // the row's member, declared only: the test calls the row's copy
    std::pair<T1, T2> DefaultVal;
    void TransferOut(CPersistContext& context, CCharString& out, const std::pair<T1, T2>* value) const;
};
static char g_out[256]; static CPersistContext* g_ctx = 0; static int g_ctxOk = 1; static int g_live = 0;
CCharString::CCharString() { PStringData = 0; ++g_live; }
CCharString::~CCharString() { --g_live; }
const CCharString& CCharString::operator+=(const char* text) { strcat((char*)PStringData, text); return *this; }
void __fastcall TransferOutLong(CPersistContext* context, CCharString* out, long* value)
{ char b[16]; if (context != g_ctx) g_ctxOk = 0; sprintf(b, "%ld", *value); strcat((char*)out->PStringData, b); }
void CPersistTraits<CCharString>::TransferOut(CPersistContext& context, CCharString& out, const CCharString& value)
{ if (&context != g_ctx) g_ctxOk = 0; out += "'"; out += (const char*)value.PStringData; out += "'"; }
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
static double g_traits[8];
int main()
{
    static double ctxRaw[4]; CPersistContext& ctx = *(CPersistContext*)ctxRaw; g_ctx = &ctx;
    CPersistTraits<std::pair<CCharString, long> >& traits = *(CPersistTraits<std::pair<CCharString, long> >*)g_traits;
    static char name[] = "HERO"; static double vraw[4]; std::pair<CCharString, long>& v = *(std::pair<CCharString, long>*)vraw;
    v.first.PStringData = name; v.second = -9;
    static double oraw[2]; CCharString& out = *(CCharString*)oraw; out.PStringData = g_out;
    traits.TransferOut(ctx, out, &v);
    bool ok = strcmp(g_out, "('HERO',-9)") == 0 && g_ctxOk && g_live == 0;
    if (ok) printf("006d3880_TEST PASS\n");
    else printf("FAIL out=%s live=%d\n", g_out, g_live);
    return ok ? 0 : 1;
}
