// Test for 0x00BB1740 CEngineSubPrimitiveSprite::`scalar deleting destructor': an explicit virtual destructor call goes through vtable slot 0
// with flags 0 (destructor chain only), `delete` with flags 1 (chain, then the class operator delete). The base destructor
// ~CBaseClassNonCopyable 0x0099A300 and ??3CEngineSubPrimitiveBase@@SIXPAX@Z 0x00BEB550 are recording stubs; the object lives in a static
// buffer (the class operator new is not called).
#define FABLE_RETAIL_PASS22_PRIMITIVE_IMPLICIT_DESTRUCTORS
#include "engine/CEngineSubPrimitiveSprite.h"
#include <stdio.h>
#include <new>
static int g_dtor = 0, g_delete = 0; static void* g_deleted = 0; static void* g_dtor_this = 0;
CBaseClass::CBaseClass() { }
CBaseClass::~CBaseClass() { }
CBaseClassNonCopyable::CBaseClassNonCopyable() { }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { ++g_dtor; g_dtor_this = this; }
void __fastcall CEngineSubPrimitiveBase::operator delete(void* p) { ++g_delete; g_deleted = p; }
static double g_buf[8];
int main()
{
    CEngineSubPrimitiveSprite* s = ::new (g_buf) CEngineSubPrimitiveSprite((CEngineInternalPrimitiveSprite*)0);
    s->~CEngineSubPrimitiveSprite();
    if (g_dtor != 1 || g_delete != 0 || g_dtor_this != g_buf) { printf("00bb1740_TEST FAIL flags0 %d %d\n", g_dtor, g_delete); return 1; }
    s = ::new (g_buf) CEngineSubPrimitiveSprite((CEngineInternalPrimitiveSprite*)0);
    delete s;
    if (g_dtor != 2 || g_delete != 1 || g_deleted != g_buf) { printf("00bb1740_TEST FAIL flags1 %d %d\n", g_dtor, g_delete); return 1; }
    printf("00bb1740_TEST PASS\n");
    return 0;
}
