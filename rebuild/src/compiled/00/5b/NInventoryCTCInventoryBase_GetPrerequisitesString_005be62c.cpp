// NInventory::CTCInventoryBase::GetPrerequisitesString(CDefPointer<const CQuestCardDef>) -- "Prerequisites:" plus, per renown prerequisite, "Renown:<n>(<hero renown>)"
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    CWideString(const char* text);                      // 0x0099B6B0
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    CWideString& operator+=(const CWideString& other);  // 0x0099B8D0
    static CWideString __fastcall CreateFromCharString(const CCharString& text);   // 0x0099B6A0
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);  // 0x0099BE70
CWideString __fastcall operator+(const CWideString& a, const wchar_t* b);      // 0x0099BF30
CWideString __fastcall IntToWideString(long value);                            // 0x00405F00

struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol);   // 0x009C95E0
};
struct CMainGameComponent {
    unsigned char _pad00[0x14];
    CDataBank* TextBank;                                // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0

struct SNode {
    long color; SNode* parent; SNode* left; SNode* right;
    long first;                                         // +0x10 EQuestCardPrerequisite
    long second;                                        // +0x14
};
SNode* __cdecl Successor(SNode* node);                  // 0x004292C0
struct CPrerequisiteMap {                               // STLport map<EQuestCardPrerequisite,long> (12 bytes in retail)
    SNode* header; long count; long compare;
    __forceinline bool empty() const { return count == 0; }
};
struct CDefPointeeBase { virtual void VFunc00(); virtual void Release(); long RefCount; };
struct CQuestCardDef : CDefPointeeBase {
    unsigned char _pad08[0x54];
    CPrerequisiteMap Prerequisites;                     // +0x5c
};
template<class T> struct CDefPointer {
    T* Object;
    __forceinline ~CDefPointer() { if (Object) { CDefPointeeBase* p = (CDefPointeeBase*)Object; if (--p->RefCount == 0) p->Release(); } }
    __forceinline T* operator->() const { return Object; }
};
struct CTCHeroStats {
    unsigned char _pad00[0x70];
    long RenownLevel;                                   // +0x70
};
struct CInterfaceMap {
    struct Entry { long type; void* component; };
    Entry* first; Entry* last;
    Entry* LowerBound(const long*);                     // 0x0040F020
};
struct CPresenceBits { bool test(long kind); };         // 0x00410DE0
struct CThing {
    unsigned char _pad00[0x20]; CPresenceBits present; unsigned char _pad21[0x23]; CInterfaceMap interfaces;
    template<class T> __forceinline bool GetInterface(T** out, long kind) {
        if (present.test(kind)) {
            long key = kind; CInterfaceMap* map = &interfaces; CInterfaceMap::Entry* it = map->LowerBound(&key);
            *out = (T*)(it == map->last || kind < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
namespace NInventory {
struct CTCInventoryBase {
    void* __vftable; CThing* ParentThing;
    CWideString GetPrerequisitesString(CDefPointer<const CQuestCardDef> def);
};
CWideString CTCInventoryBase::GetPrerequisitesString(CDefPointer<const CQuestCardDef> def)
{
    CWideString result = CWideString::CreateFromCharString("");
    CDataBank* bank = g_pMainGameComponent->TextBank;
    if (!def->Prerequisites.empty()) {
        result += bank->GetTextBySymbol("TEXT_GUI_MENU_PREREQUISITES") + L":";
        for (SNode* node = def->Prerequisites.header->left; node != def->Prerequisites.header; node = Successor(node)) {
            if (node->first == 0) {
                result += bank->GetTextBySymbol("TEXT_GUI_MENU_RENOWN") + L":" + IntToWideString(node->second);
                CTCHeroStats* stats;
                if (ParentThing->GetInterface(&stats, 4) && stats) {
                    long renown = stats->RenownLevel;
                    result += CWideString("(") + IntToWideString(renown) + CWideString(")");
                }
            }
        }
    }
    return result;
}
}