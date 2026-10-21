// Behaviour test for CConsole::CConsole @ 0x009ECD80: empty trees (header coloured red, self-linked), empty
// history list with both visible iterators at begin(), and the default scalar/colour settings.
#include <cstdio>
#include <cstdlib>
struct CCharString { const char* text; CCharString() : text("") { } CCharString(const char* t, long) : text(t) { } };
struct CommandLine { CCharString CurrentEntry; bool UserEntry; long CursorPosition; CommandLine() : CurrentEntry("", -1), UserEntry(false), CursorPosition(0) { } };
struct Colour { unsigned char B, G, R, A; Colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; } };
struct NodeBase { char Color; NodeBase* Parent; NodeBase* Left; NodeBase* Right; };
template<int N> struct Tree {
    struct Node : NodeBase { unsigned char Value[N]; };
    Node* Header; unsigned NodeCount;
    Tree() : Header(0) { Header = (Node*)std::malloc(sizeof(Node)); NodeCount = 0; Header->Color = 0; Header->Parent = 0; Header->Left = Header; Header->Right = Header; }
};
struct List {
    struct Node { Node* Next; Node* Prev; CommandLine Value; };
    Node* Sentinel;
    List() : Sentinel(0) { Node* n = (Node*)std::malloc(sizeof(Node)); n->Next = n; n->Prev = n; Sentinel = n; }
    Node* begin() { return Sentinel->Next; }
};
struct Console {
    Tree<8> Key, Str; CommandLine Front, Back; CCharString Help; Tree<12> Inputs; List History;
    List::Node* VisStart; List::Node* VisFinish; long Lines; bool HistoryMode; bool Moving; bool Guess; void* GuessPos;
    void* PFont; bool Active; long Alpha; Colour Hist, Client, HelpC, Scroll, Text; long Blink, BlinkRate;
    Console() : VisStart(History.begin()), VisFinish(History.begin()), Lines(9), HistoryMode(false), Moving(true), Guess(false),
        GuessPos(0), PFont(0), Active(false), Alpha(255), Hist(60, 60, 60, 255), Client(0, 0, 0, 255), HelpC(60, 60, 60, 255),
        Scroll(0, 0, 0, 255), Text(255, 255, 255, 255), Blink(0), BlinkRate(32) { }
};
int main()
{
    Console c;
    bool ok = c.Key.NodeCount == 0 && c.Key.Header->Left == c.Key.Header && c.Key.Header->Right == c.Key.Header && c.Key.Header->Parent == 0
        && c.Inputs.Header->Color == 0 && c.History.Sentinel->Next == c.History.Sentinel && c.VisStart == c.History.Sentinel
        && c.VisFinish == c.History.Sentinel && c.Lines == 9 && !c.HistoryMode && c.Moving && !c.Guess && c.Alpha == 255
        && c.Hist.R == 60 && c.Hist.B == 60 && c.Hist.A == 255 && c.Client.G == 0 && c.Text.B == 255 && c.BlinkRate == 32
        && !c.Front.UserEntry && c.Back.CursorPosition == 0;
    if (!ok) { std::printf("009ecd80_TEST FAIL\n"); return 1; }
    std::printf("009ecd80_TEST PASS\n");
    return 0;
}
