#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
#define free test_free
#define malloc test_malloc
// Behaviour test for 009a7b70: the reconstructed source below (copied verbatim from src/) runs against stub callees.
// CBankFileManager::~CBankFileManager() -- close every bank, then destroy the created-bank set, the base path, the bank alias map, the retail bank list and the bank map in reverse order, and the CBaseClass base
// Leaf: ??1CBankFileManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A7B70, length 229 bytes. Called by the deleting destructor 0x009AC870 (slot 0
// of CBankFileManager's vtable 0x0129B018) and 0x0122A6A5 (static teardown); the catalog label
// "CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip" is propagated.
// Callees: CBankFileManager::Close @ 0x009A7990 (landed), the tree erase helpers @ 0x009AA880 (CreatedRetailBanks),
//          @ 0x009AA830 (BankAliasMap) and @ 0x009AA7C0 (Banks) (landed CRetailTree_erase* family), ~CWideString @ 0x0099B510,
//          the RetailBanks list clear @ 0x009AAE50 (label "remove_if" propagated), free @ 0x00BFEA14 (x4),
//          ~CBaseClass @ 0x0099A2E0 (tail jump).
// Layout: Ego_r.pdb CBankFileManager with the retail containers (as the landed Close 0x009A7990): Banks +0x04 (12-byte map),
// RetailBanks +0x10 (one-word list), UsingRetailBanks +0x14, BankAliasMap +0x18, BasePath +0x24, CreatedRetailBanks +0x28,
// FileVersion +0x34.
// Shape: the destructor body is Close(); every map destructor is inline (erase from the root when non-empty, reset the
// header through the re-read header pointer, free it), the zero kept in ebx; the list destructor is its out-of-line clear
// plus the inline header free.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

class CBaseClass {
public:
    virtual void v00();
    ~CBaseClass();                                          // 0x0099A2E0
};

class CWideString {
public:
    void* PStringData;
    ~CWideString();                                         // 0x0099B510
};

struct CRetailTree_erase009aa7c0 {                          // map<CCharString, CBankFileManager::CBankInfo>
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; };
    node* head;
    unsigned long size;
    unsigned long _comp;
    void Erase(node* root);                                 // 0x009AA7C0
    ~CRetailTree_erase009aa7c0()
    {
        if (size) {
            Erase(head->parent);
            head->left = head;
            head->parent = 0;
            head->right = head;
            size = 0;
        }
        if (head)
            free(head);
    }
};
struct CRetailTree_erase009aa830 {                          // map<CCharString, CCharString>
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; };
    node* head;
    unsigned long size;
    unsigned long _comp;
    void Erase(node* root);                                 // 0x009AA830
    ~CRetailTree_erase009aa830()
    {
        if (size) {
            Erase(head->parent);
            head->left = head;
            head->parent = 0;
            head->right = head;
            size = 0;
        }
        if (head)
            free(head);
    }
};
struct CRetailTree_erase009aa880 {                          // set<CWideString>
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; };
    node* head;
    unsigned long size;
    unsigned long _comp;
    void Erase(node* root);                                 // 0x009AA880
    ~CRetailTree_erase009aa880()
    {
        if (size) {
            Erase(head->parent);
            head->left = head;
            head->parent = 0;
            head->right = head;
            size = 0;
        }
        if (head)
            free(head);
    }
};

struct CRetailBankList {                                    // list<CCountedPointer<NBankFileManager::CRetailBank>> (one word)
    void* m_pNode;
    void clear_009aae50();                                  // 0x009AAE50
    ~CRetailBankList()
    {
        clear_009aae50();
        if (m_pNode)
            free(m_pNode);
    }
};

class CBankFileManager : public CBaseClass {                // vtable 0x0129B018
public:
    CRetailTree_erase009aa7c0 Banks;                        // +0x04
    CRetailBankList           RetailBanks;                  // +0x10
    bool                      UsingRetailBanks;             // +0x14
    CRetailTree_erase009aa830 BankAliasMap;                 // +0x18
    CWideString               BasePath;                     // +0x24
    CRetailTree_erase009aa880 CreatedRetailBanks;           // +0x28
    unsigned long             FileVersion;                  // +0x34
    void Close();                                           // 0x009A7990
    virtual ~CBankFileManager();
};

CBankFileManager::~CBankFileManager()
{
    Close();
}

// ---- stubs and driver ----
static char g_log[1024];
static char g_num[64];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }
static bool g_armed = false;
static void* g_freed[32];
static int g_nfreed = 0;
extern "C" void test_free(void* p) { if (g_armed && g_nfreed < 32) g_freed[g_nfreed++] = p; }
static bool Freed(void* p) { for (int i = 0; i < g_nfreed; ++i) if (g_freed[i] == p) return true; return false; }
static long g_dummy[64];
#define FAKE(i) ((void*)&g_dummy[i])
void CBaseClass::v00() {}
CBaseClass::~CBaseClass() { Log("BaseClass"); }
CWideString::~CWideString() { Log("BasePath"); }
void CRetailTree_erase009aa7c0::Erase(node*) { Log("EBanks"); }
void CRetailTree_erase009aa830::Erase(node*) { Log("EAlias"); }
void CRetailTree_erase009aa880::Erase(node*) { Log("ECreated"); }
void CRetailBankList::clear_009aae50() { Log("RetailClear"); }
void CBankFileManager::Close() { Log("Close"); }
int main()
{
    static CRetailTree_erase009aa7c0::node h1;
    static CRetailTree_erase009aa830::node h2;
    static CRetailTree_erase009aa880::node h3;
    static unsigned char buf[sizeof(CBankFileManager)];
    CBankFileManager* m = (CBankFileManager*)buf;
    m->Banks.head = &h1; m->Banks.size = 0;
    m->BankAliasMap.head = &h2; m->BankAliasMap.size = 2;
    m->CreatedRetailBanks.head = &h3; m->CreatedRetailBanks.size = 1;
    m->RetailBanks.m_pNode = FAKE(5);
    g_armed = true;
    m->CBankFileManager::~CBankFileManager();
    g_armed = false;
    bool ok = std::strcmp(g_log, "Close,ECreated,BasePath,EAlias,RetailClear,BaseClass,") == 0 && g_nfreed == 4 &&
              g_freed[0] == &h3 && g_freed[1] == &h2 && g_freed[2] == FAKE(5) && g_freed[3] == &h1 && h2.left == &h2 && h2.right == &h2;
    if (!ok) { std::printf("009a7b70_TEST FAIL\n"); return 1; }
    std::printf("009a7b70_TEST PASS\n");
    return 0;
}