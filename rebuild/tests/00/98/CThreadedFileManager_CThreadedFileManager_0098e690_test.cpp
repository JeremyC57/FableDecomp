// Behaviour test for 0x0098E690 CThreadedFileManager::~CThreadedFileManager: Uninitialise() first, then the Objects tree and
// the 14 DeviceQueue trees (last first) are cleared -- every node freed through the right-recursive / left-iterative erase,
// headers relinked and freed -- then the base dtor.
#include <cstdio>
static int g_log[64]; static int g_n = 0; static int g_freed = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }
struct Node { bool Color; Node* Parent; Node* Left; Node* Right; };
struct Tree {
    Node* Header; unsigned Count; int Tag;
    void Erase(Node* x) { while (x) { Erase(x->Right); Node* y = x->Left; ++g_freed; delete x; x = y; } }
    ~Tree() { if (Count != 0) { Erase(Header->Parent); Header->Left = Header; Header->Parent = 0; Header->Right = Header; Count = 0; } Log(Tag); delete Header; }
};
struct CPendingObjectMap { Tree Objects; void* CurrentPosition; };
struct CBaseClassNonCopyable { virtual ~CBaseClassNonCopyable() { Log(9999); } };
class CThreadedFileManager : public CBaseClassNonCopyable { public:
    CPendingObjectMap DeviceQueue[2][7]; Tree Objects;
    void Uninitialise() { Log(1); }
    virtual ~CThreadedFileManager() { Uninitialise(); }
};
static Node* Chain(int n, Node* parent)
{
    if (n == 0) return 0;
    Node* x = new Node; x->Parent = parent; x->Left = Chain(n / 2, x); x->Right = Chain(n - 1 - n / 2, x); return x;
}
static void Init(Tree& t, int n, int tag)
{
    t.Header = new Node; t.Count = n; t.Tag = tag;
    t.Header->Parent = Chain(n, t.Header); t.Header->Left = t.Header; t.Header->Right = t.Header;
}
int main()
{
    {
        CThreadedFileManager* m = new CThreadedFileManager;
        for (int i = 0; i < 2; ++i) for (int j = 0; j < 7; ++j) Init(m->DeviceQueue[i][j].Objects, i * 7 + j, 100 + i * 7 + j);
        Init(m->Objects, 5, 50);
        delete m;
    }
    if (g_n != 17 || g_log[0] != 1 || g_log[1] != 50 || g_log[16] != 9999) { std::printf("0098e690_TEST FAIL\n"); return 1; }
    for (int k = 0; k < 14; ++k) if (g_log[2 + k] != 113 - k) { std::printf("0098e690_TEST FAIL\n"); return 1; }
    if (g_freed != 5 + 91) { std::printf("0098e690_TEST FAIL\n"); return 1; }
    std::printf("0098e690_TEST PASS\n");
    return 0;
}