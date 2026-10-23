#include <cstdio>
#include <cstring>
#if !defined(_MSC_VER)
#  define __fastcall
#  define __forceinline inline
#endif

// Behavior test for CGameScriptInterface::AddScreenTitleMessage @ 0x0089e4c0.
// When the owner lacks component 0x8e the retail target is left unset (it reads a
// dead stack slot), so that path is not exercised here.

static void* g_chain_seen; static void* g_a_seen; static void* g_A_ret; static void* g_B_ret;
static void* g_lb_this; static int g_lb_key; static void* g_lb_ret;
static void* g_d_this; static void* g_d_a0; static float g_d_a1; static bool g_d_a2; static int g_d_z, g_d_one, g_d_calls;

struct AstmTarget;
struct AstmNode { int key; AstmTarget* value; };
struct AstmMap {
    AstmNode* lb(int* pKey);
    AstmNode* first;
    AstmNode* end;
    __forceinline AstmTarget* Find(int kind)   // the inlined map find: end when the key is absent
    {
        int key = kind;
        AstmNode* node = lb(&key);
        return ((node == end || kind < node->key) ? end : node)->value;
    }
};
__forceinline AstmTarget* GetTCIfPresent(unsigned char* thing, int kind)
{
    AstmTarget* tc;                                // deliberately unset when the component is absent
    if (*(unsigned int*)(thing + 0x30) & 0x4000)
        tc = ((AstmMap*)(thing + 0x44))->Find(kind);
    return tc;
}
struct AstmTarget { void d(void* a0, float a1, bool a2, int z, int one); };
extern void* __fastcall ASTM_Helper_A(void* p_ecx);
extern void* __fastcall ASTM_Helper_B(void* p_ecx);

void __fastcall CGameScriptInterface_AddScreenTitleMessage(
    void* self_ecx,
    unsigned long,
    void* areaName,
    float fadeTime,
    bool messageFlag)
{
    void* chain = *(void**)((char*)self_ecx + 0x14);
    void* a = ASTM_Helper_A(chain);
    unsigned char* obj = (unsigned char*)ASTM_Helper_B(a);
    if (obj == 0) return;
    if (*(unsigned char*)(obj + 0x91) & 0x01) return;
    AstmTarget* target = GetTCIfPresent(obj, 0x8e);
    if (target == 0) return;
    target->d(areaName, fadeTime, messageFlag, 0, 1);
}


void* __fastcall ASTM_Helper_A(void* p_ecx) { g_chain_seen = p_ecx; return g_A_ret; }
void* __fastcall ASTM_Helper_B(void* p_ecx) { g_a_seen = p_ecx; return g_B_ret; }
AstmNode* AstmMap::lb(int* pKey) { g_lb_this = this; g_lb_key = *pKey; return (AstmNode*)g_lb_ret; }
void AstmTarget::d(void* a0, float a1, bool a2, int z, int one) {
    g_d_this = this; g_d_a0 = a0; g_d_a1 = a1; g_d_a2 = a2; g_d_z = z; g_d_one = one; ++g_d_calls;
}

static void reset() {
    g_chain_seen = g_a_seen = g_A_ret = g_B_ret = 0; g_lb_this = 0; g_lb_key = 0; g_lb_ret = 0;
    g_d_this = 0; g_d_a0 = 0; g_d_a1 = 0; g_d_a2 = false; g_d_z = g_d_one = 0; g_d_calls = 0;
}

int main() {
    static unsigned char self[0x40];
    void* chainval = (void*)0x11110000;
    *(void**)(self + 0x14) = chainval;
    static unsigned char obj[0x100];
    AstmMap* map = (AstmMap*)(obj + 0x44);
    AstmTarget targetObj;
    void* areaName = (void*)0xA000;
    int fails = 0;

    // Case 1: no owner returns after both lookup helpers.
    reset(); g_A_ret = (void*)0x22220000; g_B_ret = 0;
    CGameScriptInterface_AddScreenTitleMessage(self, 0, areaName, 0.75f, true);
    if (g_chain_seen != chainval || g_a_seen != g_A_ret || g_d_calls != 0) { printf("c1\n"); fails++; }

    // Case 2: owner byte 0x91 bit 0 disables the operation.
    reset(); std::memset(obj, 0, sizeof(obj)); obj[0x91] = 0x01; g_A_ret = (void*)0x33330000; g_B_ret = obj;
    CGameScriptInterface_AddScreenTitleMessage(self, 0, areaName, 0.75f, true);
    if (g_d_calls != 0 || g_lb_this != 0) { printf("c2\n"); fails++; }

    // Case 3: a valid lower-bound entry supplies the display; the bool is forwarded, then false and true.
    reset(); std::memset(obj, 0, sizeof(obj)); *(unsigned int*)(obj + 0x30) = 0x4000;
    AstmNode endNode = { 0, 0 }; AstmNode node = { 0x10, &targetObj };
    map->end = &endNode; g_lb_ret = &node; g_A_ret = (void*)0x55550000; g_B_ret = obj;
    CGameScriptInterface_AddScreenTitleMessage(self, 0, areaName, 0.75f, true);
    if (g_lb_this != map || g_lb_key != 0x8e || g_d_calls != 1 || g_d_this != &targetObj) { printf("c3 lookup\n"); fails++; }
    if (g_d_a0 != areaName || g_d_a1 != 0.75f || g_d_a2 != true || g_d_z != 0 || g_d_one != 1) { printf("c3 args\n"); fails++; }

    // Case 4: an entry above key 0x8e falls back to the end node.
    reset(); AstmNode endNode4 = { 0, &targetObj }; AstmNode node4 = { 0x100, 0 };
    map->end = &endNode4; g_lb_ret = &node4; g_B_ret = obj;
    CGameScriptInterface_AddScreenTitleMessage(self, 0, areaName, 0.5f, false);
    if (g_d_calls != 1 || g_d_this != &targetObj || g_d_a2 != false) { printf("c4\n"); fails++; }

    // Case 5: an end node with no display produces no call.
    reset(); AstmNode endNode5 = { 5, 0 }; map->end = &endNode5; g_lb_ret = &endNode5; g_B_ret = obj;
    CGameScriptInterface_AddScreenTitleMessage(self, 0, areaName, 0.5f, true);
    if (g_d_calls != 0) { printf("c5\n"); fails++; }

    if (fails == 0) { printf("FSE2_0089e4c0_TEST PASS\n"); return 0; }
    printf("FSE2_0089e4c0_TEST FAIL (%d)\n", fails);
    return 1;
}
