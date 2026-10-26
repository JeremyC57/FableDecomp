// NInventory::CTCInventoryBase::GetPrerequisitesString(CTCQuestCard*) -- per renown prerequisite of the card's def, "Renown: <n> (Current level <hero renown>)"
// Leaf: GetPrerequisitesString
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
#include "engine/CTCQuestCard.h"
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString { public:
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

namespace NGameText {
class CDataBank { public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
}
using NGameText::CDataBank;
class CMainGameComponent { public:
    unsigned char _pad00[0x14];
    CDataBank* TextBank;                                // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
__forceinline CDataBank* GetTextBank() { return g_pMainGameComponent->TextBank; }

struct SNode {
    long color; SNode* parent; SNode* left; SNode* right;
    long first;                                         // +0x10 EQuestCardPrerequisite
    long second;                                        // +0x14
};
SNode* __cdecl Successor(SNode* node);                  // 0x004292C0
struct CPrerequisiteMap {                               // STLport map<EQuestCardPrerequisite,long> (12 bytes in retail)
    SNode* header; long count; long compare;
    __forceinline SNode* begin() const { return header->left; }
    __forceinline SNode* end() const { return header; }
    __forceinline bool empty() const { return count == 0; }
};
class CDefPointeeBase { public: virtual void VFunc00(); virtual void Release(); long RefCount; };
class CQuestCardDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x54];
    CPrerequisiteMap Prerequisites;                     // +0x5c
};
template<class T> class CDefPointer { public:
    T* Object;
    __forceinline CDefPointer(T* p) : Object(p) { if (Object) ++((CDefPointeeBase*)Object)->RefCount; }
    __forceinline ~CDefPointer() { if (Object) { CDefPointeeBase* p = (CDefPointeeBase*)Object; if (--p->RefCount == 0) p->Release(); } }
    __forceinline T* operator->() const { return Object; }
};
__forceinline CDefPointer<const CQuestCardDef> GetDef(const CTCQuestCard* card) { return CDefPointer<const CQuestCardDef>((const CQuestCardDef*)card->PDef_Object); }
class CTCHeroStats { public:
    unsigned char _pad00[0x70];
    long RenownLevel;                                   // +0x70
    __forceinline long GetRenownLevel() const { return RenownLevel; }
};
struct CInterfaceMap {
    struct Entry { long type; void* component; };
    Entry* first; Entry* last;
    Entry* LowerBound(const long*);                     // 0x0040F020
};
struct CPresenceBits { bool test(long kind); };         // 0x00410DE0
class CThing { public:
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
class CTCInventoryBase { public:
    void* __vftable; CThing* ParentThing;
    CWideString GetPrerequisitesString(CTCQuestCard* card);
};
CWideString CTCInventoryBase::GetPrerequisitesString(CTCQuestCard* card)
{
    CWideString result = CWideString::CreateFromCharString("");
    if (!GetDef(card)->Prerequisites.empty()) {
        SNode* node = GetDef(card)->Prerequisites.begin();
        while (node != GetDef(card)->Prerequisites.end()) {
            if (node->first == 0) {
                result += GetTextBank()->GetTextBySymbol("TEXT_GUI_MENU_RENOWN") + CWideString(":") + IntToWideString(node->second) + CWideString(" ");
                CTCHeroStats* stats;
                if (ParentThing->GetInterface(&stats, 4) && stats)
                    result += CWideString("(") + GetTextBank()->GetTextBySymbol("TEXT_GUI_MENU_CURRENT_LEVEL") + IntToWideString(stats->GetRenownLevel()) + CWideString(")");
            }
            node = Successor(node);
        }
    }
    return result;
}
}