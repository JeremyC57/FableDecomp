// CBankFileManager::FindBank(const CCharString& name) -- resolve a bank name through the alias map, look it up among the registered banks and return its full path (base path + bank path); an unknown bank throws CGenericException("<name> bank not found!")
// Grade: DIFFER(146v144) -- (1) the alias ternary: retail pushes the copy source in each arm and shares the copy-constructor call (`je ; add eax,14h ; push eax ; jmp` / `push ebx`, `lea ecx` hoisted above the branch) where this build selects into eax and pushes once; (2) lane gap: the build emits `int3` after the noreturn _CxxThrowException (retail's trailing byte is trimmed as padding). Tried: reference ternary, an inline ResolveAlias helper, a ternary of copy temporaries (214v144).
// Leaf: FindBank
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A7CA0, 144 bytes (+16 int3), __thiscall, ret 8 (hidden CWideString result + name).
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
    CWideString FindBank(const CCharString& name);
};

CWideString CBankFileManager::FindBank(const CCharString& name)
{
    CRetailMap<CCharString, CCharString>::SNode* alias = BankAliasMap.find(name);
    CCharString key(alias != BankAliasMap.end() ? alias->second : name);
    CRetailMap<CCharString, CBankInfo>::SNode* bank = Banks.find(key);
    if (bank != Banks.end())
        return BasePath + bank->second.Path;
    throw CGenericException(key + " bank not found!");
}
