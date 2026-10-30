#include "rebuild_abi.h"
#include <stdio.h>

struct CActiveFile
{
    void* field_0;
    int   DstFileHandle;
};

extern void FABLE_FASTCALL CActiveFile_call1(void* thisp, int edx, char* p);
void FABLE_FASTCALL CActiveFile_OnReadFinished(CActiveFile* thisptr);

static int g_call1_seen_edx = 0;
static int g_call1_hits = 0;
static int g_call2_hits = 0;

void FABLE_FASTCALL CActiveFile_call1(void* thisp, int edx, char* p)
{
    (void)thisp; (void)p;
    g_call1_hits++;
    g_call1_seen_edx = edx;
}

static int g_free_hook = 0;   // the CRT frees its own blocks before main: count only the frees under test
extern "C" void __cdecl free(void* p)
{
    if (!g_free_hook) return;
    (void)p;
    g_call2_hits++;
}

int main(void)
{
    g_free_hook = 1;   // from here on free() is the hooked MSVCR71 free (0x00BFEA14)
    int dummy = 0;

    // field_0 non-null: both calls fire.
    CActiveFile a;
    a.field_0 = &dummy;
    a.DstFileHandle = 0x1234;
    CActiveFile_OnReadFinished(&a);

    // field_0 null: only call1 fires.
    CActiveFile b;
    b.field_0 = 0;
    b.DstFileHandle = 0x5678;
    CActiveFile_OnReadFinished(&b);

    if (g_call1_hits == 2 && g_call2_hits == 1 && g_call1_seen_edx == 0x5678)
        printf("PASS_0046ee0a\n");
    else
        printf("FAIL_0046ee0a c1=%d c2=%d edx=%x\n",
               g_call1_hits, g_call2_hits, g_call1_seen_edx);
    return 0;
}