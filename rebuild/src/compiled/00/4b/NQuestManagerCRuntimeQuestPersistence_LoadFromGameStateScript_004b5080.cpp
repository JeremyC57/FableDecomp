// NQuestManager::CRuntimeQuestPersistence::LoadFromGameStateScript(const CCharString&) -- empty both persist maps, then parse every START_NEW_QUEST <quest> <script> END_NEW_QUEST block of the game-state text into the quest-script map and every START_NEW_MASTER_QUEST_DATA block into the master-data map, re-sorting each map once if anything was added
// Leaf: LoadFromGameStateScript
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B5080, 1149 bytes (int3 padded to 1152), `ret 4`.
// Callees: ClearAll 0x004B41F0 (manifest "CQuestManager::RotateQuestPersistenceVectors" is descriptive), CStringParser ctor
//   0x00404720 and its implicit inline dtor (~list 0x004046B0, two ~CCharString, ~CBaseClass 0x0099A2E0),
//   SkipPastNextItemIfIs 0x009B9E00, ReadAsStringUntilString 0x009BA330, ReadAsStringUntilWhitespace 0x009BA4A0,
//   SkipWhitespace 0x009B9C60, NextItemIs 0x009B9BA0, CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / copy
//   0x0099EC30 / operator= 0x0099EFB0 / ~ 0x0099EAE0, the STLport _M_insert_overflow 0x004BB400 of push_back, the
//   maps' sort 0x004BBB40 (`__fastcall(first, last, compare)`).
// Layout: as the landed sibling NScript::CRuntimeScriptedEntityPersist::LoadFromGameStateScript 0x00CB8440 and the
//   landed SetQuestScriptPersistScript 0x004B4E30 (PersistScripts +0, MasterDataPersistScripts +0x10; each CVectorMap =
//   12-byte STLport vector + Compare +0xc + Dirty +0xd; element pair<CCharString, CPersistInfo> 12 bytes).
class CCharString { public:
    void* m_pData;
    CCharString();                                 // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
class CBaseClass { public:
    virtual void v00();
    virtual ~CBaseClass();                                 // 0x0099A2E0
};
struct CStringParserCommentList {
    void* m_pHead;
    ~CStringParserCommentList();                   // 0x004046B0
};
class CStringParser : public CBaseClass { public:                // retail 0x28 bytes
    char*                    String;               // +0x04
    unsigned long            BaseLineOffset;       // +0x08
    CCharString              Filename;             // +0x0c
    CCharString              TempCharString;       // +0x10
    CStringParserCommentList Comments;             // +0x14
    void (*ErrorCallback)(CCharString*);           // +0x18
    long                     CursorPos[3];         // +0x1c
    CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename,
                  long baseLineOffset);            // 0x00404720
    bool NextItemIs(const CCharString& item);              // 0x009B9BA0
    long SkipWhitespace();                                 // 0x009B9C60
    bool SkipPastNextItemIfIs(const CCharString& item);    // 0x009B9E00
    CCharString ReadAsStringUntilString(const CCharString& end);   // 0x009BA330
    CCharString ReadAsStringUntilWhitespace();             // 0x009BA4A0
    bool SkipPastText(const char* text)
    {
        CCharString item(text);
        return SkipPastNextItemIfIs(item);
    }
};
struct __false_type { __false_type() { } };
inline void* operator new(unsigned int, void* p) { return p; }

namespace NQuestManager {
struct CRuntimeQuestPersistence {
    class CPersistInfo { public:
        CCharString QuestName;                     // +0x00
        CCharString PersistScript;                 // +0x04
        CPersistInfo() { }
        __forceinline CPersistInfo(const CPersistInfo& other) : QuestName(other.QuestName), PersistScript(other.PersistScript) { }
    };
    struct SPair {                                 // pair<CCharString, CPersistInfo>, 12 bytes
        CCharString  first;                        // +0x00
        CPersistInfo second;                       // +0x04
        __forceinline SPair(const CCharString& key, const CPersistInfo& value) : first(key), second(value) { }
        __forceinline SPair(const SPair& other) : first(other.first), second(other.second) { }
    };
    struct CKeyPairCompareLess { };
    struct CPersistMap {
        SPair*              _M_start;              // +0x00
        SPair*              _M_finish;             // +0x04
        SPair*              _M_end_of_storage;     // +0x08
        CKeyPairCompareLess Compare;               // +0x0c
        bool                Dirty;                 // +0x0d
        void _M_insert_overflow(SPair* position, const SPair& value, const __false_type&, unsigned int count, bool atEnd);   // 0x004BB400
        void push_back(const SPair& value)
        {
            if (_M_finish != _M_end_of_storage) {
                if (_M_finish) new (_M_finish) SPair(value);
                ++_M_finish;
            } else
                _M_insert_overflow(_M_finish, value, __false_type(), 1, true);
        }
    };
    CPersistMap PersistScripts;                    // +0x00
    CPersistMap MasterDataPersistScripts;          // +0x10
    void ClearAll();                               // 0x004B41F0
    void LoadFromGameStateScript(const CCharString& text);
};
void __fastcall sort(CRuntimeQuestPersistence::SPair* first, CRuntimeQuestPersistence::SPair* last,
                     CRuntimeQuestPersistence::CKeyPairCompareLess comp);   // 0x004BBB40
}

void NQuestManager::CRuntimeQuestPersistence::LoadFromGameStateScript(const CCharString& text)
{
    ClearAll();
    CStringParser parser(text, 0, CCharString(""), 0);
    {
        CCharString start("START_NEW_QUEST");
        parser.ReadAsStringUntilString(start);
    }
    while (parser.SkipPastText("START_NEW_QUEST")) {
        parser.SkipWhitespace();
        CPersistInfo info;
        info.QuestName = parser.ReadAsStringUntilWhitespace();
        parser.SkipWhitespace();
        {
            CCharString end("END_NEW_QUEST");
            info.PersistScript = parser.ReadAsStringUntilString(end);
        }
        PersistScripts.push_back(SPair(info.QuestName, info));
        PersistScripts.Dirty = true;
        {
            CCharString end("END_NEW_QUEST");
            parser.NextItemIs(end);
        }
    }
    {
        CCharString start("START_NEW_MASTER_QUEST_DATA");
        parser.ReadAsStringUntilString(start);
    }
    while (parser.SkipPastText("START_NEW_MASTER_QUEST_DATA")) {
        CPersistMap& masterData = MasterDataPersistScripts;
        parser.SkipWhitespace();
        CPersistInfo info;
        info.QuestName = parser.ReadAsStringUntilWhitespace();
        parser.SkipWhitespace();
        {
            CCharString end("END_NEW_MASTER_QUEST_DATA");
            info.PersistScript = parser.ReadAsStringUntilString(end);
        }
        masterData.push_back(SPair(info.QuestName, info));
        masterData.Dirty = true;
        {
            CCharString end("END_NEW_MASTER_QUEST_DATA");
            parser.NextItemIs(end);
        }
    }
    if (PersistScripts.Dirty) {
        sort(PersistScripts._M_start, PersistScripts._M_finish, PersistScripts.Compare);
        PersistScripts.Dirty = false;
    }
    if (MasterDataPersistScripts.Dirty) {
        sort(MasterDataPersistScripts._M_start, MasterDataPersistScripts._M_finish, MasterDataPersistScripts.Compare);
        MasterDataPersistScripts.Dirty = false;
    }
}