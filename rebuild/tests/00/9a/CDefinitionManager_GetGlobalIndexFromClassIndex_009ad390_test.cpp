// Behaviour test for CDefinitionManager::GetGlobalIndexFromClassIndex (0x009AD390): the class record GetPDefClassInfo(name) finds, then
// its GlobalIndices[class_index]. NF39 (2026-11-16): the row is now the CDefinitionManager member on the shared header, so the test
// links the source object and supplies the one callee, GetPDefClassInfo (0x009AD2E0), returning a record whose GlobalIndices words
// (start / finish / end +8 / +0xC / +0x10, the STLport vector) point at a test array. No local class model.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_CTOR_SETSTRING
#define FABLE_RETAIL_CSYMBOLMAP
#define FABLE_RETAIL_CSYMBOLMAP_ADDSYMBOLDIRTY
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR_ADOPT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATEDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER_CLASSINFO_QUERIES
#include "fable_string.h"
#include "engine/CDefString.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include <cstdio>

static unsigned long g_indices[4] = { 10, 20, 30, 40 };
static void* g_record[5];                     // CDefClassInfo words: Name, AllocFunc, GlobalIndices start / finish / end
static void* g_manager[16];                   // storage the call's `this` points at (the member never reads it)
static void* g_name[1];                       // the CCharString argument (passed through, never read)

const CDefinitionManager::CDefClassInfo* CDefinitionManager::GetPDefClassInfo(const CCharString&) const
{
    return reinterpret_cast<const CDefClassInfo*>(g_record);
}

int main()
{
    g_record[2] = g_indices; g_record[3] = g_indices + 4; g_record[4] = g_indices + 4;
    const CDefinitionManager* manager = reinterpret_cast<const CDefinitionManager*>(g_manager);
    long r = manager->GetGlobalIndexFromClassIndex(*reinterpret_cast<const CCharString*>(g_name), 2);
    if (r == 30) printf("009ad390_TEST PASS\n");
    else printf("FAIL r=%ld\n", r);
    return 0;
}
