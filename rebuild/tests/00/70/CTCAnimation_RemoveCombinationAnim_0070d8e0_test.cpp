#include <cstdio>

// Behaviour model: AddChild forwards the same pointer to two subobject lists
// at offsets +0x58 and +0x64, in that order.

struct CCountedPointer { void* p; };

static int g_calls = 0;
static void* g_recv[4];
static void* g_subself[4];

struct CSubList {
    void RemoveCombinationAnim(CCountedPointer* p) {
        g_subself[g_calls] = this;
        g_recv[g_calls] = p;
        g_calls++;
    }
};

struct CTCAnimation {
    char pad[0x58];
    CSubList a;
    char pad2[0x64 - 0x58 - sizeof(CSubList)];
    CSubList b;
    void RemoveCombinationAnim(CCountedPointer* ptr) {
        this->a.RemoveCombinationAnim(ptr);
        this->b.RemoveCombinationAnim(ptr);
    }
};

int main() {
    CTCAnimation list;
    CCountedPointer cp; cp.p = (void*)0xDEADBEEF;

    list.RemoveCombinationAnim(&cp);

    bool ok = true;
    if (g_calls != 2) ok = false;
    if (g_recv[0] != &cp || g_recv[1] != &cp) ok = false;
    // subobject offsets
    if (g_subself[0] != (char*)&list + 0x58) ok = false;
    if (g_subself[1] != (char*)&list + 0x64) ok = false;

    if (ok) printf("PARITY_OK\n");
    else printf("PARITY_FAIL calls=%d\n", g_calls);
    return 0;
}