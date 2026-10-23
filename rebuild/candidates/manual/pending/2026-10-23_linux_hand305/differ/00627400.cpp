// CTCQuickAccessMenu::ConstructExpressionCategoryList(const CWideString&, NUISystem::CList*, vector<pair<long,long> >&, NUISystem::CComponent*) -- add one expression category entry to the menu list, with its sub-list of items
// Leaf: ConstructExpressionCategoryList
// Grade: DIFFER(737v742) -- fe/zero registers swapped (retail fe=ebx, zero=esi; ours fe=esi, zero=ebx), so the else branch lacks retail xor ebx,ebx / xor esi,esi and one release compares [eax],0 by immediate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00627400, length 742 bytes, `ret 10h`.
// Callees: CFrontEndManager::GetInstance @ 0x0041E5F2, CManager::CreateMenuEntryComponent @ 0x00428494,
// CManager::CreateComponent(const CCharString&, bool) @ 0x0041DB1D, CCountedPointer(T*) @ 0x00429C15 (out of line),
// CCountedPointer::DeleteData @ 0x00428B10, CWideString copy 0x0099B720 / default 0x0099AED0 / += 0x0099B8D0 /
// ~ 0x0099B510 / Format (cdecl varargs) 0x0099BA70, GetXString @ 0x005CC772, CCharString(const char*, long)
// @ 0x0099EBF0 / ~ 0x0099EAE0, operator new 0x00BFEA1A / delete 0x00BFE9BC, malloc 0x00BFEA0E, the UI action
// data copy ctor 0x0042CD84 / dtor 0x0042ABCA, CTCQuickAccessMenu::ContructItemLists @ 0x00626750.
// Component slots (descriptive): +0x0c FindChild(CCharString), +0xec AddChild, +0xf8 SetAction, +0x170 Disable,
// +0x260 AddEntry, +0x270 SetNumEntries, +0x278 SetSelection.
// Layout: CCountedPointer {Data, Info} with the 12-byte info block {RefCount, DeleteFunc, Data} (landed 0x00429C15).
// Shape: the first counted pointer is built with the inline constructor; the two sub-component pointers use the
// out-of-line 0x00429C15 instance (an explicit specialisation keeps them out of line); every release is the inline dtor.
#include <stdlib.h>

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
    void __cdecl Format(const unsigned short* format, ...);    // 0x0099BA70
};
CWideString __fastcall GetXString();                    // 0x005CC772

inline CWideString CountString(long count)
{
    CWideString text;
    text.Format((const unsigned short*)L"(%d)", count);
    return text;
}

struct CCPPointerInfo {
    long  RefCount;
    void  (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
void __fastcall DeleteData(void* data);                 // 0x00428B10

template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(T* data) : Data(data)
    {
        if (data) {
            CCPPointerInfo* info = new CCPPointerInfo;
            if (info) {
                info->RefCount = 1;
                info->DeleteFunc = DeleteData;
                info->Data = Data;
            }
            Info = info;
        } else
            Info = 0;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

struct SNode { SNode* next; SNode* prev; void* data; };
struct CParamList {                                     // STLport list with a malloc node allocator
    SNode* _M_node;
    CParamList()
    {
        SNode* node = (SNode*)malloc(sizeof(SNode));
        node->next = node;
        node->prev = node;
        _M_node = node;
    }
};
struct SUIActionData {                                  // {type, malloc-node list of params}
    long       Type;
    CParamList Params;
    SUIActionData(long type) { Type = type; }
    SUIActionData(const SUIActionData& other);          // 0x0042CD84
    ~SUIActionData();                                   // 0x0042ABCA
};

namespace NUISystem {
struct CComponent {
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual CComponent* FindChild(CCharString name);                    // +0x0c
    virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013();
    virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018();
    virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028();
    virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033();
    virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038();
    virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048();
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058();
    virtual void AddChild(const CCountedPointer<CComponent>& child);    // +0xec (slot 59)
    virtual void v060(); virtual void v061();
    virtual void SetAction(SUIActionData action);                       // +0xf8 (slot 62)
    virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072();
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077();
    virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082();
    virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091();
    virtual void Disable();                                             // +0x170 (slot 92)
    virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097();
    virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102();
    virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112();
    virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117();
    virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122();
    virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127();
    virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132();
    virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137();
    virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142();
    virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147();
    virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151();
    virtual void AddEntry(const CCountedPointer<CComponent>& entry);    // +0x260 (slot 152)
    virtual void v153(); virtual void v154(); virtual void v155();
    virtual void SetNumEntries(long count);                             // +0x270 (slot 156)
    virtual void v157();
    virtual void SetSelection(long index);                              // +0x278 (slot 158)
};
struct CList : CComponent { };
struct CSubList : CComponent { };
struct CManager {
    CComponent* CreateComponent(const CCharString& definition, bool flag);                  // 0x0041DB1D
    CComponent* CreateMenuEntryComponent(const CWideString& text, float a, float b, const CCharString& definition,
                                         float c, CCharString d, float e, CCharString button);   // 0x00428494
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance();  // 0x0041E5F2
};
}
using namespace NUISystem;

template <> CCountedPointer<CList>::CCountedPointer(CList* data);        // 0x00429C15
template <> CCountedPointer<CSubList>::CCountedPointer(CSubList* data);  // 0x00429C15

struct SLongPair { long first; long second; };
struct CPairVector {
    SLongPair* _M_start;
    SLongPair* _M_finish;
    SLongPair* _M_end_of_storage;
    bool empty() const { return _M_start == _M_finish; }
    unsigned int size() const { return _M_finish - _M_start; }
};

struct CTCQuickAccessMenu {
    void ContructItemLists(CPairVector& items, CList* list, CComponent* viewport,
                           CCountedPointer<CSubList>* entries);         // 0x00626750
    void ConstructExpressionCategoryList(const CWideString& title, CList* list, CPairVector& items,
                                         CComponent* viewport);
};

void CTCQuickAccessMenu::ConstructExpressionCategoryList(const CWideString& title, CList* list, CPairVector& items,
                                                         CComponent* viewport)
{
    CFrontEndManager* fe = CFrontEndManager::GetInstance();
    CWideString text(title);
    if (!items.empty()) {
        text += GetXString();
        text += CountString(items.size());
    }
    CCountedPointer<CComponent> entry(fe->CreateMenuEntryComponent(text, 0.0f, 1.0f, "UI_QAM_CATEGORY_TEMPLATE",
                                                                   0.0f, "", 0.0f, "PC_BUTTON"));
    list->AddEntry(entry);
    if (items.empty()) {
        CComponent* data = entry.Data;
        CComponent* child = data->FindChild("ARROW");
        if (child) {
            SUIActionData action(1);
            child->SetAction(action);
        }
        entry.Data->Disable();
    } else {
        CCountedPointer<CList> subList((CList*)fe->CreateComponent("UI_QAM_SUBLIST", false));
        CList* sub = subList.Data;
        entry.Data->AddChild((CCountedPointer<CComponent>&)subList);
        sub->SetNumEntries(items.size());
        CCountedPointer<CSubList> entries((CSubList*)CFrontEndManager::GetInstance()->CreateComponent("UI_QAM_ENTRIES", false));
        entries.Data->SetSelection(0);
        ContructItemLists(items, sub, viewport, &entries);
    }
}
