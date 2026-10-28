// CConsole::NextCommandGuess() -- step the command guess: unless the front line still equals (case-insensitively) the lower-cased user entry, advance GuessPos through the registered inputs and show the next input whose name starts with the user entry, else leave guess mode
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EB0D0, 327 bytes (336 with padding).
// Callees: CCharString copy ctor 0x0099EC30, ToLower 0x0099ED10 (by value), dtor 0x0099EAE0, operator= 0x0099EFB0;
//          _stricmp 0x00BFEBA8, strncmp 0x00BFEAF8; the map iterator's operator++ 0x009EE470 (manifest
//          BinaryTree_FindSuccessor); CConsole::SetGuessMode 0x009EAFF0; CConsoleCommandLine(const
//          CCharString&) 0x009EEA10 and CBaseClass::~CBaseClass 0x0099A2E0; the input's virtual slot 2 (+0x08,
//          GetFormattedName, CCharString by value).
//
// Shape: both command lines hand out their entry by value (`GetEntry()`), so the user entry is copied before
// ToLower and the front entry is copied for the case-insensitive test, an inline CCharString member (the copy is
// built before either GetData; the bool is `== 0`, `neg ; sbb ; inc`). GetData
// is the inline `p ? p->PData : ""` (empty literal 0x0129AAF4); strncmp's lower-entry data and length share one
// null test. The new front line is assigned straight from the input's formatted name: the implicit conversion
// through CConsoleCommandLine(const CCharString&) builds the line temporary that the implicit operator= reads
// with `lea ecx,[esp+18h]` (an explicit `CConsoleCommandLine(...)` temporary reuses the ctor's eax instead); the
// line (string dtor, then the base dtor, no vtable re-store) and the name die at the end of the statement.
// Layout: the retail CConsole offsets of the landed rows (0x009EA660, 0x009EB260): FrontCommandLine +0x1c,
// BackCommandLine +0x2c, RegisteredConsoleInputs +0x40, GuessPos +0x68.
extern "C" int __cdecl _stricmp(const char* a, const char* b);                  // 0x00BFEBA8
extern "C" int __cdecl strncmp(const char* a, const char* b, unsigned int n);   // 0x00BFEAF8

class CCharStringData { public:
    const char*   PData;                    // +0
    unsigned long StringLength;             // +4
};
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);  // 0x0099EC30
    ~CCharString();                         // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    CCharString ToLower() const;            // 0x0099ED10
    const char* GetData() const
    {
        if (PStringData == 0)
            return "";                      // 0x0129AAF4
        return PStringData->PData;
    }
    bool IsEqualNoCase(const CCharString& other) const { return _stricmp(GetData(), other.GetData()) == 0; }
    unsigned long GetLength() const
    {
        if (PStringData == 0)
            return 0;
        return PStringData->StringLength;
    }
};
class CBaseClass { public:
    virtual void v00();                     // vtable 0x01231710
    virtual ~CBaseClass();                          // 0x0099A2E0
};
class CConsoleCommandLine : public CBaseClass { public:   // vtable 0x0129C400 (16 bytes)
    CCharString CurrentEntry;               // +0x04
    bool        UserEntry;                  // +0x08
    long        CursorPosition;             // +0x0c
    virtual void v00();
    CConsoleCommandLine(const CCharString& entry);      // 0x009EEA10
    CCharString GetEntry() const { return CurrentEntry; }
};
class CConsoleInputBase { public:
    virtual void v00();
    virtual void v01();
    virtual CCharString GetFormattedName() const;       // slot 2 (+0x08)
};
struct CInputPointer {                      // CCountedPointer<CConsoleInputBase>
    CConsoleInputBase* Object;
    void*              Info;
    CConsoleInputBase* operator->() const { return Object; }
};
struct SInputNode {                         // STLport _Rb_tree_node<pair<const CCharString, CCountedPointer<..>>>
    unsigned char  _M_color;
    SInputNode*    _M_parent;
    SInputNode*    _M_left;
    SInputNode*    _M_right;
    CCharString    first;                   // +0x10
    CInputPointer  second;                  // +0x14
};
struct CInputIterator {
    SInputNode* _M_node;
    CInputIterator& operator++();           // 0x009EE470
    SInputNode* operator->() const { return _M_node; }
};
struct CInputMap {                          // map<CCharString, CCountedPointer<CConsoleInputBase>> (12 bytes)
    SInputNode*   _M_header;
    unsigned long _M_node_count;
    unsigned char _M_compare;
};
struct CConsole_Retail {
    virtual void v00();
    unsigned char       _maps[0x18];        // +0x04 KeyBindingMap, +0x10 StringBindingMap
    CConsoleCommandLine FrontCommandLine;   // +0x1c
    CConsoleCommandLine BackCommandLine;    // +0x2c
    CCharString         HelpBarString;      // +0x3c
    CInputMap           RegisteredConsoleInputs;    // +0x40
    unsigned char       _pad4c[0x1c];       // +0x4c CommandLineHistory .. GuessMode
    CInputIterator      GuessPos;           // +0x68
    void SetGuessMode();                    // 0x009EAFF0
    void NextCommandGuess();
};

void CConsole_Retail::NextCommandGuess()
{
    CCharString userEntry = BackCommandLine.GetEntry().ToLower();
    bool unchanged = FrontCommandLine.GetEntry().IsEqualNoCase(userEntry);
    if (unchanged)
        return;
    ++GuessPos;
    if (GuessPos._M_node != RegisteredConsoleInputs._M_header
        && strncmp(GuessPos->first.GetData(), userEntry.GetData(), userEntry.GetLength()) == 0) {
        FrontCommandLine = GuessPos->second->GetFormattedName();
    } else {
        SetGuessMode();
    }
}