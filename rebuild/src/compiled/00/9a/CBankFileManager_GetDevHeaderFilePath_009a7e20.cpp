// CBankFileManager::GetDevHeaderFilePath(const CCharString& bank) -- resolve the bank alias, then return BasePath + the bank's DevHeaderFilePath, or L"" when the bank is unknown or has no such path
// Leaf: GetDevHeaderFilePath
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A7E20, length 171 (+5 int3) bytes, ret 8 (hidden CWideString result).
// Callees: map<CCharString,CCharString>::find @ 0x009AB5D0 (BankAliasMap; catalog "BinaryTree_Search"), CCharString copy ctor
// @ 0x0099EC30, map<CCharString,CBankInfo>::find @ 0x009AB560 (Banks; catalog "map_lower_bound"), CWideString copy ctor @ 0x0099B720,
// CWideString::GetLength @ 0x0099B220, operator+(const CWideString&, const CWideString&) @ 0x0099BE70 (__fastcall),
// CWideString::~CWideString @ 0x0099B510, CWideString(const wchar_t*) @ 0x0099B6B0 on L"" @ 0x0122D70C, CCharString::~CCharString
// @ 0x0099EAE0.
// Shape: the alias lookup is an inline helper returning a const reference (alias value or the argument), so each arm
// pushes its own reference for the named copy (a ternary pushes once after a join); the path copy is homed in the dead bank-argument slot; the empty-path test falls into the L"" exit. Twin of
// GetRetailHeaderFilePath 0x009A7ED0.
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
    CWideString GetDevHeaderFilePath(const CCharString& bank);
};
template <> SMapNode<CCharString>* CStringMap<CCharString>::find(const CCharString& key);   // 0x009AB5D0
template <> SMapNode<CBankInfo>* CStringMap<CBankInfo>::find(const CCharString& key);       // 0x009AB560

CWideString CBankFileManager::GetDevHeaderFilePath(const CCharString& bank)
{
    CCharString name = ResolveAlias(bank);
    SMapNode<CBankInfo>* info = Banks.find(name);
    if (info != Banks.end()) {
        CWideString path(info->second.DevHeaderFilePath);
        if (path.GetLength())
            return BasePath + path;
    }
    return CWideString(L"");
}