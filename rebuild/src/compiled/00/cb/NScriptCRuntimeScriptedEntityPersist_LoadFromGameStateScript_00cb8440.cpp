// NScript::CRuntimeScriptedEntityPersist::LoadFromGameStateScript(const CCharString&) -- reset PersistScripts, then parse every START_NEW_ENTITY_SCRIPT <id> <script> END_NEW_ENTITY_SCRIPT block of the game-state text into it, re-sorting the map once if anything was added
// Leaf: LoadFromGameStateScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB8440, length 580 bytes, ret 4 (called by the landed
// CScriptBase::LoadGameState 0x00CB8690).
// Callees: vector erase(first, last) 0x00CBBDA0 and CVectorMap::Clear 0x00CBC660 (the inline Reset, out of line at
// 0x00CB80F0), CStringParser ctor 0x00404720 and its implicit inline dtor (~list 0x004046B0, two ~CCharString,
// ~CBaseClass 0x0099A2E0), SkipPastNextItemIfIs 0x009B9E00, ReadAsStringUntilString 0x009BA330, SkipUntilWhitespace
// 0x009B9CE0, ReadInt64 0x009BA5B0, SkipWhitespace 0x009B9C60, NextItemIs 0x009B9BA0, CCharString() 0x0099E4B0 /
// (const char*, long) 0x0099EBF0 / copy 0x0099EC30 / operator= 0x0099EFB0 / dtor 0x0099EAE0, the STLport
// _M_insert_overflow 0x00CBC130 of push_back, the map's sort 0x00CBD370 (__fastcall(first, last, compare)).
// Literals: "" 0x0122D70E, "START_NEW_ENTITY_SCRIPT" 0x012C1674, "END_NEW_ENTITY_SCRIPT" 0x012C165C.
// Layout: Ego_r.pdb CRuntimeScriptedEntityPersist (20 bytes) = PersistScripts CVectorMap<unsigned __int64, CPersistInfo>:
// vector (retail 12 bytes) +0, Compare +0x0c, Dirty +0x0d; element pair<unsigned __int64, CPersistInfo{UID, PersistScript}>
// is 0x18 bytes.
class CCharString { public:
    void* m_pData;
    CCharString();                                 // 0x0099E4B0
    CCharString(const char* text, long length);    // 0x0099EBF0
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
class CBaseClass { public:
    virtual void v00();
    ~CBaseClass();                                 // 0x0099A2E0
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
    long SkipUntilWhitespace();                            // 0x009B9CE0
    bool SkipPastNextItemIfIs(const CCharString& item);    // 0x009B9E00
    CCharString ReadAsStringUntilString(const CCharString& end);   // 0x009BA330
    unsigned __int64 ReadInt64();                          // 0x009BA5B0
    void SkipToText(const char* text)
    {
        CCharString end(text, -1);
        ReadAsStringUntilString(end);
    }
    void ReadUntilText(CCharString& out, const char* text)
    {
        CCharString end(text, -1);
        out = ReadAsStringUntilString(end);
    }
    bool NextItemIsText(const char* text)
    {
        CCharString item(text, -1);
        return NextItemIs(item);
    }
    bool SkipPastText(const char* text)
    {
        CCharString item(text, -1);
        return SkipPastNextItemIfIs(item);
    }
};
extern const char g_emptyString_0122d70e[];       // 0x0122D70E ""
extern const char g_startNewEntityScript[];       // 0x012C1674 "START_NEW_ENTITY_SCRIPT"
extern const char g_endNewEntityScript[];         // 0x012C165C "END_NEW_ENTITY_SCRIPT"

struct __false_type { __false_type() { } };
inline void* operator new(unsigned int, void* p) { return p; }

namespace NScript {
struct CRuntimeScriptedEntityPersist {
    class CPersistInfo { public:
        unsigned __int64 UID;                       // +0x00
        CCharString      PersistScript;             // +0x08
    };
    struct CEntry {                                 // pair<unsigned __int64, CPersistInfo>, 0x18 bytes
        unsigned __int64 first;
        CPersistInfo     second;
        CEntry(const unsigned __int64& id, const CPersistInfo& info) : first(id), second(info) { }
    };
    struct CCompare { };
    template <class T> struct CVector {
        T* _M_start;
        T* _M_finish;
        T* _M_end_of_storage;
        T* erase(T* first, T* last);                                          // 0x00CBBDA0
        void _M_insert_overflow(T* position, const T& value, const __false_type&, unsigned int count, bool atEnd);   // 0x00CBC130 (same-TU stand-in below)
        void push_back(const T& value)
        {
            if (_M_finish != _M_end_of_storage) {
                if (_M_finish) new (_M_finish) T(value);
                ++_M_finish;
            } else
                _M_insert_overflow(_M_finish, value, __false_type(), 1, true);
        }
    };
    struct CMap {
        CVector<CEntry> Data;                              // +0x00
        CCompare Compare;                           // +0x0c
        bool     Dirty;                             // +0x0d
        void Clear();                               // 0x00CBC660
    };
    CMap PersistScripts;

    void LoadFromGameStateScript(const CCharString& text);
};
#pragma auto_inline(off)
template <class T> __declspec(noinline) void CRuntimeScriptedEntityPersist::CVector<T>::_M_insert_overflow(T* position, const T& value, const __false_type&, unsigned int count, bool atEnd)
{
    if (atEnd && value.first != 0)
        _M_finish = position + count;
}
#pragma auto_inline(on)
void __fastcall SortEntries(CRuntimeScriptedEntityPersist::CEntry* first, CRuntimeScriptedEntityPersist::CEntry* last, CRuntimeScriptedEntityPersist::CCompare compare);   // 0x00CBD370
}

void NScript::CRuntimeScriptedEntityPersist::LoadFromGameStateScript(const CCharString& text)
{
    PersistScripts.Data.erase(PersistScripts.Data._M_start, PersistScripts.Data._M_finish);
    PersistScripts.Clear();
    CStringParser parser(text, 0, CCharString(g_emptyString_0122d70e, -1), 0);
    {
        CCharString start(g_startNewEntityScript, -1);
        parser.ReadAsStringUntilString(start);
    }
    while (parser.SkipPastText(g_startNewEntityScript)) {
        parser.SkipUntilWhitespace();
        CPersistInfo info;
        info.UID = parser.ReadInt64();
        parser.SkipWhitespace();
        {
            CCharString end(g_endNewEntityScript, -1);
            info.PersistScript = parser.ReadAsStringUntilString(end);
        }
        PersistScripts.Data.push_back(CEntry(info.UID, info));
        PersistScripts.Dirty = true;
        {
            CCharString end(g_endNewEntityScript, -1);
            parser.NextItemIs(end);
        }
    }
    if (PersistScripts.Dirty) {
        SortEntries(PersistScripts.Data._M_start, PersistScripts.Data._M_finish, PersistScripts.Compare);
        PersistScripts.Dirty = false;
    }
}