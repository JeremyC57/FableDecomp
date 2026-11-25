// Behaviour test for 0x00DCC000 NScript::CQ_OpeningGraveyardSecretPassageScript::`scalar deleting destructor' (NF434): links the row object
// and calls its ??_G copy through /alternatename (a __fastcall stand-in: ecx = this, edx unused, the flags argument popped by the callee,
// as the retail `ret 4`). The test defines the out-of-line callees the row object references (~CScriptBase, the CCharString
// constructor / destructor, the class's virtuals) and records them. Checks: hero_object[3..0] destroyed in reverse order, then ~CScriptBase
// on this; flags 0 frees nothing, flags 1 frees this; the result is this.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#include "engine/CQ_OpeningGraveyardSecretPassageScript.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef NScript::CQ_OpeningGraveyardSecretPassageScript Owner;
static const void* g_seq[16]; static int g_n = 0; static int g_frees = 0; static const void* g_freed = 0;
static void Rec(const void* p) { if (g_n < 16) g_seq[g_n++] = p; }
static const char g_base = 0;
void CScriptBase::RegisterMain() { }
void CScriptBase::Main() { }
void CScriptBase::Init() { }
void CScriptBase::OnPersist(CPersistContext&) { }
void Owner::RegisterMain() { }
void Owner::Main() { }
void Owner::Init() { }
void Owner::OnPersist(CPersistContext&) { }
// Special members the row object calls, as __fastcall stand-ins (ecx = this; stack arguments popped by the callee) bound by
// /alternatename (the 0x008C01B0 test pattern), so the test defines no class body that would emit a vftable of its own.
extern "C" void* __fastcall nf434_this(void* self) { return self; }
extern "C" void* __fastcall nf434_base_dtor(void* self) { Rec(&g_base); Rec(self); return self; }
extern "C" void* __fastcall nf434_str_dtor(void* self) { Rec(self); return self; }
#pragma comment(linker, "/alternatename:??0CCharString@@QAE@XZ=@nf434_this@4")
#pragma comment(linker, "/alternatename:??1CCharString@@QAE@XZ=@nf434_str_dtor@4")
#pragma comment(linker, "/alternatename:??0CScriptBase@@QAE@XZ=@nf434_this@4")
#pragma comment(linker, "/alternatename:??1CScriptBase@@UAE@XZ=@nf434_base_dtor@4")
void operator delete(void* p) { ++g_frees; g_freed = p; free(p); }
extern "C" void* __fastcall nf434_dd(void* self, int edx_unused, unsigned flags);
#pragma comment(linker, "/alternatename:@nf434_dd@12=??_GCQ_OpeningGraveyardSecretPassageScript@NScript@@UAEPAXI@Z")
static int Check(char* p)
{
    if (g_n != 6) return 0;
    for (int i = 0; i < 4; ++i) if (g_seq[i] != p + 0x58 + 4 * (3 - i)) return 0;
    return g_seq[4] == &g_base && g_seq[5] == p;
}
static double g_obj[(sizeof(Owner) + 7) / 8];
int main(void)
{
    int ok = sizeof(Owner) == 0x74;
    char* p = (char*)g_obj; memset(p, 0x5A, sizeof(g_obj));
    ok = ok && nf434_dd(p, 0, 0) == p && Check(p) && g_frees == 0;
    char* h = (char*)malloc(sizeof(Owner)); g_n = 0;
    ok = ok && nf434_dd(h, 0, 1) == h && Check(h) && g_frees == 1 && g_freed == h;
    if (ok) { printf("00dcc000_TEST PASS\n"); return 0; }
    printf("00dcc000_TEST FAIL\n");
    return 1;
}
