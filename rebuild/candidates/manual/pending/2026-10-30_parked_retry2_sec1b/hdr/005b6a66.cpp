// CTCInventoryClothing::ConstructClothingListPC(const std::vector<std::pair<long,long> >& items, NUISystem::CViewport* viewport, NUISystem::CComponent* frame, EClothingSuitPart part, bool plainEntries) -- PC clothing list for one suit part: build the clothing-actions list with its "CLOTHING TITLE" header, then (for a non-empty item list) a sub-list with the same title and one menu entry per item wired with select (viewport, entry, frame), focus, unfocus, equip and remove actions, each entry either cleared (plain) or given the actions list, and registered with AddClothingMenuEntry; returns the sub-list (null when there are no items)
// Grade: DIFFER(1511v1507) -- frame layout only: retail homes titleString at [ebp-8], the early CCharString temporaries and later the sub-list pointer at [ebp-0xC] (0x7C frame); ours puts the temporaries at [ebp-8], titleString at [ebp-0xC], the sub-list in its own slot (0x78 frame) and every lower slot shifts by 4 (tried: base, the non-empty path as an else block, the console twin 0x005B70B8 shape with a null-initialised sub-list and a block [1509v1507, still 0x78])
// Leaf: ConstructClothingListPC@CTCInventoryClothing
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B6A66, 1507 bytes, `ret 14h`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CTCInventoryBase::OpenInventory 0x005BD4EA, CFrontEndManager::GetInstance 0x0041E5F2, CManager::CreateComponent
//   0x0041DB1D / CreateMenuEntryComponent 0x00428494, CCountedPointer<CComponent>(T*) 0x00429C15 / Reset 0x00429C6F / dtor
//   0x004291DE, CCountedPointer<CText> ShareData 0x00429460 / dtor 0x0042925C, CWideString() 0x0099AED0 / CWideString(const
//   wchar*) 0x0099B6B0 / ~ 0x0099B510, CCharString 0x0099EBF0 / ~ 0x0099EAE0, CTCInventoryClothing::ConstructTitleString
//   0x005B6881 / AddClothingMenuEntry 0x005B4771, operator new 0x00BFEA1A, CAction 0x0042BE50, CCountedPointer<CAction>
//   0x0042AA29 / ~ 0x00429160, param-list push_back 0x0042B646 (same-TU noinline stand-in), thing-param list<long> push_back
//   0x0042B687, CAction::AddParam 0x0053FDE0; component slots 59 (+0xEC), 66..68 (+0x10C/+0x110 action adders), 83 (+0x14C
//   SetName), 92 (+0x170 Clear), 124 (+0x1F0 frame registration), 144 (+0x240 CText::SetText), 145 (+0x244 SetHeader),
//   150 (+0x258 SetUserValue), 152/153 (+0x260/+0x264), 156 (+0x270 SetItemCount).
// Shape: the landed CreateFrontEndButtonComponent 0x00596A68 CCountedPointer<CText> model (a default-constructed text pointer
//   assigned from the CComponent pointer through ShareData); the viewport / frame CActionParam conversions are loop invariants
//   VC hoists in front of the index loop (the index lives in the dead `frame` slot); the def index is a named local because
//   the thing-param list takes its address.
// Layout: engine/CTCInventoryClothing.h is the Ego_r.pdb layout and no data member is read here, so `this` is a method-only
//   local model; CAction 0x10 (thing-param list<long> at +0x08), CActionParamLong 0x0C (vtable 0x012366F4, type 3).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    CWideString(const unsigned short* text);            // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
#include "engine/CCountedPointer.h"
namespace NUISystem {
struct CActionParam {
    virtual void v00();
    long Type;                                          // +0x04
    CActionParam(long type) : Type(type) {}
};
struct CActionParamLong : CActionParam {                // vtable 0x012366F4
    long Value;                                         // +0x08
    CActionParamLong(long value) : CActionParam(3), Value(value) {}
};
struct CActionParamList {
    void* _M_node;
    void push_back(CActionParam* const& value);         // 0x0042B646
};
struct CLongList {
    void* _M_node;
    void push_back(const long& value);                  // 0x0042B687
};
}
void InsertParamNode(void* node, NUISystem::CActionParam* value);
#pragma auto_inline(off)
__declspec(noinline) void NUISystem::CActionParamList::push_back(NUISystem::CActionParam* const& value)
{
    InsertParamNode(_M_node, value);
}
#pragma auto_inline(on)
namespace NUISystem {
struct CAction {
    long             ActionType;                        // +0x00
    CActionParamList UIActionParams;                    // +0x04
    CLongList        ThingActionParams;                 // +0x08
    bool             Valid;                             // +0x0c
    CAction();                                          // 0x0042BE50
    void AddParam(CActionParam* param, CCountedPointer<CAction>& self);   // 0x0053FDE0
};
struct CComponentBase {
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
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058();
    virtual void AddChildAtEnd(const CCountedPointer<struct CComponent>& child);     // slot 59 (+0xEC)
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066();
    virtual void AddSelectedAction(const CCountedPointer<CAction>& action);         // slot 67 (+0x10C)
    virtual void AddSpecialAction(const CCountedPointer<CAction>& action);          // slot 68 (+0x110)
    virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082();
    virtual void SetName(const CCharString& name);                                  // slot 83 (+0x14C)
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091();
    virtual void Clear();                                                           // slot 92 (+0x170)
    virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097();
    virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102();
    virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112();
    virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117();
    virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122();
    virtual void v123();
    virtual void AddFrameMember(struct CComponent* member);                         // slot 124 (+0x1F0)
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
    virtual void SetText(const CWideString& text, bool a, bool b);                  // slot 144 (+0x240, CText)
    virtual void SetHeader(const CCountedPointer<struct CComponent>& header);       // slot 145 (+0x244)
    virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149();
    virtual void SetUserValue(long value);                                          // slot 150 (+0x258)
    virtual void v151();
    virtual void AddFocusAction(const CCountedPointer<CAction>& action);            // slot 152 (+0x260)
    virtual void AddUnfocusAction(const CCountedPointer<CAction>& action);          // slot 153 (+0x264)
    virtual void v154(); virtual void v155();
    virtual void SetItemCount(long count);                                          // slot 156 (+0x270)
    unsigned char _pad04[0x18 - 0x04];
};
struct CComponent : CComponentBase, CActionParam {};
struct CText : CComponent {};
struct CViewport : CComponent {};
struct CList {                                          // a list component (separate view of the vtable: slot 152 adds an entry)
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
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084();
    virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109();
    virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144();
    virtual void SetHeader(const CCountedPointer<CComponent>& header);       // slot 145 (+0x244)
    virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150();
    virtual void v151();
    virtual void AddEntry(const CCountedPointer<CComponent>& entry);         // slot 152 (+0x260)
    virtual void v153(); virtual void v154(); virtual void v155();
    virtual void SetItemCount(long count);                                   // slot 156 (+0x270)
};
struct CManager {
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
    CComponent* CreateMenuEntryComponent(const CWideString& text, float a, float b, const CCharString& definition,
                                         float c, CCharString d, float e, CCharString button);   // 0x00428494
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
    unsigned long size() const { return _M_finish - _M_start; }
    bool empty() const { return _M_start == _M_finish; }
    const T& operator[](unsigned long n) const { return _M_start[n]; }
};
}
typedef std::vector<std::pair<long, long> > CItemVector;
enum EClothingSuitPart { CLOTHING_SUIT_PART_NONE };
struct CTCInventoryClothing {
    void OpenInventory();                               // 0x005BD4EA (CTCInventoryBase)
    void ConstructTitleString(CWideString& out);        // 0x005B6881
    void AddClothingMenuEntry(EClothingSuitPart part, CComponent* entry);   // 0x005B4771
    CComponent* ConstructClothingListPC(const CItemVector& items, CViewport* viewport, CComponent* frame, EClothingSuitPart part,
                                        bool plainEntries);
};
CComponent* CTCInventoryClothing::ConstructClothingListPC(const CItemVector& items, CViewport* viewport, CComponent* frame,
                                                          EClothingSuitPart part, bool plainEntries)
{
    OpenInventory();
    CFrontEndManager* fe = CFrontEndManager::GetInstance();
    CCountedPointer<CComponent> actions(fe->CreateComponent("PC_UI_LIST_CLOTHING_ACTIONS", false));
    CComponent* actionsList = actions.Data;
    CCountedPointer<CComponent> title(fe->CreateComponent("PC_TITLE", false));
    CCountedPointer<CText> titleText;
    title->SetName("CLOTHING TITLE");
    titleText.SetStaticCast(title);
    CWideString titleString;
    ConstructTitleString(titleString);
    titleText->SetText(titleString, true, true);
    actionsList->SetHeader(title);
    if (frame)
        frame->AddFrameMember(title.Data);
    if (items.empty())
        return 0;
    CList* subList = (CList*)fe->CreateComponent("PC_SUB_LIST", false);
    CCountedPointer<CComponent> subTitle(fe->CreateComponent("PC_TITLE", false));
    subTitle->SetName("CLOTHING TITLE");
    titleText.SetStaticCast(subTitle);
    titleText->SetText(titleString, true, true);
    subList->SetHeader(subTitle);
    if (frame)
        frame->AddFrameMember(subTitle.Data);
    subList->SetItemCount(items.size());
    for (unsigned long i = 0; i < items.size(); ++i) {
        long defIndex = items[i].first;
        CCountedPointer<CComponent> entry;
        if (plainEntries)
            entry.Reset(fe->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE", 0.0f, "", 0.0f,
                                                     "PC_BUTTON_BASTARD"));
        else
            entry.Reset(fe->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE", 0.0f, "", 0.0f,
                                                     "PC_BUTTON_BASTARD"));
        CComponent* comp = entry.Data;
        CCountedPointer<CAction> select(new CAction);
        select->ActionType = 0x7d2;
        select->UIActionParams.push_back(new CActionParamLong(defIndex));
        select->AddParam(viewport, select);
        select->AddParam(entry.Data, select);
        select->AddParam(frame, select);
        entry->AddSelectedAction(select);
        CCountedPointer<CAction> focus(new CAction);
        focus->ActionType = 0x7e1;
        focus->AddParam(entry.Data, focus);
        comp->AddFocusAction(focus);
        CCountedPointer<CAction> unfocus(new CAction);
        unfocus->ActionType = 0x7d5;
        unfocus->UIActionParams.push_back(new CActionParamLong(defIndex));
        unfocus->AddParam(entry.Data, unfocus);
        unfocus->AddParam(frame, unfocus);
        comp->AddUnfocusAction(unfocus);
        comp->SetUserValue(defIndex);
        CCountedPointer<CAction> equip(new CAction);
        equip->ActionType = 0x1f;
        equip->ThingActionParams.push_back(defIndex);
        entry->AddSelectedAction(equip);
        CCountedPointer<CAction> remove(new CAction);
        remove->ActionType = 0x27;
        entry->AddSpecialAction(remove);
        subList->AddEntry(entry);
        if (plainEntries)
            entry->Clear();
        else
            entry->AddChildAtEnd(actions);
        AddClothingMenuEntry(part, entry.Data);
    }
    return (CComponent*)subList;
}
