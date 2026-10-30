// Grade: DIFFER(976v989) -- improved by the shared engine/CCountedPointer.h in place of the parked local CCountedPointer stand-in: differing instructions 131 (was 188, relocation-only pushes included). Levers tried this wave: shared header (1 grade). Next: read the remaining non-relocation residue (stack-slot / scheduling) side by side.
//   parked: Grade: DIFFER(963v989) -- 226 difflines, coarse first pass: overall block shape right (ability list, counted pointers, entry loop, list teardown); ebx/ebp swap for the list head and the zero register, templateUI zero-store position and the entry-name formatting temporaries differ.
// CTCQuickAccessMenu::GetMagicListUI() -- build the quick-access magic list: collect the available hero abilities 1..19, create the magic-actions list and the wield/augment template, and add one menu entry per ability (named with its level when the owner has experience), each with an ability callback; returns the template component
// Leaf: GetMagicListUI
// Retail: Fable.exe (TLC, VC7.1) @ 0x00625AF0, 989 bytes, `ret`.
// Layout: local model -- CTCQuickAccessMenu Parent CThing* +0x04 (interface bitset +0x20, map +0x44); CCountedPointer<T>
//   {T*, counter*} with counter {Count, Deleter 0x00428B10, Data}; STLport list<long> nodes from malloc/free.
#include "engine/CCountedPointer.h"
#include <string.h>
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
void* __cdecl operator new(unsigned int);
void __cdecl operator delete(void*);
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    CCharString(const CCharString& o);                     // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
};
struct CWideString {
    void* m_pData;
    CWideString();                                         // 0x0099AED0
    CWideString(const CWideString& o);                     // 0x0099B720
    ~CWideString();                                        // 0x0099B510
    CWideString& operator=(const CWideString& o);          // 0x0099B7D0
    CWideString& operator+=(const CWideString& o);         // 0x0099B8D0
};
CWideString __fastcall ToWide(const CCharString& s);       // 0x0099B6A0
void __cdecl Format(CWideString* out, const wchar_t* fmt, ...);   // 0x0099BA70
void __fastcall DeleteData(void* data);                    // 0x00428B10
struct CCounter { long Count; void (__fastcall *Deleter)(void*); void* Data; };

struct CMenuCallback { virtual void Invoke(); long Kind; long Ability;
    CMenuCallback(long ability) : Kind(3), Ability(ability) { } };
struct CComponent {
    virtual void v000();
    virtual void v004();
    virtual void v008();
    virtual void v00c();
    virtual void v010();
    virtual void v014();
    virtual void v018();
    virtual void v01c();
    virtual void v020();
    virtual void v024();
    virtual void v028();
    virtual void v02c();
    virtual void v030();
    virtual void v034();
    virtual void v038();
    virtual void v03c();
    virtual void v040();
    virtual void v044();
    virtual void v048();
    virtual void v04c();
    virtual void v050();
    virtual void v054();
    virtual void v058();
    virtual void v05c();
    virtual void v060();
    virtual void v064();
    virtual void v068();
    virtual void v06c();
    virtual void v070();
    virtual void v074();
    virtual void v078();
    virtual void v07c();
    virtual void v080();
    virtual void v084();
    virtual void v088();
    virtual void v08c();
    virtual void v090();
    virtual void v094();
    virtual void v098();
    virtual void v09c();
    virtual void v0a0();
    virtual void v0a4();
    virtual void v0a8();
    virtual void v0ac();
    virtual void v0b0();
    virtual void v0b4();
    virtual void v0b8();
    virtual void v0bc();
    virtual void v0c0();
    virtual void v0c4();
    virtual void v0c8();
    virtual void v0cc();
    virtual void v0d0();
    virtual void v0d4();
    virtual void v0d8();
    virtual void v0dc();
    virtual void v0e0();
    virtual void v0e4();
    virtual void v0e8();
    virtual void v0ec();
    virtual void v0f0();
    virtual void v0f4();
    virtual void v0f8();
    virtual void v0fc();
    virtual void v100();
    virtual void v104();
    virtual void v108();
    virtual void v10c();
    virtual void v110();
    virtual void v114();
    virtual void v118();
    virtual void v11c();
    virtual void v120();
    virtual void v124();
    virtual void v128();
    virtual void v12c();
    virtual void v130();
    virtual void v134();
    virtual void v138();
    virtual void v13c();
    virtual void v140();
    virtual void v144();
    virtual void v148();
    virtual void v14c();
    virtual void v150();
    virtual void v154();
    virtual void v158();
    virtual void v15c();
    virtual void v160();
    virtual void v164();
    virtual void v168();
    virtual void v16c();
    virtual void v170();
    virtual void v174();
    virtual void v178();
    virtual void v17c();
    virtual void v180();
    virtual void v184();
    virtual void v188();
    virtual void v18c();
    virtual void v190();
    virtual void v194();
    virtual void v198();
    virtual void v19c();
    virtual void v1a0();
    virtual void v1a4();
    virtual void v1a8();
    virtual void v1ac();
    virtual void v1b0();
    virtual void v1b4();
    virtual void v1b8();
    virtual void v1bc();
    virtual void v1c0();
    virtual void v1c4();
    virtual void v1c8();
    virtual void v1cc();
    virtual void v1d0();
    virtual void v1d4();
    virtual void v1d8();
    virtual void v1dc();
    virtual void v1e0();
    virtual void v1e4();
    virtual void v1e8();
    virtual void v1ec();
    virtual void v1f0();
    virtual void v1f4();
    virtual void v1f8();
    virtual void v1fc();
    virtual void v200();
    virtual void v204();
    virtual void v208();
    virtual void v20c();
    virtual void v210();
    virtual void v214();
    virtual void v218();
    virtual void v21c();
    virtual void v220();
    virtual void v224();
    virtual void v228();
    virtual void v22c();
    virtual void v230();
    virtual void v234();
    virtual void v238();
    virtual void v23c();
    virtual void v240();
    virtual void v244();
    virtual void v248();
    virtual void v24c();
    virtual void v250();
    virtual void v254();
    virtual void v258();
    virtual void SetCallback(CMenuCallback* cb);           // +0x25c
    virtual void AddEntry(const CCountedPointer<CComponent>& entry);   // +0x260
    virtual void v264();
    virtual void v268();
    virtual void v26c();
    virtual void SetNumEntries(long n);                    // +0x270
};
struct CFrontEndManager {
    static CFrontEndManager* __fastcall GetInstance();     // 0x0041E5F2
    CComponent* CreateComponent(const CCharString& name, bool b);   // 0x0041DB1D
    CComponent* CreateMenuEntryComponent(const CWideString& text, float a, float b, const CCharString& tmpl, float c,
        CCharString d, float e, CCharString f);            // 0x00428494
};
struct CTCInventoryAbilities {
    bool IsAbilityAvailable(long ability);                 // 0x005EDF08
    long GetAbilityLevel(long ability);                    // 0x005EDE93
};
struct CTCInventoryExperience {
    CWideString GetExperienceAbilityName(long ability);    // 0x0056F2BD
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type);                  // 0x0040F020
};
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
#pragma auto_inline(on)
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                        // +0x20
    CInterfaceMap m_interfaces;                            // +0x44
    __forceinline unsigned long HasTC(long type) { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
struct CLongNode { CLongNode* next; CLongNode* prev; long value; };
struct CTCQuickAccessMenu {
    void* vftable;
    CThing* Parent;                                        // +0x04
    CComponent* GetMagicListUI();
};

CComponent* CTCQuickAccessMenu::GetMagicListUI()
{
    CTCInventoryAbilities* abilities = 0;
    if (Parent->HasTC(0x6f)) abilities = Parent->GetTC<CTCInventoryAbilities>(0x6f);
    CLongNode* head = (CLongNode*)malloc(0xc);
    head->next = head; head->prev = head;
    for (long ability = 1; ability < 0x14; ++ability) {
        if (abilities->IsAbilityAvailable(ability)) {
            CLongNode* node = (CLongNode*)malloc(0xc);
            long* slot = &node->value;
            if (slot) *slot = ability;
            CLongNode* last = head->prev;
            node->next = head; node->prev = last; last->next = node; head->prev = node;
        }
    }
    CCountedPointer<CComponent> listUI(CFrontEndManager::GetInstance()->CreateComponent("UI_LIST_MAGIC_ACTIONS", false));
    CComponent* templateUI = 0;
    CFrontEndManager* frontEnd = CFrontEndManager::GetInstance();
    if (head->next != head) {
        templateUI = frontEnd->CreateComponent("UI_LIST_WIELD_AUGMENT_TEMPLATE_QUICK_ACCESS", false);
        long count = 0;
        for (CLongNode* n = head->next; n != head; n = n->next) ++count;
        templateUI->SetNumEntries(count);
        for (CLongNode* it = head->next; it != head; it = it->next) {
            CWideString name;
            if (Parent->HasTC(0x16)) {
                name = Parent->GetTC<CTCInventoryExperience>(0x16)->GetExperienceAbilityName(it->value);
                name += ToWide(" ");
                long level = abilities->GetAbilityLevel(it->value);
                CWideString text;
                Format(&text, L"%d", level);
                name += CWideString(text);
            }
            CCountedPointer<CComponent> entry(frontEnd->CreateMenuEntryComponent(name, 0, 1.0f,
                "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_FOR_QUICK_ACCESS", 0, "", 0, "PC_BUTTON_BASTARD"));
            {
                CCountedPointer<CComponent> copy(entry);
                copy->SetCallback(new CMenuCallback(it->value));
                templateUI->AddEntry(entry);
            }
        }
    }
    CLongNode* n = head->next;
    while (n != head) { CLongNode* dead = n; n = n->next; free(dead); }
    head->next = head; head->prev = head;
    free(head);
    return templateUI;
}
