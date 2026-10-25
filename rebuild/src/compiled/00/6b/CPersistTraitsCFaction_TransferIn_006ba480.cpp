// CPersistTraits<CFaction>::TransferIn(CPersistContext& context, CStringParser& parser, CFaction& value) -- text persist read of a faction: "Name <string>", "NumberOfAllies <n>" then n "Ally <string>" entries into the allied set, "NumberOfEnemies <n>" then n "Enemy <string>" entries into the enemy set, then "EndFaction"; a missing keyword stops the read
// Leaf: ?TransferIn@?$CPersistTraits@VCFaction@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BA480, 584 bytes (+ int3 padding), `ret 0Ch`, ebp frame aligned to 8.
// Ego_r.pdb: ?TransferIn@?$CPersistTraits@VCFaction@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAVCFaction@@@Z;
//   CFaction { CCharString Name +0, set<CCharString> AlliedFactions +4, EnemyFactions } (retail STLport set is 12 bytes, so the
//   enemy set is at +0x10, not the PDB's +0x14).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("Name" 0x0125D5E0, "NumberOfAllies" 0x0125D5D0, "Ally"
//   0x0125D5C8, "NumberOfEnemies" 0x0125D5B8, "Enemy" 0x0125D5B0, "EndFaction" 0x0125D568), CCharString() @ 0x0099E4B0,
//   ~CCharString @ 0x0099EAE0, CStringParser::NextItemIs(const CCharString&) @ 0x009B9BA0, CPersistTraits<CCharString>::TransferIn
//   @ 0x0040E510, the __fastcall long TransferIn(CPersistContext&, CStringParser&, long*) @ 0x009E2590, set<CCharString>::insert
//   @ 0x004AC380 (pair<iterator, bool> through the hidden pointer).
// Shape: each keyword test is `if (!parser.NextItemIs("...")) return;` (implicit lea-passed temporary) (the negated result kept in bl across the
//   temporary's destructor); the CPersistTraits<CCharString> object (CCharString DefaultVal) is built after the "Name" test and
//   destroyed on every later exit; the discarded 8-byte insert result inside the loops gives the `and esp,-8` frame (GOTCHAS); each
//   count + loop is its own block, so the count's frame word is reused by the later temporaries.
struct CCharString {
    void* m_pData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CStringParser {
    bool NextItemIs(const CCharString& name);          // 0x009B9BA0
};
struct CPersistContext;
void __fastcall PersistTransferInLong(CPersistContext& context, CStringParser& parser, long* value);   // 0x009E2590

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() : first(T1()), second(T2()) { }
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
}
struct CStringSetIterator {                            // STLport _Rb_tree_iterator
    void* _M_node;
    CStringSetIterator() : _M_node(0) { }
};
struct CStringSet {                                    // STLport set<CCharString>, 12 bytes in retail
    void* _M_header;
    unsigned long _M_node_count;
    unsigned long _M_key_compare;
    std::pair<CStringSetIterator, bool> insert(const CCharString& value);   // 0x004AC380
};
class CFaction {
public:
    CCharString Name;                                  // +0x00
    CStringSet  AlliedFactions;                        // +0x04
    CStringSet  EnemyFactions;                         // +0x10
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CCharString> {
    CCharString DefaultVal;
    CPersistTraits() { }
    void TransferIn(CPersistContext* context, CStringParser* parser, CCharString* value);   // 0x0040E510
};
template <> struct CPersistTraits<CFaction> {
    void TransferIn(CPersistContext& context, CStringParser& parser, CFaction& value);
};

void CPersistTraits<CFaction>::TransferIn(CPersistContext& context, CStringParser& parser, CFaction& value)
{
    if (!parser.NextItemIs("Name"))
        return;
    CPersistTraits<CCharString> stringTraits;
    stringTraits.TransferIn(&context, &parser, &value.Name);
    {
        if (!parser.NextItemIs("NumberOfAllies"))
            return;
        long numberOfAllies;
        PersistTransferInLong(context, parser, &numberOfAllies);
        for (long i = 0; i < numberOfAllies; ++i) {
            if (!parser.NextItemIs("Ally"))
                return;
            CCharString ally;
            stringTraits.TransferIn(&context, &parser, &ally);
            value.AlliedFactions.insert(ally);
        }
    }
    {
        if (!parser.NextItemIs("NumberOfEnemies"))
            return;
        long numberOfEnemies;
        PersistTransferInLong(context, parser, &numberOfEnemies);
        for (long i = 0; i < numberOfEnemies; ++i) {
            if (!parser.NextItemIs("Enemy"))
                return;
            CCharString enemy;
            stringTraits.TransferIn(&context, &parser, &enemy);
            value.EnemyFactions.insert(enemy);
        }
    }
    if (!parser.NextItemIs("EndFaction"))
        return;
}