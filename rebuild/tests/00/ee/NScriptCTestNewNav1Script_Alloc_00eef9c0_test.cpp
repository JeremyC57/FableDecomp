// Behaviour test for 0x00EEF9C0 NScript::CTestNewNav1Script::Alloc (NF434): links the row object and calls Alloc. The out-of-line callees the row object
// references are defined here and record their calls: CScriptBase's constructor / destructor as __fastcall stand-ins bound by
// /alternatename (the 0x008C01B0 test pattern), operator new / delete, and the virtuals of both vftables the object emits. Checks: one
// 0x48-byte allocation, CScriptBase() on it, Game +0x40 / MasterScriptData +0x44 stored, the class's own overrides reached through the
// vptr (not CScriptBase's), and `delete` reaching ~CScriptBase on this and freeing it (the implicit ??_G).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#include "engine/CTestNewNav1Script.h"
#include <stdio.h>
#include <stdlib.h>
typedef NScript::CTestNewNav1Script Owner;
static int g_last = 0; static int g_ctor = 0; static void* g_ctorThis = 0; static void* g_dtorThis = 0;
static unsigned g_size = 0; static void* g_new = 0; static void* g_freed = 0; static long g_ctx[4];
void Owner::RegisterMain() { g_last = 1; }
void Owner::Main() { g_last = 2; }
void CScriptBase::RegisterMain() { g_last = 11; }
void CScriptBase::Main() { g_last = 12; }
void CScriptBase::Init() { g_last = 13; }
void CScriptBase::OnPersist(CPersistContext&) { g_last = 14; }
extern "C" void* __fastcall nf434_base_ctor(void* self) { ++g_ctor; g_ctorThis = self; return self; }
extern "C" void* __fastcall nf434_base_dtor(void* self) { g_dtorThis = self; return self; }
#pragma comment(linker, "/alternatename:??0CScriptBase@@QAE@XZ=@nf434_base_ctor@4")
#pragma comment(linker, "/alternatename:??1CScriptBase@@UAE@XZ=@nf434_base_dtor@4")
void* operator new(size_t n) { g_size = (unsigned)n; g_new = malloc(n); return g_new; }
void operator delete(void* p) { g_freed = p; free(p); }
int main(void)
{
    static long data[4]; static long game[4];
    CScriptBase* b = Owner::Alloc((CScriptDataBase*)data, *(CGameScriptInterfaceBase*)game);
    Owner* p = (Owner*)b;
    int ok = b != 0 && (void*)b == g_new && g_size == 0x48 && g_ctor == 1 && g_ctorThis == (void*)b;
    ok = ok && (void*)p->Game == (void*)game && (void*)p->MasterScriptData == (void*)data;
    p->RegisterMain(); ok = ok && g_last == 1;
    p->Main(); ok = ok && g_last == 2;
    delete p;
    ok = ok && g_dtorThis == (void*)b && g_freed == (void*)b;
    if (ok) { printf("00eef9c0_TEST PASS\n"); return 0; }
    printf("00eef9c0_TEST FAIL\n");
    return 1;
}
