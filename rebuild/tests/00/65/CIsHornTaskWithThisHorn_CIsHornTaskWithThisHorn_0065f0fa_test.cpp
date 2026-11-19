// Test for CIsHornTaskWithThisHorn::CIsHornTaskWithThisHorn(CIntelligentPointer<CThing>) @ 0x0065F0FA (NF95 2026-11-19, q11276;
// NF97 2026-11-19 port): the classes come from the shared headers (engine/CThing.h, engine/CIntelligentPointer.h,
// engine/CIsHornTaskWithThisHorn.h with the row's opt-ins), no local models. The test links the row and supplies the out-of-line
// callees it binds: the CIntelligentPointer<CThing> copy 0x004A9730 and ~CBaseIntelligentPointer 0x00A01C10 (plus the base ctor and
// the GetPItem / SetPItem pair they use). Contract: PHorn is copy-constructed from the by-value argument, then the callee destroys
// the argument.
#define FABLE_RETAIL_CGAMETIMEMANAGER_FULL
#define FABLE_RETAIL_HORN_TASKS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CVILLAGETASKHORN
#define FABLE_RETAIL_CISHORNTASKWITHTHISHORN_CTOR_OUTOFLINE
#include <cstdio>
#include "engine/CThing.h"
#include "engine/CIsHornTaskWithThisHorn.h"

static int g_copies = 0;
static int g_destroys = 0;
static CBaseObject* g_items[16];
static int g_next = 0;
static const void* g_copyTargets[4];
static CBaseObject* g_copyItems[4];
CBaseIntelligentPointer::CBaseIntelligentPointer() : PData(0) { }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_destroys; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return PData ? *reinterpret_cast<CBaseObject* const*>(PData) : 0; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item)
{
    g_items[g_next] = item;
    PData = reinterpret_cast<CBaseObjectPointer*>(&g_items[g_next++]);
}
template<> CIntelligentPointer<CThing>::CIntelligentPointer(const CIntelligentPointer<CThing>& other) {
    CBaseObject* item = other.GetPItem();
    if (g_copies < 4) { g_copyTargets[g_copies] = this; g_copyItems[g_copies] = item; }
    ++g_copies;
    SetPItem(item);
}

int main()
{
    static unsigned long horn[4];
    CBaseObject* item = reinterpret_cast<CBaseObject*>(horn);
    int fails = 0;
    {
        CIntelligentPointer<CThing> source;
        source.SetPItem(item);
        g_copies = 0; g_destroys = 0;
        CIsHornTaskWithThisHorn predicate(source);   // caller copies the argument; the row copies it into PHorn and destroys it
        if (g_copies != 2) fails |= 1;
        if (g_destroys != 1) fails |= 2;
        if (g_copyTargets[1] != static_cast<const void*>(&predicate) || g_copyItems[1] != item) fails |= 4;   // PHorn is the object's only member (+0)
        if (source.GetPItem() != item) fails |= 8;
    }
    if (fails) { std::printf("ONCREATE_BUYHOUSE_FAIL %d\n", fails); return 1; }
    std::printf("ONCREATE_BUYHOUSE_OK\n");
    return 0;
}
