// Behaviour test for 0x0045228F CPersistContext::Transfer<CDefString>(CDefString&) (NF415: linked): links the row object (the
// specialisation is declared here, not defined) and defines its callees under the row's spellings. Checks the CheckCRC key is
// retail's pooled "" (0x0122D70E), then the mode switch (2: load the value from +0x24, 3: write it to the stream at +0x28).
#include <cstdio>
#include <cstring>

class CDefString;
class CPersistContext { public:
    char pad[0x18];
    int state;      /* +0x18 */
    char pad2[0x24-0x1c];
    int field24;    /* +0x24 */
    void* field28;  /* +0x28 */
    template <class T> void Transfer(T& value);
};
class CDataOutputStream { public: public:
    void WriteSLONG(long);
};
struct CShadowSceneB { void Put(int v); };
extern void __stdcall PrepShadow(int tag);
// The row's specialisation Transfer<CDefString> is reached through the declared (undefined) member template: the call below names
//   ??$Transfer@VCDefString@@@CPersistContext@@QAEXAAVCDefString@@@Z, which the linked row object defines.

static int g_prep = 0, g_bad = 0;
static int g_write_count = 0, g_put_count = 0;
static long g_write_v = 0; static int g_put_v = 0;
static void* g_write_this = 0;
static void* g_put_this = 0;
void CDataOutputStream::WriteSLONG(long v) { g_write_count++; g_write_v = v; g_write_this = this; }
void CShadowSceneB::Put(int v) { g_put_count++; g_put_v = v; g_put_this = this; }
void __stdcall PrepShadow(int tag) { g_prep++; const char* k = (const char*)tag; if (k == 0 || std::strcmp(k, "") != 0) ++g_bad; }

int main() {
    CPersistContext m;
    m.field24 = 77;
    CDataOutputStream stream;
    m.field28 = &stream;
    int arg = 999;
    m.state = 2; g_prep = 0; g_write_count = g_put_count = 0; g_put_v = -1; g_put_this = 0;
    m.Transfer<CDefString>(*(CDefString*)&arg);
    if (!(g_prep == 1 && g_put_count == 1 && g_write_count == 0 && g_put_v == 77 && g_put_this == (void*)&arg)) { std::printf("FAIL state2\n"); return 1; }
    m.state = 3; g_prep = 0; g_write_count = g_put_count = 0; g_write_v = -1; g_write_this = 0;
    m.Transfer<CDefString>(*(CDefString*)&arg);
    if (!(g_prep == 1 && g_write_count == 1 && g_put_count == 0 && g_write_v == 999 && g_write_this == (void*)&stream)) { std::printf("FAIL state3\n"); return 1; }
    m.state = 9; g_prep = 0; g_write_count = g_put_count = 0;
    m.Transfer<CDefString>(*(CDefString*)&arg);
    if (!(g_prep == 1 && g_write_count == 0 && g_put_count == 0)) { std::printf("FAIL state9\n"); return 1; }
    if (g_bad) { std::printf("FAIL key is not the empty string\n"); return 1; }
    std::printf("CEngineLightingManager_0045228f_TEST PASS\n");
    return 0;
}
