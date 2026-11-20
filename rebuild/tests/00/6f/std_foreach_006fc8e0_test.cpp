// std::for_each<const NAnimationEvents::CEvent*, NAnimationEvents::CFillEventNames> (0x006FC8E0)
// Leaf: ??$for_each@PBVCEvent@NAnimationEvents@@VCFillEventNames@2@@std@@YI?AVCFillEventNames@NAnimationEvents@@PBVCEvent@2@0V12@@Z
// Test (NF208, 2026-11-20): the row on the shared headers (fable_string.h, engine/CSymbolMap.h, stlport/fable_containers.h,
//   stlport/fable_algorithm.h); the harness defines the CCharString members (a reference count per string id), the CSymbolMap
//   lookup FindFirstSymbolWithVal(long) 0x009E5090 over a fixed table, and the out-of-line constructors / destructors the headers
//   declare; push_back runs both the in-place and the _M_insert_overflow (0x00433530) paths.
#include "fable_compiler.h"
#include "rebuild_abi.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_CSYMBOLMAP
#define FABLE_RETAIL_CSYMBOLMAP_FINDFIRSTSYMBOLWITHVAL_VALUE
#include "engine/CSymbolMap.h"
#define FABLE_RETAIL_STLP_FOR_EACH_BODY
#include "stlport/fable_algorithm.h"

namespace NAnimationEvents {
class CEvent {                                            // Ego_r.pdb: 8 bytes
public:
    unsigned long Index : 30;                             // +0x00
    unsigned long ExternallyImported : 1;
    unsigned long StopEvent : 1;
    float Time;                                           // +0x04
};

class CFillEventNames {                                   // Ego_r.pdb: 8 bytes
public:
    std::vector<CCharString>* EventNames;                 // +0x00
    const CSymbolMap* EventSymbolMap;                     // +0x04
    CFillEventNames(std::vector<CCharString>& names, const CSymbolMap& symbols) : EventNames(&names), EventSymbolMap(&symbols) {}
    void operator()(const CEvent& event)
    {
        EventNames->push_back(EventSymbolMap->FindFirstSymbolWithVal(event.Index)->EntryName);
    }
};
}

template NAnimationEvents::CFillEventNames FABLE_FASTCALL std::for_each(const NAnimationEvents::CEvent* first, const NAnimationEvents::CEvent* last,
                                                                    NAnimationEvents::CFillEventNames f);
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static long gRefs[256];
static long gLive;
static unsigned long IdOf(const CCharString& s) { void* p; memcpy(&p, &s, sizeof p); return (unsigned long)(size_t)p; }
static void SetId(CCharString* s, unsigned long id) { void* p = (void*)(size_t)id; memcpy(s, &p, sizeof p); }
CCharString::CCharString() { SetId(this, 0); }
CCharString::CCharString(const char* text, long) { unsigned long id = (unsigned char)text[0]; SetId(this, id); if (id) { ++gRefs[id]; ++gLive; } }
CCharString::CCharString(const CCharString& other) { unsigned long id = IdOf(other); SetId(this, id); if (id) { ++gRefs[id]; ++gLive; } }
CCharString::~CCharString() { unsigned long id = IdOf(*this); if (id) { --gRefs[id]; --gLive; } SetId(this, 0); }

CBaseClass::CBaseClass() {} CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {} CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CSymbolMap::CSymbolMap() : CheckForDuplicateSymbols(false) {} CSymbolMap::~CSymbolMap() {}
static const CSymbolMap::CSymbolInfo* gTable;
static int gTableCount;
static const CSymbolMap* gExpectedMap;
static int gBad;
const CSymbolMap::CSymbolInfo* CSymbolMap::FindFirstSymbolWithVal(long value) const
{
    if (this != gExpectedMap) gBad = 1;
    for (int i = 0; i < gTableCount; ++i)
        if (gTable[i].Val == value)
            return gTable + i;
    return 0;
}

static int Run()
{
    CSymbolMap::CSymbolInfo* table = (CSymbolMap::CSymbolInfo*)malloc(3 * sizeof(CSymbolMap::CSymbolInfo));
    new (&table[0].GroupName) CCharString("e"); new (&table[0].EntryName) CCharString("f"); table[0].Val = 3;
    new (&table[1].GroupName) CCharString("e"); new (&table[1].EntryName) CCharString("s"); table[1].Val = 0x3fffffff;
    new (&table[2].GroupName) CCharString("e"); new (&table[2].EntryName) CCharString("i"); table[2].Val = 7;
    gTable = table; gTableCount = 3;
    int r = 0;
    {
        CSymbolMap symbols;
        gExpectedMap = &symbols;
        NAnimationEvents::CEvent events[3];
        const unsigned long index[3] = { 7, 0x3fffffff, 3 };
        for (int i = 0; i < 3; ++i) { events[i].Index = index[i]; events[i].ExternallyImported = 1; events[i].StopEvent = 1; events[i].Time = 0.25f * (i + 1); }
        std::vector<CCharString> names;
        NAnimationEvents::CFillEventNames f = std::for_each((const NAnimationEvents::CEvent*)events, (const NAnimationEvents::CEvent*)events + 3,
                                                            NAnimationEvents::CFillEventNames(names, symbols));
        if (gBad || f.EventNames != &names || f.EventSymbolMap != &symbols || names.size() != 3) r = 1;
        else if (IdOf(names[0]) != 'i' || IdOf(names[1]) != 's' || IdOf(names[2]) != 'f') r = 2;
        else if (gRefs['i'] != 2 || gRefs['s'] != 2 || gRefs['f'] != 2 || gRefs['e'] != 3) r = 3;
    }
    for (int i = 0; i < 3; ++i) { table[i].GroupName.~CCharString(); table[i].EntryName.~CCharString(); }
    free(table);
    return r;
}

int main()
{
    int r = Run();
    if (r) { printf("006fc8e0_TEST FAIL %d\n", r); return r; }
    if (gLive != 0) { printf("006fc8e0_TEST FAIL leak %ld\n", gLive); return 9; }
    printf("006fc8e0_TEST PASS\n");
    return 0;
}
