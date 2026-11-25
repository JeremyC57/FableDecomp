#include <cstdio>

// Behaviour test: model the three sub-object destructors with observable
// side effects and confirm _Dest_val runs them in the exact retail order
// (~CWideString on member@0x18, ~CWideString on member@0x14, ~CCharString on member@0x10).

static int g_log[8];
static int g_n = 0;

class CCharString { public: void* PStringData; ~CCharString(); };
class CWideString { public: void* PStringData; ~CWideString(); };

struct SymbolPair {
    int         m_pad[4];   // +0x00..0x0F
    CCharString ScriptName; // +0x10
    CWideString Text;       // +0x14
    CWideString Title;      // +0x18
};

static SymbolPair* g_pair;

static int tag_of(const void* p) {
    if (p == (const void*)&g_pair->Title)      return 0x18;
    if (p == (const void*)&g_pair->Text)       return 0x14;
    if (p == (const void*)&g_pair->ScriptName) return 0x10;
    return -1;
}

CCharString::~CCharString() { g_log[g_n++] = 0x100 | tag_of(this); }
CWideString::~CWideString() { g_log[g_n++] = 0x200 | tag_of(this); }

void __fastcall Dest_val_CSymbolInfo___(SymbolPair* self, int edx);

int main() {
    static double storage[4];   // raw storage: no automatic destruction of the members
    SymbolPair* self = (SymbolPair*)storage;
    g_pair = self;

    // verify reconstructed member offsets
    char* base = (char*)self;
    unsigned oS = (unsigned)((char*)&self->ScriptName - base);
    unsigned oT = (unsigned)((char*)&self->Text - base);
    unsigned oL = (unsigned)((char*)&self->Title - base);
    if (oS != 0x10 || oT != 0x14 || oL != 0x18) {
        printf("BAD_OFFSETS %x %x %x\n", oS, oT, oL);
        return 1;
    }

    g_n = 0;
    Dest_val_CSymbolInfo___(self, 0);

    if (g_n == 3 && g_log[0] == 0x218 && g_log[1] == 0x214 && g_log[2] == 0x110) {
        printf("DEST_VAL_OK\n");
        return 0;
    }
    printf("BAD_ORDER n=%d %x %x %x\n", g_n, g_log[0], g_log[1], g_log[2]);
    return 1;
}
