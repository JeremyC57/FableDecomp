// Behaviour test for 0x006a0d20 CCreatureAction_UseObject::Clone (links the REAL row object, NF273 rewrite of the standalone host model; PassPattern kept).
// The private PDB leaf ?Clone@CCreatureAction_UseObject@@EBEPAVCCreatureActionBase@@XZ is called through the test-only extern "C" alias CallClone (/alternatename, thiscall = __fastcall with one
// argument). The out-of-line copy constructor 0x006A0D50 is a __fastcall probe bound by /alternatename (thiscall + one stack argument, ret 4 =
// __fastcall(self, edx, other)); global operator new is replaced by a test definition that records the requested size and can fail.
// Cases: allocation of sizeof(CCreatureAction_UseObject) = 0xe0, the copy constructed in the new block from `*this` and returned; a failed allocation
// returns 0 without calling the copy constructor.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_USEOBJECT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CRightHandedSet.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_UseObject.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma comment(linker, "/alternatename:??0CCreatureAction_UseObject@@QAE@ABV0@@Z=@ProbeCopyCtor@12")
#pragma comment(linker, "/alternatename:@CallClone@4=?Clone@CCreatureAction_UseObject@@EBEPAVCCreatureActionBase@@XZ")
extern "C" void* __fastcall CallClone(const void* self);
static int g_copies, g_news, g_fail; static unsigned int g_size; static const void* g_src; static void* g_built;
void* operator new(unsigned int size)
{
    ++g_news; g_size = size;
    return g_fail ? 0 : malloc(size);
}
void operator delete(void* p) { free(p); }
extern "C" void* __fastcall ProbeCopyCtor(void* self, int, const void* other)
{
    ++g_copies; g_src = other; g_built = self;
    memcpy(self, other, sizeof(CCreatureAction_UseObject));
    return self;
}
int main()
{
    unsigned char src[sizeof(CCreatureAction_UseObject)];
    for (unsigned int i = 0; i < sizeof(src); ++i) src[i] = (unsigned char)(i * 7 + 3);
    void* c = CallClone(src);
    if (sizeof(CCreatureAction_UseObject) != 0xe0 || !c || c != g_built || g_news != 1 || g_size != 0xe0 || g_copies != 1 || g_src != (const void*)src
        || memcmp(c, src, sizeof(src)) != 0) { printf("006a0d20_TEST FAIL (copy)\n"); return 1; }
    operator delete(c);
    g_fail = 1;
    c = CallClone(src);
    if (c != 0 || g_news != 2 || g_copies != 1) { printf("006a0d20_TEST FAIL (null)\n"); return 1; }
    printf("006a0d20_TEST PASS\n");
    return 0;
}
