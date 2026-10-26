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
class CCharStringData { public:                        // 17 bytes
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
class CDefPointeeBase { public: virtual void v00(); long m_refCount; };
class CQuestCardDef : public CDefPointeeBase { public:
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