// CBankFileManager::CloseRetailBank(const CWideString& name) -- drop the open retail bank of that name from the RetailBanks list (first match by name)
// Leaf: CloseRetailBank
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A78D0, length 185 bytes (+7 int3), ret 4.
// Callees: CWideString::ConvertToCharString() @ 0x0099B2C0 (by value), strcmp @ 0x00BFEBA8 (a real call:
// `#pragma function(strcmp)`), operator delete @ 0x00BFE9BC (count block), free @ 0x00BFEA14 (list node),
// CCharString::~CCharString @ 0x0099EAE0. "" @ 0x0129AAF4.
// Shape: the converted name is homed in the dead argument slot; CCharString's inline c_str() is written null-first
// (`p == 0 ? "" : p->Text`: the `p ? p->Text : ""` form flips the branch layout); the list end is re-read
// after each strcmp; the inline STLport erase unlinks the node, runs the counted pointer's dtor (count re-read through
// the member) and frees the node; the match breaks out to the shared name-dtor tail.
// Layout: retail CBankFileManager (see CreateRetailBank 0x009A7720): RetailBanks +0x10 (list<CCountedPointer<
// NBankFileManager::CRetailBank>>, 4 bytes); CRetailBank::FilePath (CCharString) +0x24.
extern "C" int __cdecl strcmp(const char* a, const char* b);   // 0x00BFEBA8
#pragma function(strcmp)
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
void __cdecl operator delete(void* p);             // 0x00BFE9BC
extern const char g_emptyString[];                 // "" @ 0x0129AAF4

struct CCharStringData { const char* Text; };
struct CCharString {
    CCharStringData* PStringData;
    ~CCharString();                                // 0x0099EAE0
    const char* c_str() const { return PStringData == 0 ? g_emptyString : PStringData->Text; }
};
struct CWideString {
    void* PStringData;
    CCharString ConvertToCharString() const;       // 0x0099B2C0
};
struct SCountBlock {
    long  m_references;                            // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
namespace NBankFileManager {
struct CRetailBank {
    unsigned char _pad[0x24];
    CCharString FilePath;                          // +0x24 (PDB +0x28: the retail map is 12 bytes)
};
}
struct CRetailBankPointer {                        // CCountedPointer<NBankFileManager::CRetailBank>
    NBankFileManager::CRetailBank* Data;
    SCountBlock* Info;
    __forceinline ~CRetailBankPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct SRetailBankNode {                           // STLport list node, 0x10 bytes
    SRetailBankNode* _M_next;
    SRetailBankNode* _M_prev;
    CRetailBankPointer _M_data;
};
struct CRetailBankList {                           // list<CCountedPointer<CRetailBank>> (STLport, 4 bytes)
    SRetailBankNode* _M_node;
    SRetailBankNode* begin() const { return _M_node->_M_next; }
    SRetailBankNode* end() const { return _M_node; }
    void erase(SRetailBankNode* n)
    {
        SRetailBankNode* next = n->_M_next;
        SRetailBankNode* prev = n->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        n->_M_data.~CRetailBankPointer();
        free(n);
    }
};

class CBankFileManager {
public:
    unsigned char _pad[0x10];
    CRetailBankList RetailBanks;                   // +0x10
    void CloseRetailBank(const CWideString& name);
};

void CBankFileManager::CloseRetailBank(const CWideString& name)
{
    CCharString bankName = name.ConvertToCharString();
    for (SRetailBankNode* it = RetailBanks.begin(); it != RetailBanks.end(); it = it->_M_next) {
        if (strcmp(bankName.c_str(), it->_M_data.Data->FilePath.c_str()) == 0) {
            RetailBanks.erase(it);
            break;
        }
    }
}