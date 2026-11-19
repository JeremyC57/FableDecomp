// Behaviour test for 0x005148D0 CReplaceSerialise<COldFileFormatMapHeader2, COldFileFormatMapHeader1>::ConvertPreviousUnknownVersionUp:
// the row's defines, includes and body verbatim; the previous step CBaseSerialise<COldFileFormatMapHeader1,1>::ConvertPreviousUnknownVersionUp
// (out of line in retail) is defined here and records its calls. Data already at version 1: no previous step; any other version: the
// previous step runs on the same data and the header it returns is released (deleted) before returning. Either way a fresh version-2
// header comes back with one reference, and nothing else is leaked or freed early.
#include <stdio.h>
#include <stdlib.h>
static int g_deletes = 0, g_news = 0;
void* operator new(unsigned int n) { ++g_news; return malloc(n); }
void operator delete(void* p) { if (p) { ++g_deletes; free(p); } }
#define FABLE_RETAIL_CFILEFORMATBRUSH
#define FABLE_RETAIL_CFILEFORMAT_HEIGHTMAP_V5
#define FABLE_RETAIL_CFILEFORMAT_OLDMAPHEADER12
#include "engine/CFileFormatBrush.h"
template<class T,class Old>
CCountedPointer<T> FABLE_FASTCALL CReplaceSerialise<T,Old>::ConvertPreviousUnknownVersionUp(void* old_void)
{
 CCountedPointer<T> new_thing(new T);
 CCountedPointer<Old> old_thing;
 Old* p_old_thing=(Old*)old_void;
 if(p_old_thing->SerialiseVersionNumber!=Old::GetClassVersionNumber()){
  old_thing=Old::ConvertPreviousUnknownVersionUp(old_void);
  p_old_thing=old_thing.Get();
 }
 new_thing->ConvertFromOld(p_old_thing);
 return new_thing;
}
template CCountedPointer<COldFileFormatMapHeader2> FABLE_FASTCALL
CReplaceSerialise<COldFileFormatMapHeader2,COldFileFormatMapHeader1>::ConvertPreviousUnknownVersionUp(void*);

static int g_prev_calls = 0;
static const void* g_prev_arg = 0;
template<> CCountedPointer<COldFileFormatMapHeader1> FABLE_FASTCALL
CBaseSerialise<COldFileFormatMapHeader1,1>::ConvertPreviousUnknownVersionUp(const void* old_void)
{
    ++g_prev_calls; g_prev_arg = old_void;
    COldFileFormatMapHeader1* h = new COldFileFormatMapHeader1;
    h->ByteLength = 5; h->SerialiseVersionNumber = 1;
    return CCountedPointer<COldFileFormatMapHeader1>(h);
}
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
int main()
{
    unsigned char data1[5] = { 5, 0, 0, 0, 1 };      // already version 1
    unsigned char data0[5] = { 5, 0, 0, 0, 0 };      // an older (unknown) version
    {
        int news = g_news, dels = g_deletes;
        CCountedPointer<COldFileFormatMapHeader2> r = CReplaceSerialise<COldFileFormatMapHeader2,COldFileFormatMapHeader1>::ConvertPreviousUnknownVersionUp(data1);
        check(g_prev_calls == 0, "version 1: no previous step");
        check(r.Data != 0 && (void*)r.Data != (void*)data1 && r.Info != 0 && r.Info->RefCount == 1, "fresh header, one reference");
        check(g_news - news == 2 && g_deletes == dels, "header + info allocated, nothing freed");
    }
    {
        int dels = g_deletes;
        CCountedPointer<COldFileFormatMapHeader2> r = CReplaceSerialise<COldFileFormatMapHeader2,COldFileFormatMapHeader1>::ConvertPreviousUnknownVersionUp(data0);
        check(g_prev_calls == 1 && g_prev_arg == data0, "older version: previous step on the same data");
        check(r.Data != 0 && r.Info != 0 && r.Info->RefCount == 1, "fresh header, one reference");
        check(g_deletes - dels == 2, "the previous step's header and its info released");
    }
    {
        unsigned char data7[5] = { 5, 0, 0, 0, 7 };
        CCountedPointer<COldFileFormatMapHeader2> r = CReplaceSerialise<COldFileFormatMapHeader2,COldFileFormatMapHeader1>::ConvertPreviousUnknownVersionUp(data7);
        check(g_prev_calls == 2, "any version other than 1 converts");
    }
    if (!g_fail) printf("005148d0_TEST PASS\n");
    return g_fail;
}