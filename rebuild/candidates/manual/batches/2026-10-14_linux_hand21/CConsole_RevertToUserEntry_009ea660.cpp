// CConsole::RevertToUserEntry() -- leaving a guess or the history restores the saved user entry into the front command line, otherwise the front line is emptied
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009EA660, length 94 bytes (+ int3 padding).
// Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0 (the two memberwise command-line
// copies), CCharString::operator=(const char*) @ 0x0099EFE0 (the "" literal at 0x0122D70E).
// Layout: Ego_r.pdb CConsole has FrontCommandLine at +0x24 and HistoryMode at +0x70, but every retail
// std::map is 12 bytes (PDB 16) and the retail std::list is the sentinel pointer only (PDB 12), so the
// shipped offsets are 0x14 lower from CommandLineHistory on: FrontCommandLine +0x1c (CurrentEntry +0x20,
// UserEntry +0x24, CursorPosition +0x28), BackCommandLine +0x2c, CommandLineHistory +0x4c, HistoryMode
// +0x5c, HistoryPos +0x60, GuessMode +0x65 -- hence the `_Retail` model. A CConsoleCommandLine is
// {vptr, CCharString CurrentEntry +4, bool UserEntry +8, long CursorPosition +0xc} (vtable 0x0129C400);
// its compiler-generated operator= copies the three members (the string through
// CCharString::operator= @ 0x0099EFB0).
// Three exits: GuessMode (tested first) and HistoryMode each copy BackCommandLine into
// FrontCommandLine (compiler-generated operator=: string assignment, then the bool and the cursor)
// and clear their own flag; the fall-through resets the front line. The zero register and the front
// line's address are hoisted above the first test.
struct CCharString {
    void* PStringData;
    CCharString& operator=(const CCharString& other);           // 0x0099EFB0
    const CCharString& operator=(const char* text);             // 0x0099EFE0
};

struct CBaseClass {
    virtual void v00();                                         // vtable 0x01231710
};

struct CConsoleCommandLine : CBaseClass {                       // vtable 0x0129C400 (16 bytes)
    CCharString CurrentEntry;                                   // +0x04
    bool        UserEntry;                                      // +0x08
    long        CursorPosition;                                 // +0x0c
    virtual void v00();
    void Reset()                                                // an empty, non-user entry with the cursor at the start
    {
        CurrentEntry = "";                                      // the shared empty literal 0x0122D70E
        UserEntry = false;
        CursorPosition = 0;
    }
};

template<class T> struct CRetailList {                          // retail std::list: the sentinel pointer only, node = {next, prev, value}
    struct node { node* next; node* prev; T value; };
    node* head;
    struct iterator {
        node* p;
        iterator() { }
        explicit iterator(node* q) : p(q) { }
        T& operator*() const { return p->value; }
        T* operator->() const { return &p->value; }
        iterator& operator++() { p = p->next; return *this; }
        iterator& operator--() { p = p->prev; return *this; }
        bool operator!=(const iterator& o) const { return p != o.p; }
        bool operator==(const iterator& o) const { return p == o.p; }
    };
    iterator begin() { return iterator(head->next); }
    iterator end() { return iterator(head); }
};

struct CConsole_Retail {                                        // retail offsets (Ego_r.pdb minus the container shrink, see the header comment)
    virtual void v00();
    unsigned char       _maps[0x18];                            // +0x04 KeyBindingMap, +0x10 StringBindingMap (12 bytes each)
    CConsoleCommandLine FrontCommandLine;                       // +0x1c
    CConsoleCommandLine BackCommandLine;                        // +0x2c
    CCharString         HelpBarString;                          // +0x3c
    unsigned char       _registered[0x0c];                      // +0x40 RegisteredConsoleInputs
    CRetailList<CConsoleCommandLine>           CommandLineHistory;    // +0x4c
    CRetailList<CConsoleCommandLine>::iterator VisibleHistoryStart;   // +0x50
    CRetailList<CConsoleCommandLine>::iterator VisibleHistoryFinish;  // +0x54
    long                NumberOfHistoryLinesOnscreen;           // +0x58
    bool                HistoryMode;                            // +0x5c
    CRetailList<CConsoleCommandLine>::iterator HistoryPos;      // +0x60
    bool                MovingIntoHistory;                      // +0x64
    bool                GuessMode;                              // +0x65

    void RevertToUserEntry();
};

void CConsole_Retail::RevertToUserEntry()
{
    if (GuessMode) {
        FrontCommandLine = BackCommandLine;
        GuessMode = false;
    }
    else if (HistoryMode) {
        FrontCommandLine = BackCommandLine;
        HistoryMode = false;
    }
    else {
        FrontCommandLine.Reset();
    }
}
