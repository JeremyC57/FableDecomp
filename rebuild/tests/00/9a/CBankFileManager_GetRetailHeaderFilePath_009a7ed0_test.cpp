// Behaviour test for 009a7ed0: src/009a7ed0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CBankFileManager::GetRetailHeaderFilePath(const CCharString& bank)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CBankFileManager::GetRetailHeaderFilePath(const CCharString& bank) -- resolve the bank alias, then return BasePath + the bank's RetailHeaderFilePath, or L"" when the bank is unknown or has no such path
// Leaf: GetRetailHeaderFilePath
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A7ED0, length 171 (+5 int3) bytes, ret 8 (hidden CWideString result).
// Callees: map<CCharString,CCharString>::find @ 0x009AB5D0 (BankAliasMap; catalog "BinaryTree_Search"), CCharString copy ctor
// @ 0x0099EC30, map<CCharString,CBankInfo>::find @ 0x009AB560 (Banks; catalog "map_lower_bound"), CWideString copy ctor @ 0x0099B720,
// CWideString::GetLength @ 0x0099B220, operator+(const CWideString&, const CWideString&) @ 0x0099BE70 (__fastcall),
// CWideString::~CWideString @ 0x0099B510, CWideString(const wchar_t*) @ 0x0099B6B0 on L"" @ 0x0122D70C, CCharString::~CCharString
// @ 0x0099EAE0.
// Shape: the alias lookup is an inline helper returning a const reference (alias value or the argument), so each arm
// pushes its own reference for the named copy (a ternary pushes once after a join); the path copy is homed in the dead bank-argument slot; the empty-path test falls into the L"" exit. Twin of
// GetDevHeaderFilePath 0x009A7E20.
// Layout: retail CBankFileManager (see CreateRetailBank 0x009A7720): Banks +0x04, BankAliasMap +0x18, BasePath +0x24;
// Ego_r.pdb CBankFileManager::CBankInfo (DevBankFilePath, RetailBankFilePath, DevHeaderFilePath +8,
// RetailHeaderFilePath +0xc, RegisteredBank).
struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    CWideString(const unsigned short* text);       // 0x0099B6B0 (wchar_t is unsigned short in this lane)
    CWideString(const CWideString& other);         // 0x0099B720
    ~CWideString();                                // 0x0099B510
    long GetLength() const;                        // 0x0099B220
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);   // 0x0099BE70
class CBankFile;
struct CBankInfo {                                 // CBankFileManager::CBankInfo, 0x14 bytes
    CWideString DevBankFilePath;                   // +0x00
    CWideString RetailBankFilePath;                // +0x04
    CWideString DevHeaderFilePath;                 // +0x08
    CWideString RetailHeaderFilePath;              // +0x0c
    CBankFile* RegisteredBank;                     // +0x10
};
template <class V> struct SMapNode {               // STLport tree node: color, parent, left, right, key, value
    long _M_color;
    SMapNode* _M_parent;
    SMapNode* _M_left;
    SMapNode* _M_right;
    CCharString first;                             // +0x10
    V second;                                      // +0x14
};
template <class V> struct CStringMap {             // map<CCharString, V> (STLport, 12 bytes)
    SMapNode<V>* _M_header;
    unsigned long _M_node_count;
    unsigned char _M_compare;
    SMapNode<V>* find(const CCharString& key);
    SMapNode<V>* end() const { return _M_header; }
};
class CBankFileManager {
public:
    void* _vtable;                                 // +0x00
    CStringMap<CBankInfo> Banks;                   // +0x04
    void* RetailBanks;                             // +0x10
    bool UsingRetailBanks;                         // +0x14
    CStringMap<CCharString> BankAliasMap;          // +0x18
    CWideString BasePath;                          // +0x24
    CCharString ResolveAlias(const CCharString& bank)
    {
        SMapNode<CCharString>* alias = BankAliasMap.find(bank);
        if (alias != BankAliasMap.end())
            return alias->second;
        return bank;
    }
    CWideString GetRetailHeaderFilePath(const CCharString& bank);
};
template <> SMapNode<CCharString>* CStringMap<CCharString>::find(const CCharString& key);   // 0x009AB5D0
template <> SMapNode<CBankInfo>* CStringMap<CBankInfo>::find(const CCharString& key);       // 0x009AB560

CWideString CBankFileManager::GetRetailHeaderFilePath(const CCharString& bank)
{
    CCharString name = ResolveAlias(bank);
    SMapNode<CBankInfo>* info = Banks.find(name);
    if (info != Banks.end()) {
        CWideString path(info->second.RetailHeaderFilePath);
        if (path.GetLength())
            return BasePath + path;
    }
    return CWideString(L"");
}

// ---- stubs and driver ----
#include <wchar.h>
// strings are modelled as heap copies of the source text so that copies and concatenation are observable
static int g_live = 0;
static void* dupc(const void* p) { if (!p) return 0; size_t n = strlen((const char*)p) + 1; void* q = new char[n]; memcpy(q, p, n); ++g_live; return q; }
static void* dupw(const void* p) { if (!p) return 0; size_t n = (wcslen((const wchar_t*)p) + 1) * 2; void* q = new char[n]; memcpy(q, p, n); ++g_live; return q; }
CCharString::CCharString(const CCharString& other) { PStringData = dupc(other.PStringData); }
CCharString::~CCharString() { if (PStringData) { delete[] (char*)PStringData; --g_live; } }
CWideString::CWideString(const unsigned short* text) { PStringData = dupw(text); }
CWideString::CWideString(const CWideString& other) { PStringData = dupw(other.PStringData); }
CWideString::~CWideString() { if (PStringData) { delete[] (char*)PStringData; --g_live; } }
long CWideString::GetLength() const { return PStringData ? (long)wcslen((const wchar_t*)PStringData) : 0; }
CWideString __fastcall operator+(const CWideString& a, const CWideString& b)
{
    wchar_t buf[128]; wcscpy(buf, (const wchar_t*)a.PStringData); wcscat(buf, (const wchar_t*)b.PStringData);
    return CWideString((const unsigned short*)buf);
}
static char g_lastAliasKey[32], g_lastBankKey[32];
static SMapNode<CCharString>* g_aliasHit = 0; static SMapNode<CBankInfo>* g_bankHit = 0;
template <> SMapNode<CCharString>* CStringMap<CCharString>::find(const CCharString& key) { strcpy(g_lastAliasKey, (const char*)key.PStringData); return g_aliasHit ? g_aliasHit : _M_header; }
template <> SMapNode<CBankInfo>* CStringMap<CBankInfo>::find(const CCharString& key) { strcpy(g_lastBankKey, (const char*)key.PStringData); return g_bankHit ? g_bankHit : _M_header; }
static bool weq(const CWideString& s, const wchar_t* t) { return s.PStringData ? wcscmp((const wchar_t*)s.PStringData, t) == 0 : *t == 0; }
int main()
{
    unsigned char aliasHeader[sizeof(SMapNode<CCharString>)], bankHeader[sizeof(SMapNode<CBankInfo>)];
    unsigned char aliasNode[sizeof(SMapNode<CCharString>)], bankNode[sizeof(SMapNode<CBankInfo>)];
    memset(aliasHeader, 0, sizeof(aliasHeader)); memset(bankHeader, 0, sizeof(bankHeader)); memset(aliasNode, 0, sizeof(aliasNode)); memset(bankNode, 0, sizeof(bankNode));
    SMapNode<CCharString>* an = (SMapNode<CCharString>*)aliasNode; an->second.PStringData = (void*)"REAL";
    SMapNode<CBankInfo>* bn = (SMapNode<CBankInfo>*)bankNode; bn->second.RetailHeaderFilePath.PStringData = (void*)L"banks/x.hdr";
    unsigned char raw[sizeof(CBankFileManager)]; memset(raw, 0, sizeof(raw));
    CBankFileManager* m = (CBankFileManager*)raw;
    m->Banks._M_header = (SMapNode<CBankInfo>*)bankHeader; m->BankAliasMap._M_header = (SMapNode<CCharString>*)aliasHeader;
    m->BasePath.PStringData = (void*)L"C:/";
    CCharString bank(*(CCharString*)&an->second); delete[] (char*)bank.PStringData; --g_live; bank.PStringData = dupc("ALIAS");
    // alias found and bank found: base path + header path
    g_aliasHit = an; g_bankHit = bn;
    { CWideString r = m->GetRetailHeaderFilePath(bank); CHECK(strcmp(g_lastAliasKey, "ALIAS") == 0); CHECK(strcmp(g_lastBankKey, "REAL") == 0); CHECK(weq(r, L"C:/banks/x.hdr")); }
    // no alias: the bank name itself is looked up
    g_aliasHit = 0;
    { CWideString r = m->GetRetailHeaderFilePath(bank); CHECK(strcmp(g_lastBankKey, "ALIAS") == 0); CHECK(weq(r, L"C:/banks/x.hdr")); }
    // empty header path -> empty string
    bn->second.RetailHeaderFilePath.PStringData = (void*)L"";
    { CWideString r = m->GetRetailHeaderFilePath(bank); CHECK(weq(r, L"")); }
    // unknown bank -> empty string
    g_bankHit = 0; bn->second.RetailHeaderFilePath.PStringData = (void*)L"x";
    { CWideString r = m->GetRetailHeaderFilePath(bank); CHECK(weq(r, L"")); }
    CHECK(g_live == 1);
    m->BasePath.PStringData = 0;
    if (fails == 0) printf("009a7ed0_TEST PASS\n"); else printf("009a7ed0_TEST FAIL\n");
    return fails;
}