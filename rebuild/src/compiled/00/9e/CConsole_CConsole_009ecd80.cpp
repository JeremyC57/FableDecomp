// CConsole::CConsole() -- build the developer console: binding maps, the front/back command lines, help bar,
// registered-input map, the history list and its iterators, and the default window colours
// Leaf: ??0CConsole
// Retail: Fable.exe (TLC, VC7.1) @ 0x009ECD80, 432 bytes. Catalog label "CHeroInformationScreens::Ctor" is a
// propagation: the vtable stored first is 0x0129C600 (rtti-vtables: CConsole), and the two embedded objects
// store 0x0129C400 (CConsoleCommandLine, landed ctor 0x009EA2B0 inlined here).
// Callees: malloc 0x00BFEA0E (each STLport tree header / list sentinel), CBaseClass::CBaseClass 0x0099A2D0,
//   CCharString(const char*, long) 0x0099EBF0 (the empty literal 0x0122D70E, -1), CCharString() 0x0099E4B0.
// Layout: Ego_r.pdb CConsole with the retail container shrink of the landed CConsole::SetHistoryMode 0x009EAB80
// (maps 12 bytes, list = sentinel pointer only): KeyBindingMap +0x04, StringBindingMap +0x10, FrontCommandLine +0x1c,
// BackCommandLine +0x2c, HelpBarString +0x3c, RegisteredConsoleInputs +0x40, CommandLineHistory +0x4c,
// VisibleHistoryStart/Finish +0x50/+0x54, NumberOfHistoryLinesOnscreen +0x58 (9), HistoryMode +0x5c, HistoryPos +0x60,
// MovingIntoHistory +0x64, GuessMode +0x65, GuessPos +0x68, ToggleChar +0x6c, ToggleKey +0x70, PFont +0x74,
// Active +0x78, ConsoleAlpha +0x7c, five CRGBColour {B,G,R,A} +0x80..+0x90, CursorBlink +0x94, CursorBlinkRate +0x98.
// Shape: each STLport _Rb_tree is base (header proxy zeroed, then malloc'd) + derived (count, then
// _M_empty_initialize); the list sentinel is linked to itself before it is stored.
extern "C" void* malloc(unsigned int size);         // 0x00BFEA0E

class CCharString { public:
    void* PStringData;
    CCharString();                                  // 0x0099E4B0
    CCharString(const char* text, long length);     // 0x0099EBF0
};
class CBaseClass {
public:
    virtual void v00();
    CBaseClass();                                        // 0x0099A2D0
};
class CConsoleCommandLine : public CBaseClass { public:                // vtable 0x0129C400
    virtual void v00();
    CCharString CurrentEntry;                       // +0x04
    bool        UserEntry;                          // +0x08
    long        CursorPosition;                     // +0x0c
    CConsoleCommandLine() : CBaseClass(), CurrentEntry("", -1), UserEntry(false), CursorPosition(0) { }
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

struct CRbNodeBase {
    char         Color;
    CRbNodeBase* Parent;
    CRbNodeBase* Left;
    CRbNodeBase* Right;
};
template<int ValueSize> struct CRbTreeBase {
    struct Node : CRbNodeBase { unsigned char Value[ValueSize]; };
    Node* Header;
    CRbTreeBase() : Header(0) { Header = (Node*)malloc(sizeof(Node)); }
};
struct CLessKey { };
template<int ValueSize> struct CRbTree : CRbTreeBase<ValueSize> {
    unsigned int NodeCount;
    CLessKey     KeyCompare;
    CRbTree() : NodeCount(0)
    {
        this->Header->Color = 0;
        this->Header->Parent = 0;
        this->Header->Left = this->Header;
        this->Header->Right = this->Header;
    }
};
struct CHistoryList {
    struct Node { Node* Next; Node* Prev; CConsoleCommandLine Value; };
    struct iterator { Node* p; iterator() { } explicit iterator(Node* q) : p(q) { } };
    Node* Sentinel;
    CHistoryList() : Sentinel(0)
    {
        Node* n = (Node*)malloc(sizeof(Node));
        n->Next = n;
        n->Prev = n;
        Sentinel = n;
    }
    iterator begin() { return iterator(Sentinel->Next); }
};
struct CTreeIterator { void* Node; CTreeIterator(void* n) : Node(n) { } };
class CFontBank;

class CConsole { public:
    virtual void v00();
    CRbTree<8>             KeyBindingMap;           // +0x04 map<char, CCharString>
    CRbTree<8>             StringBindingMap;        // +0x10 map<CCharString, CCharString>
    CConsoleCommandLine    FrontCommandLine;        // +0x1c
    CConsoleCommandLine    BackCommandLine;         // +0x2c
    CCharString            HelpBarString;           // +0x3c
    CRbTree<12>            RegisteredConsoleInputs; // +0x40 map<CCharString, CCountedPointer<CConsoleInputBase> >
    CHistoryList           CommandLineHistory;      // +0x4c
    CHistoryList::iterator VisibleHistoryStart;     // +0x50
    CHistoryList::iterator VisibleHistoryFinish;    // +0x54
    long                   NumberOfHistoryLinesOnscreen; // +0x58
    bool                   HistoryMode;             // +0x5c
    CHistoryList::iterator HistoryPos;              // +0x60
    bool                   MovingIntoHistory;       // +0x64
    bool                   GuessMode;               // +0x65
    CTreeIterator          GuessPos;                // +0x68
    char                   ToggleChar;              // +0x6c
    long                   ToggleKey;               // +0x70 EInputKey
    CFontBank*             PFont;                   // +0x74
    bool                   Active;                  // +0x78
    long                   ConsoleAlpha;            // +0x7c
    CRGBColour             HistoryWindowColour;     // +0x80
    CRGBColour             ClientWindowColour;      // +0x84
    CRGBColour             HelpWindowColour;        // +0x88
    CRGBColour             ScrollBarColour;         // +0x8c
    CRGBColour             TextColour;              // +0x90
    long                   CursorBlink;             // +0x94
    long                   CursorBlinkRate;         // +0x98
    CConsole();
};

CConsole::CConsole()
    : VisibleHistoryStart(CommandLineHistory.begin()),
      VisibleHistoryFinish(CommandLineHistory.begin()),
      NumberOfHistoryLinesOnscreen(9),
      HistoryMode(false),
      MovingIntoHistory(true),
      GuessMode(false),
      GuessPos(0),
      PFont(0),
      Active(false),
      ConsoleAlpha(255),
      HistoryWindowColour(60, 60, 60, 255),
      ClientWindowColour(0, 0, 0, 255),
      HelpWindowColour(60, 60, 60, 255),
      ScrollBarColour(0, 0, 0, 255),
      TextColour(255, 255, 255, 255),
      CursorBlink(0),
      CursorBlinkRate(32)
{
}