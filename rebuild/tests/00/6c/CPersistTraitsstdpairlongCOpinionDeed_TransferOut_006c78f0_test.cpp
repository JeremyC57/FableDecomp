// Behaviour test for 0x006C78F0 CPersistTraits<std::pair<long, COpinionDeed*> >::TransferOut: writes "(first,second)" with the
// separators "(" "," ")" (retail 0x0122E028 / 0x0122E024 / 0x0122E020), the long through TransferOutLong and the deed through its traits.
// NF420 (2026-11-25): LINKED test -- it links the row object (the explicit instantiation's member is declared here, not defined); the
// callees are defined under the row's own spellings and append to one text buffer, so the test asserts the exact text; a mutated literal fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
class CPersistContext;
class CCharString { public: void* PStringData; const CCharString& operator+=(const char* text); };
void __fastcall TransferOutLong(CPersistContext* context, CCharString* out, long* value);
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
class COpinionDeed { public: int Id; };
template <class T> struct CPersistTraits { T DefaultVal; void TransferOut(CPersistContext& context, CCharString& out, const T* value) const; };
template <class T1, class T2> struct CPersistTraits<std::pair<T1, T2> > {   // the row's member, declared only: the test calls the row's copy
    std::pair<T1, T2> DefaultVal;
    void TransferOut(CPersistContext& context, CCharString& out, const std::pair<T1, T2>* value) const;
};
static char g_out[256]; static CPersistContext* g_ctx = 0; static int g_ctxOk = 1;
const CCharString& CCharString::operator+=(const char* text) { strcat((char*)PStringData, text); return *this; }
void __fastcall TransferOutLong(CPersistContext* context, CCharString* out, long* value)
{ char b[16]; if (context != g_ctx) g_ctxOk = 0; sprintf(b, "%ld", *value); strcat((char*)out->PStringData, b); }
template <> void CPersistTraits<COpinionDeed*>::TransferOut(CPersistContext& context, CCharString& out, COpinionDeed* const* value) const
{ char b[16]; if (&context != g_ctx) g_ctxOk = 0; sprintf(b, "deed%d", (*value)->Id); out += b; }
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    static double ctxRaw[4]; CPersistContext& ctx = *(CPersistContext*)ctxRaw; g_ctx = &ctx;
    CPersistTraits<std::pair<long, COpinionDeed*> > traits; memset(&traits, 0, sizeof(traits));
    COpinionDeed deed; deed.Id = 42;
    std::pair<long, COpinionDeed*> v; v.first = 17; v.second = &deed;
    CCharString out; out.PStringData = g_out;
    traits.TransferOut(ctx, out, &v);
    bool ok = strcmp(g_out, "(17,deed42)") == 0 && g_ctxOk;
    if (ok) printf("006c78f0_TEST PASS\n");
    else printf("FAIL out=%s\n", g_out);
    return ok ? 0 : 1;
}
