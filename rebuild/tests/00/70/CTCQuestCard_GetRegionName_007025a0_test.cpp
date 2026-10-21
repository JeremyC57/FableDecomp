// Behaviour test for 007025a0: src/007025a0.cpp copied verbatim; the card's own region name unless it is empty (null
// block or "" text) and a def exists, in which case the def's region name from the def string table.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCQuestCard::GetRegionName() const -- the quest card's region name; when none was set, the quest card def's region name from the def string table
// Inline CCharString ==(""): the null-data arm is `if (strcmp(text, "") == 0) return true; return false;`, so VC7.1 branches straight off the repe cmpsb flags (`return strcmp(text, "") == 0;` materialises a sete al).
// Leaf: GetRegionName
// Retail: Fable.exe (TLC, VC7.1) @ 0x007025A0, 129 bytes, `ret 4` (hidden CCharString result).
// Callees: CBasicString<char>::operator==(const char*) const @ 0x004115A0 (inline CCharString::operator== on a non-null block),
//   CCharString::CCharString(const CCharString&) @ 0x0099EC30, CDefStringTable::GetString(long) @ 0x009D49B0 on g_DefStringTable
//   0x013CA828, CCharString::~CCharString @ 0x0099EAE0; literal "" @ 0x0122D70E.
// Layout: PDB CTCQuestCard PDef +0x0c (CDefPointer<CQuestCardDef const>), RegionName +0x20 (CCharString); CQuestCardDef
//   RegionName +0x38 (CDefString id).
// Shape: the inline `RegionName == ""` (a null block compares "" with the literal through an inlined strcmp: `repe cmpsb`);
//   the def name is a named local copied into the result (no NRVO) and then destroyed.
#include <string.h>
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const;    // 0x004115A0
};
struct CCharStringData {                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;               // +0x00
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
struct CDefStringTable {
    CCharString GetString(long id);             // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;        // 0x013CA828
struct CDefPointeeBase { virtual void v00(); long m_refCount; };
struct CQuestCardDef : CDefPointeeBase {
    unsigned char _pad08[0x38 - 0x08];
    long RegionName;                            // +0x38 CDefString id
};
class CTCQuestCard {
public:
    virtual ~CTCQuestCard();
    void*         ParentThing;                  // +0x04
    unsigned char _pad08[0x04];
    const CQuestCardDef* PDef;                  // +0x0c CDefPointer
    bool          Finished;                     // +0x10
    bool          Replayable;                   // +0x11
    long          FeatIndex;                    // +0x14
    CCharString   QuestName;                    // +0x18
    CCharString   ObjectiveName;                // +0x1c
    CCharString   RegionName;                   // +0x20
    CCharString GetRegionName() const;
};

CCharString CTCQuestCard::GetRegionName() const
{
    if (RegionName == "") {
        if (PDef) {
            CCharString name = g_DefStringTable.GetString(PDef->RegionName);
            return name;
        }
    }
    return RegionName;
}
// ---- end source ----
static int g_copies, g_dtors; static long g_lastId;
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_copies; }
CCharString::~CCharString() { ++g_dtors; }
bool CBasicStringChar::operator==(const char* text) const { return strcmp(PData, text) == 0; }
static CCharStringData* g_defName;
CCharString CDefStringTable::GetString(long id) { g_lastId = id; return *(CCharString*)&g_defName; }
CDefStringTable g_DefStringTable;
void CDefPointeeBase::v00() {}
CTCQuestCard::~CTCQuestCard() {}
static CCharStringData MakeData(char* text) { CCharStringData d; memset(&d, 0, sizeof(d)); d.Data.PData = text; d.Data.StringLength = (unsigned long)strlen(text); return d; }
int main()
{
    static char tRegion[] = "Oakvale", tDef[] = "Bowerstone", tEmpty[] = "";
    CCharStringData dRegion = MakeData(tRegion), dDef = MakeData(tDef), dEmpty = MakeData(tEmpty);
    g_defName = &dDef;
    static long defRaw[0x40 / 4]; memset(defRaw, 0, sizeof(defRaw));
    CQuestCardDef* def = (CQuestCardDef*)defRaw; def->RegionName = 77;
    static long cardRaw[0x24 / 4]; memset(cardRaw, 0, sizeof(cardRaw));
    CTCQuestCard* card = (CTCQuestCard*)cardRaw;
    // own name set
    card->RegionName.PStringData = &dRegion; card->PDef = def;
    { CCharString r = card->GetRegionName(); CHECK(r.PStringData == &dRegion && g_lastId == 0); }
    // null block + def -> def name
    card->RegionName.PStringData = 0;
    { CCharString r = card->GetRegionName(); CHECK(r.PStringData == &dDef && g_lastId == 77); }
    // "" text + def -> def name
    g_lastId = 0; card->RegionName.PStringData = &dEmpty;
    { CCharString r = card->GetRegionName(); CHECK(r.PStringData == &dDef && g_lastId == 77); }
    // empty and no def -> own (empty) name
    g_lastId = 0; card->PDef = 0;
    { CCharString r = card->GetRegionName(); CHECK(r.PStringData == &dEmpty && g_lastId == 0); }
    card->RegionName.PStringData = 0;
    { CCharString r = card->GetRegionName(); CHECK(r.PStringData == 0 && g_lastId == 0); }
    CHECK(g_copies == g_dtors && g_copies == 7);   // every copy (5 results + 2 def-name temporaries) is destroyed
    if (fails == 0) printf("007025a0_TEST PASS\n"); else printf("007025a0_TEST FAIL\n");
    return fails;
}