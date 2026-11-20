// Behaviour test for 009a7ca0: src/009a7ca0.cpp copied verbatim, the map finds and the string classes stubbed so the alias
// resolution, the found path and the not-found throw are observable. CBankFileManager::FindBank(const CCharString&)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CBankFileManager::FindBank(const CCharString& name) -- resolve a bank name through the alias map, look it up among the registered banks and return its full path (base path + bank path); an unknown bank throws CGenericException("<name> bank not found!")
// Leaf: FindBank
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A7CA0, 145 bytes (the int3 VC7.1 emits after the final noreturn call included), __fastcall, ret 8 (hidden CWideString result + name).
// Shape: the alias copy is an inline by-value ResolveAlias() helper with two returns (the two copy-constructor calls are cross-jumped, as retail).
// Callees: BankAliasMap.find 0x009AB5D0 and Banks.find 0x009AB560 (out of line, return the node), CCharString copy
// constructor 0x0099EC30, operator+(const CWideString&, const CWideString&) 0x0099BE70 (__fastcall: hidden result
// in ecx, left operand in edx, right on the stack), CCharString::~CCharString 0x0099EAE0, operator+(const
// CCharString&, const char*) 0x0099F600 (__fastcall, same convention), CCharString::operator const char*
// 0x0099E4C0, _CxxThrowException 0x00BFEB84 (ThrowInfo 0x013692F8: CGenericException : CExceptionBase, 1 byte,
// no destructor).
// Layout: Ego_r.pdb CBankFileManager with retail containers (as Close 0x009A7990): Banks map head +4,
// BankAliasMap head +0x18, BasePath (CWideString) +0x24; map nodes carry the key at +0x10 and the value at
// +0x14 (the alias string / CBankInfo, whose first member is the bank path).
// Shape: the key is a named local in the `push ecx` frame word copy-constructed from a ternary over the alias
// lookup (the pushed source is the node's value or the argument); the found path is the fall-through and
// builds the result straight into the caller's slot. The failure path builds the message temporary in the
// dead result-pointer slot and the exception object in the dead `name` slot; the TU has no /EHsc (no unwind
// frame, the key is never destroyed on the throw path).
class CWideString {
public:
    void* m_pData;
    CWideString(const CWideString& other);
    ~CWideString();
};
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);   // 0x0099BE70

class CCharString {
public:
    void* m_pData;
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
    operator const char*() const;                    // 0x0099E4C0
};
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600

class CExceptionBase {
};
class CGenericException : public CExceptionBase {
public:
    CGenericException(const char* message) {}
};

struct CBankInfo {
    CWideString Path;                                // +0x00
};

template <class K, class V>
struct CRetailMap {                                  // VC6-era map: {head, size}
    struct SNode {
        unsigned char m_colour;
        SNode* m_pParent;
        SNode* m_pLeft;
        SNode* m_pRight;
        K first;                                     // +0x10
        V second;                                    // +0x14
    };
    SNode* m_pHead;
    unsigned long m_size;
    SNode* find(const K& key);                       // Banks: 0x009AB560, BankAliasMap: 0x009AB5D0
    SNode* end() const { return m_pHead; }
};

class CBankFileManager {
public:
    void* m_00;                                      // +0x00
    CRetailMap<CCharString, CBankInfo> Banks;        // +0x04
    unsigned long m_banksCompare;                    // +0x0C
    void* RetailBanks;                               // +0x10
    bool UsingRetailBanks;                           // +0x14
    CRetailMap<CCharString, CCharString> BankAliasMap;   // +0x18
    unsigned long m_aliasCompare;                    // +0x20
    CWideString BasePath;                            // +0x24
    CCharString ResolveAlias(const CCharString& name)
    {
        CRetailMap<CCharString, CCharString>::SNode* alias = BankAliasMap.find(name);
        if (alias != BankAliasMap.end())
            return alias->second;
        return name;
    }
    CWideString FindBank(const CCharString& name);
};

CWideString CBankFileManager::FindBank(const CCharString& name)
{
    CCharString key = ResolveAlias(name);
    CRetailMap<CCharString, CBankInfo>::SNode* bank = Banks.find(key);
    if (bank != Banks.end())
        return BasePath + bank->second.Path;
    throw CGenericException(key + " bank not found!");
}

// ---- stubs ----
static char g_wide[128], g_msg[128];
CWideString::CWideString(const CWideString& o) { m_pData = o.m_pData; }
CWideString::~CWideString() {}
CWideString __fastcall operator+(const CWideString& l, const CWideString& r)
{ strcpy(g_wide, (const char*)l.m_pData); strcat(g_wide, (const char*)r.m_pData); CWideString w(l); w.m_pData = g_wide; return w; }
CCharString::CCharString(const CCharString& o) { m_pData = o.m_pData; }
CCharString::~CCharString() {}
CCharString::operator const char*() const { return (const char*)m_pData; }
CCharString __fastcall operator+(const CCharString& l, const char* r)
{ strcpy(g_msg, (const char*)l.m_pData); strcat(g_msg, r); CCharString c(l); c.m_pData = g_msg; return c; }
typedef CRetailMap<CCharString, CCharString> CAliasMap;
typedef CRetailMap<CCharString, CBankInfo> CBankMap;
static unsigned char g_aliasRaw[sizeof(CAliasMap::SNode)]; static CAliasMap::SNode& g_aliasNode = *(CAliasMap::SNode*)g_aliasRaw; static bool g_hasAlias = false; static const void* g_bankKey = 0;
static unsigned char g_bankRaw[sizeof(CBankMap::SNode)]; static CBankMap::SNode& g_bankNode = *(CBankMap::SNode*)g_bankRaw; static bool g_hasBank = true;
template<> CAliasMap::SNode* CAliasMap::find(const CCharString& key) { return g_hasAlias ? &g_aliasNode : m_pHead; }
template<> CBankMap::SNode* CBankMap::find(const CCharString& key) { g_bankKey = key.m_pData; return g_hasBank ? &g_bankNode : m_pHead; }
int main()
{
    static unsigned char ah[sizeof(CAliasMap::SNode)], bh[sizeof(CBankMap::SNode)];
    CAliasMap::SNode& aliasHead = *(CAliasMap::SNode*)ah; CBankMap::SNode& bankHead = *(CBankMap::SNode*)bh;
    static unsigned char raw[sizeof(CBankFileManager)];
    CBankFileManager* m = (CBankFileManager*)raw;
    m->BankAliasMap.m_pHead = &aliasHead; m->Banks.m_pHead = &bankHead; m->BasePath.m_pData = (void*)"base/";
    g_bankNode.second.Path.m_pData = (void*)"text.big";
    g_aliasNode.second.m_pData = (void*)"REAL_BANK";
    CCharString name(*(CCharString*)&g_aliasNode.second); name.m_pData = (void*)"ALIAS_BANK";
    // no alias: the name itself is looked up
    CWideString r1 = m->FindBank(name);
    CHECK(g_bankKey == name.m_pData); CHECK(strcmp((const char*)r1.m_pData, "base/text.big") == 0);
    // alias: the aliased name is looked up
    g_hasAlias = true;
    CWideString r2 = m->FindBank(name);
    CHECK(strcmp((const char*)g_bankKey, "REAL_BANK") == 0);
    // unknown bank: throws with the message
    g_hasBank = false; bool caught = false;
    try { m->FindBank(name); } catch (CGenericException&) { caught = true; }
    CHECK(caught); CHECK(strcmp(g_msg, "REAL_BANK bank not found!") == 0);
    if (fails == 0) printf("009a7ca0_TEST PASS\n"); else printf("009a7ca0_TEST FAIL\n");
    return fails;
}