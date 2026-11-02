// Behaviour test for 0x009BDB40 ~CXMVPlayer (empty body): SubTitles cleared (each subtitle's strings and text entry released) and
// the sentinel freed, then PThreadedFile released, then ~CBaseClass.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
static char g_log[64];
static void Log(const char* s) { std::strcat(g_log, s); }
struct CSubTitle {
    CSubTitle(char c) : C(c) { }
    ~CSubTitle() { char b[3] = { 'S', C, 0 }; Log(b); }
    char C;
};
struct Node { Node* next; Node* prev; CSubTitle data; Node(char c) : data(c) { } };
class CSubTitleList {
public:
    CSubTitleList() { head = (Node*)std::malloc(sizeof(Node)); head->next = head->prev = head; }
    ~CSubTitleList()
    {
        Node* cur = head->next;
        while (cur != head) { Node* t = cur; cur = cur->next; t->data.~CSubTitle(); std::free(t); }
        std::free(head);
        Log("L");
    }
    void push_back(char c) { Node* n = new (std::malloc(sizeof(Node))) Node(c); n->next = head; n->prev = head->prev; head->prev->next = n; head->prev = n; }
    Node* head;
};
class CThreadedFile { public: int Refs; };
class CCountedPointer { public: CCountedPointer() : Data(0) { } ~CCountedPointer() { if (Data && --Data->Refs == 0) Log("F"); Data = 0; } CThreadedFile* Data; };
class CBaseClass { public: virtual ~CBaseClass() { Log("B"); } };
class CBaseClassNonCopyable : public CBaseClass { };
class CThreadedFileCallback : public CBaseClassNonCopyable { };
class CXMVPlayer : public CThreadedFileCallback {
public:
    virtual ~CXMVPlayer() { }
    char CriticalSection[0x18];
    CCountedPointer PThreadedFile;
    long FilePosition;
    CSubTitleList SubTitles;
    double TimeSinceMovieStarted;
};
int main()
{
    CThreadedFile file = { 1 };
    g_log[0] = 0;
    CXMVPlayer* player = new CXMVPlayer;
    player->SubTitles.push_back('1');
    player->SubTitles.push_back('2');
    player->PThreadedFile.Data = &file;
    delete (CBaseClass*)player;
    bool ok = std::strcmp(g_log, "S1S2LFB") == 0 && file.Refs == 0;
    std::printf(ok ? "009bdb40_TEST PASS\n" : "009bdb40_TEST FAIL\n");
    return ok ? 0 : 1;
}