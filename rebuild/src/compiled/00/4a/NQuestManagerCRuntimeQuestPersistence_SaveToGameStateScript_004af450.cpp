// NQuestManager::CRuntimeQuestPersistence::SaveToGameStateScript(CCharString& script) -- append every quest's and every quest master's persisted script data to the game-state script text
// Leaf: SaveToGameStateScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AF450, length 228 bytes (`ret 4`).
// Callees: operator+(const char*, const CCharString&) @ 0x0099F690 and operator+(const CCharString&, const char*)
// @ 0x0099F600 (both `__fastcall`, hidden result in ecx), CCharString::operator+=(const CCharString&) @ 0x0099F0A0,
// CCharString::operator+=(const char*) @ 0x0099F100, CCharString::~CCharString @ 0x0099EAE0.
// Layout: as the landed GetQuestMasterDataPersistScript 0x004B27F0 -- two CVectorMap<CCharString, CPersistInfo> of
// 16 bytes in retail (vector + comparator/dirty word): PersistScripts +0x00, MasterDataPersistScripts +0x10; each
// 12-byte element is {key, {QuestName, PersistScript}}.
// Shape: two identical loops, each entry framed as "\r\nSTART_NEW_<kind> " + QuestName + " \r\n", its script, then
// "END_NEW_<kind>\r\n". The two operator+ temporaries are destroyed outer first; the outer one is homed in the dead
// `script` argument slot (script itself lives in esi). The second loop's head is 16-aligned (speed TU).
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                                              // 0x0099EAE0
    const CCharString& operator+=(const CCharString& other);           // 0x0099F0A0
    const CCharString& operator+=(const char* text);                   // 0x0099F100
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600

namespace NQuestManager {

struct CRuntimeQuestPersistence {
    class CPersistInfo { public:
        CCharString QuestName;                     // +0x00
        CCharString PersistScript;                 // +0x04
    };
    struct SPair {
        CCharString  first;                        // +0x00
        CPersistInfo second;                       // +0x04
    };
    struct CPersistMap {                           // CVectorMap<CCharString, CPersistInfo>: a sorted vector + 1 word
        SPair* m_pFirst;
        SPair* m_pLast;
        SPair* m_pEnd;
        long   m_sorted;
    };
    CPersistMap PersistScripts;                    // +0x00
    CPersistMap MasterDataPersistScripts;          // +0x10
    void SaveToGameStateScript(CCharString& script);
};

void CRuntimeQuestPersistence::SaveToGameStateScript(CCharString& script)
{
    for (SPair* it = PersistScripts.m_pFirst; it != PersistScripts.m_pLast; ++it) {
        script += "\r\nSTART_NEW_QUEST " + it->second.QuestName + " \r\n";
        script += it->second.PersistScript;
        script += "END_NEW_QUEST\r\n";
    }
    for (SPair* it = MasterDataPersistScripts.m_pFirst; it != MasterDataPersistScripts.m_pLast; ++it) {
        script += "\r\nSTART_NEW_MASTER_QUEST_DATA " + it->second.QuestName + " \r\n";
        script += it->second.PersistScript;
        script += "END_NEW_MASTER_QUEST_DATA\r\n";
    }
}

}