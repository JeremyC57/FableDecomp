// CTCInventory::OpenPCSkillsMenu() -- open the PC skills screen: drop the old menu components, mark the menu state, reuse an open PC_SKILLS_LIST component or create one (frame children for the title, magic and expressions pictures, descriptions and subtitles, the descriptions also in the description viewport; click actions 0xEB / 0xF3 on the two menu entries; activated and registered), then enable the scroll and close/back clickables and set the draw layer
// Grade: DIFFER(1150v1150) -- frame 0x20 v 0x1c: one extra reserved word in the find-loop group (retail slots: [ebp-1] cmp bool, -8 i, -0xc GetName temp, -0x14 saved esi); advisor variants (named local, flipped free operator==, named bool in block) inert or worse
// Leaf: ?OpenPCSkillsMenu@CTCInventory
// Retail: Fable.exe (TLC, VC7.1) @ 0x00587BFF, 1150 bytes, `ret`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CTCInventory::DeleteMenuComponents 0x00587A41, CFrontEndManager::GetInstance 0x0041E5F2 (+ its slot 2 listener
//   registration), CManager::CreateComponent 0x0041DB1D, CBasicString<char>::operator== 0x004115A0 (inline CCharString ==,
//   `strcmp(text, "")` arm), CCharString 0x0099EBF0 / ~ 0x0099EAE0, operator new 0x00BFEA1A, CAction 0x0042BE50,
//   CCountedPointer<CAction> 0x0042AA29 / ~ 0x00429160, param-list push_back 0x0042B646 (same-TU noinline stand-in),
//   vector<CComponent*>::push_back 0x00486024. Component slot names are descriptive.
// Layout: CTCInventory MenuComponents +0x0c (vector<CComponent*>), MenuState +0x21c; a component's listener base at +4 and its
//   CActionParam subobject at +0x18.
#include "engine/CCountedPointer.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" int __cdecl strcmp(const char* a, const char* b);
#pragma intrinsic(strcmp)
struct CBasicStringChar {
    bool operator==(const char* text) const;        // 0x004115A0
};
struct CCharStringData {
    CBasicStringChar Data;
};
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
namespace NUISystem {
struct CActionParam {
    virtual void v00();
    long Type;                                          // +0x04
    CActionParam(long type) : Type(type) {}
};
struct CActionParamList {
    void* _M_node;
    void push_back(CActionParam* const& value);         // 0x0042B646
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
    long             ActionType;
    CActionParamList UIActionParams;
    void*            ThingActionParams;
    bool             Valid;
    CAction();                                          // 0x0042BE50
};
struct CComponent;
struct CComponentBase {
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual CComponent* GetChild(CCharString name);   // slot 3 (+0x0C)
    virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013();
    virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018();
    virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028();
    virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033();
    virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038();
    virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042();
    virtual void Activate();   // slot 43 (+0xAC)
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void SetDrawLayer(long layer);   // slot 48 (+0xC0)
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058();
    virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068();
    virtual void AddClickAction(const CCountedPointer<CAction>& action);   // slot 69 (+0x114)
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual CCharString GetName() const;   // slot 84 (+0x150)
    virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109();
    virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123();
    virtual void AddFrameChild(CComponent* child);   // slot 124 (+0x1F0)
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141();
    virtual void AddViewportChild(CComponent* child);   // slot 142 (+0x238)
};
struct CInputListener {                                 // the component's listener base (+4)
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void SetListenState(long state);            // slot 3 (+0x0C)
    unsigned char _pad04[0x14 - 0x04];
};
struct CComponent : CComponentBase, CInputListener, CActionParam {};
struct CViewport : CComponent {};
struct CManager {
    virtual void v00(); virtual void v01();
    virtual void AddListener(CInputListener* listener); // slot 2 (+0x08)
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance();  // 0x0041E5F2
};
}
using namespace NUISystem;
__forceinline CActionParam* AsParam(CComponent& component) { return &component; }

struct CComponentVector {                               // STLport vector<CComponent*>
    CComponent** _M_start;
    CComponent** _M_finish;
    CComponent** _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    CComponent* operator[](unsigned long i) const { return _M_start[i]; }
    void push_back(CComponent* const& x);               // 0x00486024
};
struct CTCInventory {
    unsigned char    _pad00[0x0c];
    CComponentVector MenuComponents;                    // +0x0c
    unsigned char    _pad18[0x21c - 0x18];
    long             MenuState;                         // +0x21c
    void DeleteMenuComponents();                        // 0x00587A41
    void OpenPCSkillsMenu();
};

void CTCInventory::OpenPCSkillsMenu()
{
    CComponent* menu;
    DeleteMenuComponents();
    MenuState = 1;
    CComponent* list = 0;
    CFrontEndManager* fe = CFrontEndManager::GetInstance();
    for (unsigned long i = 0; i < MenuComponents.size(); ++i) {
        if (list)
            break;
        if (MenuComponents[i]->GetName() == "PC_SKILLS_LIST")
            list = MenuComponents[i];
    }
    if (!list) {
        list = fe->CreateComponent("PC_SKILLS_LIST", false);
        menu = list;
        CComponent* frame = list->GetChild("PC_UI_FRAME");
        CComponent* description = list->GetChild("PC_DESCRIPTION_VIEWPORT");
        CViewport* viewport = 0;
        if (description)
            viewport = (CViewport*)description;
        if (frame) {
            CComponent* child = list->GetChild("PC_TITLE_SKILLS");
            if (child)
                frame->AddFrameChild(child);
            child = list->GetChild("PC_RING_PIC_MAGIC");
            if (child)
                frame->AddFrameChild(child);
            child = list->GetChild("PC_DESCRIPTION_MAGIC");
            if (child) {
                frame->AddFrameChild(child);
                if (viewport)
                    viewport->AddViewportChild(child);
            }
            child = list->GetChild("PC_SUBTITLE_MAGIC");
            if (child)
                frame->AddFrameChild(child);
            child = list->GetChild("PC_RING_PIC_EXPRESSIONS");
            if (child)
                frame->AddFrameChild(child);
            child = list->GetChild("PC_DESCRIPTION_EXPRESSIONS");
            if (child) {
                frame->AddFrameChild(child);
                if (viewport)
                    viewport->AddViewportChild(child);
            }
            child = list->GetChild("PC_SUBTITLE_EXPRESSIONS");
            if (child)
                frame->AddFrameChild(child);
            if (description)
                frame->AddFrameChild(description);
        }
        CComponent* expressions = list->GetChild("PC_MENU_ENTRY_EXPRESSIONS");
        CComponent* magic = list->GetChild("PC_MENU_ENTRY_MAGIC");
        if (expressions) {
            CCountedPointer<CAction> action(new CAction);
            action->ActionType = 0xeb;
            action->UIActionParams.push_back(AsParam(*expressions));
            action->UIActionParams.push_back(AsParam(*list));
            expressions->AddClickAction(action);
        }
        if (magic) {
            CCountedPointer<CAction> action(new CAction);
            action->ActionType = 0xf3;
            action->UIActionParams.push_back(AsParam(*magic));
            action->UIActionParams.push_back(AsParam(*list));
            magic->AddClickAction(action);
        }
        list->Activate();
        MenuComponents.push_back(menu);
    }
    CComponent* scroll = list->GetChild("SCROLL_LIST_UP");
    if (scroll)
        ((CInputListener*)scroll)->SetListenState(0x19);
    scroll = list->GetChild("SCROLL_DESCRIPTION_UP");
    if (scroll)
        ((CInputListener*)scroll)->SetListenState(0x19);
    scroll = list->GetChild("SCROLL_LIST_DOWN");
    if (scroll)
        ((CInputListener*)scroll)->SetListenState(0x19);
    scroll = list->GetChild("SCROLL_DESCRIPTION_DOWN");
    if (scroll)
        ((CInputListener*)scroll)->SetListenState(0x19);
    CComponent* clickable = list->GetChild("PC_BACK_CLICKABLE");
    if (clickable) {
        CFrontEndManager::GetInstance()->AddListener(clickable);
        ((CInputListener*)clickable)->SetListenState(0x19);
    }
    clickable = list->GetChild("PC_CLOSE_CLICKABLE");
    if (clickable) {
        CFrontEndManager::GetInstance()->AddListener(clickable);
        ((CInputListener*)clickable)->SetListenState(0x19);
    }
    list->SetDrawLayer(5);
}
