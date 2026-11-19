// Behaviour test for 0x005DC340 CAnimationSet::GetAnim: the row's defines, includes and body verbatim. The map is a raw image of
// sorted (key, CAnimationEntry) pairs; FindFirstAnimWithName (out of line in retail, 0x005DC020) is a test stand-in returning the first
// pair whose key is g_key (the name's CRC in retail). The filter accepts entries with an odd BankIndex. Within the key-7 run
// (banks 2,3,4,5,7) the n-th accepted entry must come back for n = 0,1,2 (banks 3,5,7): a mutant that ignores the filter, counts
// rejected entries, or starts from the map's first pair fails.
#include <stdio.h>
#include <string.h>
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CANIMATIONSET_ANIMS
#include "fable_string.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimFilterBase.h"
#include "engine/CAnimationSet.h"

const CAnimationEntry& CAnimationSet::GetAnim(const CCharString& name, long entry_num, const CAnimFilterBase& filter) const
{
    TAnimMap::const_iterator start_it = FindFirstAnimWithName(name);
    for (TAnimMap::const_iterator it = start_it; it != Anims.end() && it->first == start_it->first; ++it)
    {
        if (true)
        {
            if (entry_num == 0)
                return it->second;
            entry_num--;
        }
    }
#ifdef _MSC_VER
    __assume(0);
#endif
}

static unsigned long g_key;
CAnimationSet::TAnimMap::const_iterator CAnimationSet::FindFirstAnimWithName(const CCharString&) const
{
    for (TAnimMap::const_iterator it = Anims.begin(); it != Anims.end(); ++it)
        if (it->first == g_key)
            return it;
    return Anims.end();
}

class CTestOddBankFilter : public CAnimFilterBase {
public:
    virtual bool operator()(const CAnimationEntry& entry) const { return (*(const long*)&entry & 1) != 0; }
    virtual CAnimFilterBase* Clone() const { return 0; }
};

static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
static long pairs[7][5] = { {5, 1}, {7, 2}, {7, 3}, {7, 4}, {7, 5}, {7, 7}, {9, 9} };
static double image[0x20 / 8];
int main()
{
    memset(image, 0, sizeof(image));
    ((long**)image)[0] = &pairs[0][0];
    ((long**)image)[1] = &pairs[7][0];
    ((long**)image)[2] = &pairs[7][0];
    const CAnimationSet* set = (const CAnimationSet*)image;
    CTestOddBankFilter filter;
    const CCharString* name = 0;
    g_key = 7;
    check(&set->GetAnim(*name, 0, filter) == (const CAnimationEntry*)&pairs[2][1], "first accepted of key 7");
    check(&set->GetAnim(*name, 1, filter) == (const CAnimationEntry*)&pairs[4][1], "second accepted of key 7");
    check(&set->GetAnim(*name, 2, filter) == (const CAnimationEntry*)&pairs[5][1], "third accepted of key 7");
    g_key = 5;
    check(&set->GetAnim(*name, 0, filter) == (const CAnimationEntry*)&pairs[0][1], "only entry of key 5");
    g_key = 9;
    check(&set->GetAnim(*name, 0, filter) == (const CAnimationEntry*)&pairs[6][1], "last pair of the map");
    if (!g_fail) printf("005dc340_TEST PASS\n");
    return g_fail;
}
