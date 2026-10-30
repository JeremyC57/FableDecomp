// CTCInventoryAbilities::GetExpressionsListUIComponent(long& count, NUISystem::CComponent* owner) -- console expressions screen: (null without the hero inventory interface) reset the inventory's menu-entry maps, sort every owned expression item into the social / renown / alignment / stealth lists by its expression def's category (counting them), then build the expressions sub-list (back / select / title headers, four entries) with one ConstructExpressionCategoryList per category; returns the sub-list
// Grade: DIFFER(1296v1296) -- frame/register layout: retail keeps the loop index in memory ([ebp-0x10]) and the item def object in edi, ours puts the index in edi and leaves the def in its slot, which reshuffles the whole frame (0x64 vs 0x6c: this, def pointers, the five vectors); the statement shapes match (tried: base, the CDefPointer dtor as one `&&` test, the dtor through a local copy; retail also releases the def pointers with `dec [edi+4] ; jne` on a register copy where ours re-reads the pointer slot and compares the count in memory)
// Leaf: GetExpressionsListUIComponent@CTCInventoryAbilities
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F4C8F, 1296 bytes, `ret 8`. Frame-pointer size TU: optimize("y",off) + ("s",on).
//   FableWin.pdb ?GetExpressionsListUIComponent@CTCInventoryAbilities@@QAEPAVCComponent@NUISystem@@AAJPAV23@@Z.
// Callees: CFrontEndManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D, CCountedPointer<CComponent>(T*) 0x00429C15 /
//   dtor 0x004291DE, std::bitset::test 0x00410DE0 + CVectorMap::LowerBound 0x0040F020 (inline GetInterface, type 0x11 =
//   CTCInventory), the inventory's component-map clear 0x00593235 (ItemsMenuEntries +0x1FC / AssignedIcons +0x208),
//   CTCInventoryBase::GetAllItems 0x005BF482, CTCBase::GetDefinitions 0x006869D0 / GetGameSubsystem 0x00686D60,
//   CDefinitionManager::GetDef 0x0042AF3C (item def) / 0x005F81BE (expression def), CDefClassBase::GetSubDef 0x005DA240
//   ("CExpressionSubDef"), vector<pair<long,long> >::push_back 0x005947B6, free 0x00BFEA14, CDataBank::GetTextBySymbol
//   0x009C95E0, CWideString() 0x0099AED0 / operator= 0x0099B7D0 / ~ 0x0099B510, CCharString 0x0099EBF0 / ~ 0x0099EAE0,
//   CTCInventoryAbilities::ConstructExpressionCategoryList 0x005F3F7B; component slots 83 (+0x14C SetName), 145 (+0x244
//   SetHeader), 156 (+0x270 SetItemCount). The global name 0x013B8C60 is CCharString("EXPRESSIONS TITLE").
// Shape: the missing-interface test returns before the item vectors exist (edi is only saved on the main path); the
//   category switch sends unknown categories to the social list; the def pointers release inline (CDefPointer dtor
//   `dec [p+4] ; jne`); the caption is a default-constructed CWideString assigned per category.
// Layout: retail CTCInventoryAbilities ParentThing +0x04; retail CTCInventory ItemsMenuEntries / AssignedIcons
//   +0x1FC / +0x208; the item sub-def keeps its expression def index at +0x28 and the expression def its category at
//   +0x8C. engine/CTCInventoryAbilities.h is the Ego_r.pdb layout, so the classes are modelled locally.
#include "engine/CCountedPointer.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol);   // 0x009C95E0
};
struct CGameSubsystem {
    unsigned char _pad00[0x14];
    CDataBank*    TextBank;                             // +0x14
};
extern CCharString g_ExpressionsTitleName;             // 0x013B8C60 ("EXPRESSIONS TITLE")

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long m_refCount;                                    // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    __forceinline ~CDefPointer() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
struct CExpressionDef : CDefBase {
    unsigned char _pad08[0x8c - 0x08];
    long Category;                                      // +0x8c (0 social, 1 renown, 2 alignment, 3 stealth)
};
struct CExpressionSubDef : CDefBase {
    unsigned char _pad08[0x28 - 0x08];
    long ExpressionDefIndex;                            // +0x28
};
struct CDefClassBase : CDefBase {
    bool GetSubDef(CDefPointer<CExpressionSubDef>& out);   // 0x005DA240
};
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CDefClassBase>& out);    // 0x0042AF3C
    bool GetDef(long index, CDefPointer<CExpressionDef>& out);   // 0x005F81BE
};
namespace NUISystem {
struct CComponent {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082();
    virtual void SetName(const CCharString& name);                         // slot 83 (+0x14C)
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093();
    virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098();
    virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
    virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108();
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113();
    virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118();
    virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123();
    virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128();
    virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133();
    virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138();
    virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
    virtual void v144();
    virtual void SetHeader(const CCountedPointer<CComponent>& header);     // slot 145 (+0x244)
    virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150();
    virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155();
    virtual void SetItemCount(long count);                                 // slot 156 (+0x270)
};
struct CList : CComponent {};
struct CManager {
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance();  // 0x0041E5F2
};
}
using namespace NUISystem;
namespace std {
template <class A, class B> struct pair { A first; B second; };
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) { return _M_start[n]; }
    void push_back(const T& value);                     // 0x005947B6
};
}
typedef std::vector<std::pair<long, long> > CItemVector;
struct CPresenceBits { bool test(long kind); };        // 0x00410DE0
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    CPresenceBits m_present;                            // +0x20
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                         // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            CInterfaceMap::SEntry* found = (it == map->last || wanted < it->type) ? map->last : it;
            *out = (T*)found->component;
            return true;
        }
        return false;
    }
};
struct CComponentMap {                                  // map<long, CComponent*> (12 bytes)
    void*         _M_header;
    unsigned long _M_node_count;
    long          _M_key_compare;
    void clear();                                       // 0x00593235
};
struct CTCInventory {                                   // interface type 0x11
    unsigned char _pad000[0x1fc];
    CComponentMap ItemsMenuEntries;                     // +0x1fc
    CComponentMap AssignedIcons;                        // +0x208
    void GetAllItems(CItemVector& out);                 // 0x005BF482 (CTCInventoryBase)
};
struct CTCInventoryAbilities {
    void*   __vftable;                                  // +0x00
    CThing* ParentThing;                                // +0x04
    CDefinitionManager* GetDefinitions();               // 0x006869D0 (CTCBase)
    CGameSubsystem* GetGameSubsystem();                 // 0x00686D60 (CTCBase)
    void ConstructExpressionCategoryList(CWideString& title, CList* list, CItemVector& items, CComponent* owner,
                                         const CCharString& templateName, const CCharString& descriptionSymbol);   // 0x005F3F7B
    CComponent* GetExpressionsListUIComponent(long& count, CComponent* owner);
};
CComponent* CTCInventoryAbilities::GetExpressionsListUIComponent(long& count, CComponent* owner)
{
    CFrontEndManager* fe = CFrontEndManager::GetInstance();
    CTCInventory* inventory;
    if (!ParentThing->GetInterface(&inventory, 0x11))
        return 0;
    inventory->ItemsMenuEntries.clear();
    inventory->AssignedIcons.clear();
    CItemVector allItems;
    CItemVector social;
    CItemVector renown;
    CItemVector alignment;
    CItemVector stealth;
    inventory->GetAllItems(allItems);
    count = 0;
    for (unsigned long i = 0; i < allItems.size(); ++i) {
        CDefPointer<CDefClassBase> def;
        GetDefinitions()->GetDef(allItems[i].first, def);
        CDefPointer<CExpressionSubDef> subDef;
        CDefPointer<CExpressionDef> expression;
        if (def->GetSubDef(subDef)) {
            if (GetDefinitions()->GetDef(subDef->ExpressionDefIndex, expression)) {
                switch (expression->Category) {
                case 1:  renown.push_back(allItems[i]); break;
                case 2:  alignment.push_back(allItems[i]); break;
                case 3:  stealth.push_back(allItems[i]); break;
                default: social.push_back(allItems[i]); break;
                }
                ++count;
            }
        }
    }
    CList* subList = (CList*)fe->CreateComponent("UI_SUB_LIST", false);
    CCountedPointer<CComponent> back(fe->CreateComponent("UI_BACK_FOR_LIST", false));
    CCountedPointer<CComponent> select(fe->CreateComponent("UI_SELECT_FOR_LIST", false));
    subList->SetHeader(back);
    subList->SetHeader(select);
    CCountedPointer<CComponent> title(fe->CreateComponent("UI_EXPRESSIONS_TITLE", false));
    title->SetName(g_ExpressionsTitleName);
    subList->SetHeader(title);
    subList->SetItemCount(4);
    CDataBank* bank = GetGameSubsystem()->TextBank;
    CWideString caption;
    caption = bank->GetTextBySymbol("TEXT_GUI_MENU_SOCIAL");
    ConstructExpressionCategoryList(caption, subList, social, owner, "UI_RING_PIC_EXPR_SOCIAL", "TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_SOCIAL");
    caption = bank->GetTextBySymbol("TEXT_GUI_MENU_RENOWN");
    ConstructExpressionCategoryList(caption, subList, renown, owner, "UI_RING_PIC_EXPR_RENOWN", "TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_RENOWN");
    caption = bank->GetTextBySymbol("TEXT_GUI_MENU_ALIGNMENT");
    ConstructExpressionCategoryList(caption, subList, alignment, owner, "UI_RING_PIC_EXPR_ALIGNMENT",
                                    "TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_ALIGNMENT");
    caption = bank->GetTextBySymbol("TEXT_GUI_MENU_STEALTH");
    ConstructExpressionCategoryList(caption, subList, stealth, owner, "UI_RING_PIC_EXPR_STEALTH", "TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_STEALTH");
    return subList;
}
