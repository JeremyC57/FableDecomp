#include <stdio.h>

// Links against the row object: the two shared-header destructors are defined here with order tracking.
class CCharString { public: void* PStringData; ~CCharString(); };
class CWideString { public: void* PStringData; ~CWideString(); };
struct SymPair {
    char        head[0x0c];
    CCharString ScriptName;
    CWideString Text;
    CWideString Title;
    void _Dest_val_CSymbolInfo___();
};

static SymPair* g_pair;
static int g_order[3];
static int g_n = 0;
static int tag_of(const void* p) {
    if (p == (const void*)&g_pair->Title) return 0x14;
    if (p == (const void*)&g_pair->Text) return 0x10;
    if (p == (const void*)&g_pair->ScriptName) return 0x0c;
    return -1;
}
CCharString::~CCharString() { g_order[g_n++] = 0x100 | tag_of(this); }
CWideString::~CWideString() { g_order[g_n++] = 0x200 | tag_of(this); }

int main() {
    static double storage[4];              // raw storage: no automatic destruction of the members
    SymPair& sp = *(SymPair*)storage;
    g_pair = &sp;
    if ((char*)&sp.ScriptName - (char*)&sp != 0x0c || (char*)&sp.Text - (char*)&sp != 0x10 || (char*)&sp.Title - (char*)&sp != 0x14) {
        printf("BAD_OFFSETS\n");
        return 1;
    }
    g_n = 0;
    sp._Dest_val_CSymbolInfo___();
    // Expect exactly: ~CWideString(+0x14), ~CWideString(+0x10), ~CCharString(+0x0c).
    if (g_n == 3 && g_order[0] == 0x214 && g_order[1] == 0x210 && g_order[2] == 0x10c) {
        printf("DEST_VAL_ORDER_OK\n");
        return 0;
    }
    printf("DEST_VAL_ORDER_BAD n=%d %x %x %x\n", g_n, g_order[0], g_order[1], g_order[2]);
    return 1;
}
