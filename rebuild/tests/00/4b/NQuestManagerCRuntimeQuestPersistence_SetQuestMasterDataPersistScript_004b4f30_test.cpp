// Behaviour test for 004b4f30: final.cpp copied verbatim, the string, map and sort callees stubbed below so the hit /
// miss paths, the entry assignment, the push_back, the dirty flag and the re-sort are observable.
// NQuestManager::CRuntimeQuestPersistence::SetQuestMasterDataPersistScript
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __cdecl
#define __fastcall
#define _snprintf snprintf
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
#define CHECKLOG(e) do { if (strcmp(g_log, e) != 0) { ++fails; printf("FAIL %d: log=%s\n  expected=%s\n", __LINE__, g_log, e); } } while (0)
static char g_log[4096];
static void note(const char* s) { if (strlen(g_log) + strlen(s) + 2 < sizeof(g_log)) { strcat(g_log, s); strcat(g_log, ";"); } }
// ---- source (verbatim) ----
// NQuestManager::CRuntimeQuestPersistence::SetQuestMasterDataPersistScript(const CCharString& quest, const CCharString& script) -- record the persist script for the quest in the master-data persist map: overwrite the entry when the quest is present, else add {quest, script} and re-sort the map
// Leaf: SetQuestMasterDataPersistScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B4F30, length 249 bytes (`ret 8`).
// FableWin.pdb: ?SetQuestMasterDataPersistScript@CRuntimeQuestPersistence@NQuestManager@@QAEXABVCCharString@@0@Z (the manifest gives both rows the
// SetQuestMasterDataPersistScript label; the +0 map row 0x004B4E30 is SetQuestScriptPersistScript, the +0x10 one 0x004B4F30).
// Callees: CVectorMap<CCharString, CPersistInfo>::LowerBound @ 0x004B9750, CBasicString<char>::operator< @ 0x00429950 (the
// inline CCharString <), CCharString copy ctor @ 0x0099EC30 / operator= @ 0x0099EFB0 / dtor @ 0x0099EAE0, the pair vector's
// push_back @ 0x004BB780, SGI sort<pair*, CKeyPairCompareLess> @ 0x004BBB40 (`__fastcall`, the empty comparator by value).
// Layout: the landed GetQuestMasterDataPersistScript 0x004B27F0 model (PersistScripts +0, MasterDataPersistScripts +0x10;
// each CVectorMap = 12-byte STLport vector + Compare +0xc + Dirty +0xd).
// Shape: the map's inline Find (`last` read once after LowerBound), then a CPersistInfo local built from the arguments; a hit
// assigns it through the inline memberwise operator=, a miss runs the inline Add (pair temporary in its own block, Dirty =
// true) and the inline SortIfDirty (the test folds, the true store stays before the sort call, false after).
// The sort's first/last/Compare loads go through `CPersistMap* const& self = this` (a pointer that went through a
// reference is may-alias): the `Dirty = true` store then stays before them as in retail; read directly, VC7.1 sinks the
// store below the comparator load (the 00a02b30 rule).
class CCharStringData { public:
    bool operator<(const CCharStringData& other) const;   // 0x00429950 (CBasicString<char>::operator<)
};

class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
    CCharString& operator=(const CCharString& other);     // 0x0099EFB0
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (PStringData == 0)
            return true;
        if (other.PStringData == 0)
            return false;
        return *PStringData < *other.PStringData;
    }
};

namespace NQuestManager {

struct CRuntimeQuestPersistence {
    class CPersistInfo { public:
        CCharString QuestName;                     // +0x00
        CCharString PersistScript;                 // +0x04
        CPersistInfo(const CCharString& questName, const CCharString& persistScript)
            : QuestName(questName), PersistScript(persistScript) { }
        __forceinline CPersistInfo(const CPersistInfo& other) : QuestName(other.QuestName), PersistScript(other.PersistScript) { }
        __forceinline CPersistInfo& operator=(const CPersistInfo& other)
        {
            QuestName = other.QuestName;
            PersistScript = other.PersistScript;
            return *this;
        }
    };
    struct SPair {                                 // pair<CCharString, CPersistInfo>, 12 bytes
        CCharString  first;                        // +0x00
        CPersistInfo second;                       // +0x04
        __forceinline SPair(const CCharString& key, const CPersistInfo& value) : first(key), second(value) { }
    };
    struct CKeyPairCompareLess { };                // empty comparator (1 byte)
    struct CPersistMap {                           // CVectorMap<CCharString, CPersistInfo>: STLport vector, Compare, Dirty
        SPair*              m_pFirst;              // +0x00
        SPair*              m_pLast;               // +0x04
        SPair*              m_pEnd;                // +0x08
        CKeyPairCompareLess Compare;               // +0x0c
        bool                Dirty;                 // +0x0d
        SPair* LowerBound(const CCharString& key);   // 0x004B9750
        void   push_back(const SPair& value);        // 0x004BB780 (STLport vector::push_back)
        SPair* end() { return m_pLast; }
        SPair* Find(const CCharString& key)
        {
            SPair* it = LowerBound(key);
            SPair* last = m_pLast;
            return (it == last || key < it->first) ? last : it;
        }
        __forceinline void Add(const CCharString& key, const CPersistInfo& value)
        {
            {
                SPair pair(key, value);
                push_back(pair);
            }
            Dirty = true;
            SortIfDirty();
        }
        __forceinline void SortIfDirty();
    };
    CPersistMap PersistScripts;                    // +0x00
    CPersistMap MasterDataPersistScripts;          // +0x10
    void SetQuestScriptPersistScript(const CCharString& quest, const CCharString& script);
    void SetQuestMasterDataPersistScript(const CCharString& quest, const CCharString& script);
};

void __fastcall sort(CRuntimeQuestPersistence::SPair* first, CRuntimeQuestPersistence::SPair* last,
                     CRuntimeQuestPersistence::CKeyPairCompareLess comp);   // 0x004BBB40 (SGI sort, /Gr)

__forceinline void CRuntimeQuestPersistence::CPersistMap::SortIfDirty()
{
    if (Dirty) {
        CPersistMap* const& self = this;             // the sort's loads through a may-alias pointer: the Dirty store stays before them
        sort(self->m_pFirst, self->m_pLast, self->Compare);
        Dirty = false;
    }
}

void CRuntimeQuestPersistence::SetQuestMasterDataPersistScript(const CCharString& quest, const CCharString& script)
{
    SPair* it = MasterDataPersistScripts.Find(quest);
    CPersistInfo info(quest, script);
    if (it != MasterDataPersistScripts.end())
        it->second = info;
    else
        MasterDataPersistScripts.Add(quest, info);
}

}
// ---- stubs ----
using namespace NQuestManager;
static CCharStringData g_data[8];                     // string "values": compared by their index
static const char* Name(const CCharStringData* d) { static char bufs[4][16]; static int k = 0; char* b = bufs[k++ & 3]; if (!d) return "null"; _snprintf(b, 16, "s%d", (int)(d - g_data)); return b; }   // rotating buffers: up to three names per message
bool CCharStringData::operator<(const CCharStringData& other) const { return this < &other; }
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { char b[32]; _snprintf(b, sizeof b, "copy(%s)", Name(PStringData)); note(b); }
CCharString::~CCharString() { char b[32]; _snprintf(b, sizeof b, "~(%s)", Name(PStringData)); note(b); }
CCharString& CCharString::operator=(const CCharString& other) { char b[48]; _snprintf(b, sizeof b, "assign(%s<-%s)", Name(PStringData), Name(other.PStringData)); note(b); PStringData = other.PStringData; return *this; }
CRuntimeQuestPersistence::SPair* CRuntimeQuestPersistence::CPersistMap::LowerBound(const CCharString& key)
{
    SPair* it = m_pFirst;
    while (it != m_pLast && it->first < key) ++it;
    char b[32]; _snprintf(b, sizeof b, "lower(%d)", (int)(it - m_pFirst)); note(b);
    return it;
}
void CRuntimeQuestPersistence::CPersistMap::push_back(const SPair& value)
{
    char b[64]; _snprintf(b, sizeof b, "push(%s,%s,%s)", Name(value.first.PStringData), Name(value.second.QuestName.PStringData), Name(value.second.PersistScript.PStringData)); note(b);
    memcpy((void*)m_pLast, (const void*)&value, sizeof(SPair)); ++m_pLast;
}
namespace NQuestManager {
void __fastcall sort(CRuntimeQuestPersistence::SPair* first, CRuntimeQuestPersistence::SPair* last, CRuntimeQuestPersistence::CKeyPairCompareLess comp)
{
    char b[32]; _snprintf(b, sizeof b, "sort(%d)", (int)(last - first)); note(b);
    for (CRuntimeQuestPersistence::SPair* i = first; i != last; ++i)
        for (CRuntimeQuestPersistence::SPair* j = i; j != first && j->first < (j - 1)->first; --j) {
            unsigned char t[sizeof(CRuntimeQuestPersistence::SPair)]; memcpy(t, (void*)j, sizeof t); memcpy((void*)j, (void*)(j - 1), sizeof t); memcpy((void*)(j - 1), t, sizeof t);
        }
}
}
static CCharString& Str(void* slot, CCharStringData* d) { CCharString* s = (CCharString*)slot; s->PStringData = d; return *s; }
int main()
{
    CRuntimeQuestPersistence* rp = (CRuntimeQuestPersistence*)calloc(1, sizeof(CRuntimeQuestPersistence));
    unsigned char pool[8 * sizeof(CRuntimeQuestPersistence::SPair)]; memset(pool, 0, sizeof pool);
    CRuntimeQuestPersistence::CPersistMap& map = rp->MasterDataPersistScripts;
    map.m_pFirst = map.m_pLast = (CRuntimeQuestPersistence::SPair*)pool; map.m_pEnd = map.m_pFirst + 8; map.Dirty = false;
    void* slotQ[4]; void* slotS[4]; for (int i = 0; i < 4; ++i) { slotQ[i] = malloc(sizeof(CCharString)); slotS[i] = malloc(sizeof(CCharString)); }
    CCharString& q3 = Str(slotQ[0], &g_data[3]); CCharString& s5 = Str(slotS[0], &g_data[5]);
    // 1. empty map: miss -> info built, pair pushed, dirty, sorted, dirty cleared
    g_log[0] = 0; rp->SetQuestMasterDataPersistScript(q3, s5);
    CHECKLOG("lower(0);copy(s3);copy(s5);copy(s3);copy(s3);copy(s5);push(s3,s3,s5);~(s5);~(s3);~(s3);sort(1);~(s5);~(s3);");
    CHECK(map.m_pLast - map.m_pFirst == 1 && !map.Dirty && map.m_pFirst[0].first.PStringData == &g_data[3] && map.m_pFirst[0].second.PersistScript.PStringData == &g_data[5]);
    // 2. hit on the same quest: the entry's info is assigned, nothing pushed or sorted
    CCharString& s6 = Str(slotS[1], &g_data[6]);
    g_log[0] = 0; rp->SetQuestMasterDataPersistScript(q3, s6);
    CHECKLOG("lower(0);copy(s3);copy(s6);assign(s3<-s3);assign(s5<-s6);~(s6);~(s3);");
    CHECK(map.m_pLast - map.m_pFirst == 1 && map.m_pFirst[0].second.PersistScript.PStringData == &g_data[6] && !map.Dirty);
    // 3. a smaller key: lower bound at 0 but key < entry -> miss; pushed at the end and sorted to the front
    CCharString& q1 = Str(slotQ[1], &g_data[1]); CCharString& s2 = Str(slotS[2], &g_data[2]);
    g_log[0] = 0; rp->SetQuestMasterDataPersistScript(q1, s2);
    CHECKLOG("lower(0);copy(s1);copy(s2);copy(s1);copy(s1);copy(s2);push(s1,s1,s2);~(s2);~(s1);~(s1);sort(2);~(s2);~(s1);");
    CHECK(map.m_pLast - map.m_pFirst == 2 && map.m_pFirst[0].first.PStringData == &g_data[1] && map.m_pFirst[1].first.PStringData == &g_data[3] && !map.Dirty);
    // 4. a null key: ordered first (null < anything), miss, pushed and sorted to the front
    CCharString& qn = Str(slotQ[2], 0);
    g_log[0] = 0; rp->SetQuestMasterDataPersistScript(qn, s2);
    CHECKLOG("lower(0);copy(null);copy(s2);copy(null);copy(null);copy(s2);push(null,null,s2);~(s2);~(null);~(null);sort(3);~(s2);~(null);");
    CHECK(map.m_pLast - map.m_pFirst == 3 && map.m_pFirst[0].first.PStringData == 0);
    // 5. a null key again: hit on the null entry (same data pointer), assigned
    g_log[0] = 0; rp->SetQuestMasterDataPersistScript(qn, s6);
    CHECKLOG("lower(0);copy(null);copy(s6);assign(null<-null);assign(s2<-s6);~(s6);~(null);");
    CHECK(map.m_pFirst[0].second.PersistScript.PStringData == &g_data[6]);
    free(rp);
    if (fails == 0) printf("004b4f30_TEST PASS\n"); else printf("004b4f30_TEST FAIL (%d)\n", fails);
    return fails ? 1 : 0;
}