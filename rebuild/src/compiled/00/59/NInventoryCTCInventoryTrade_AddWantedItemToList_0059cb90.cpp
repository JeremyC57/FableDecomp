// NInventory::CTCInventoryTrade::AddWantedItemToList(long defIndex, NUISystem::CList* list, NUISystem::CViewport* viewport) -- builds a menu entry for a wanted item, wires its five actions and adds it to the list
// Leaf: AddWantedItemToList@CTCInventoryTrade
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059CB90, 953 bytes, `ret 0Ch`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CFrontEndManager::GetInstance 0x0041E5F2, CManager::CreateMenuEntryComponent 0x00428494, CCountedPointer<CComponent>
//   Reset 0x00429C6F / dtor 0x004291DE, CPlayerManager::GetMainPlayer 0x00449960 + GetHeroThing 0x00487DC0 (result unused),
//   CWideString 0x0099B6B0 / ~ 0x0099B510, CCharString 0x0099EBF0 / ~ 0x0099EAE0, operator new 0x00BFEA1A, CAction 0x0042BE50,
//   CCountedPointer<CAction> 0x0042AA29 / ~ 0x00429160, param-list push_back 0x0042B646 (same-TU noinline stand-in),
//   CAction::AddParam(CActionParam*, CCountedPointer<CAction>&) 0x0053FDE0, CActionParamString(CWideString&) 0x0054DAE0;
//   entry slots 67 (+0x10C) / 152 (+0x260) / 153 (+0x264), list slot 152 (+0x260). Slot names are descriptive.
// Layout: CAction 0x10 bytes (landed AddSoundAction 0x0056F615 model); CActionParamSLONG 0x0C (vtable 0x012366F4, type 3);
//   CActionParamString 0x0C; a component's CActionParam subobject sits at +0x18.
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
    CWideString(const unsigned short* text);                   // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
struct CThing;
struct CPlayer { CThing* GetHeroThing(); };           // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer() const; };   // 0x00449960
struct CMainGameComponent { unsigned char _pad[0x1c]; CPlayerManager* PlayerManager; };
extern CMainGameComponent* g_pMainGameComponent;      // 0x013B86A0
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
struct CActionParamString : CActionParam {
    CWideString Value;                                  // +0x08
    CActionParamString(CWideString& text);              // 0x0054DAE0
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
template<class T> struct CCountedPointer {
    T*    m_pObject;
    void* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) {}
    CCountedPointer(T* object);
    ~CCountedPointer();
    void Reset(T* object);
    T* operator->() const { return m_pObject; }
};
namespace NUISystem {
struct CAction {
    long             ActionType;
    CActionParamList UIActionParams;
    void*            ThingActionParams;
    bool             Valid;
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
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066();
    virtual void AddSelectedAction(const CCountedPointer<CAction>& action);   // slot 67 (+0x10C)
    virtual void v068(); virtual void v069();
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
    virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149();
    virtual void v150(); virtual void v151();
    virtual void AddFocusAction(const CCountedPointer<CAction>& action);      // slot 152 (+0x260)
    virtual void AddUnfocusAction(const CCountedPointer<CAction>& action);    // slot 153 (+0x264)
    unsigned char _pad04[0x18 - 0x04];
};
struct CComponent : CComponentBase, CActionParam {};
struct CViewport : CComponent {};
struct CList {
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
    virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149();
    virtual void v150(); virtual void v151();
    virtual void AddEntry(const CCountedPointer<CComponent>& entry);         // slot 152 (+0x260)
};
struct CManager {
    CComponent* CreateMenuEntryComponent(const CWideString& text, float a, float b, const CCharString& definition,
                                         float c, CCharString d, float e, CCharString button);   // 0x00428494
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance();  // 0x0041E5F2
};
}
using namespace NUISystem;
namespace NInventory {
struct CTCInventoryTrade {
    void AddWantedItemToList(long defIndex, CList* list, CViewport* viewport);
};
void CTCInventoryTrade::AddWantedItemToList(long defIndex, CList* list, CViewport* viewport)
{
    CFrontEndManager* fe = CFrontEndManager::GetInstance();
    CCountedPointer<CComponent> entry;
    g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroThing();
    entry.Reset(fe->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_TEMPLATE_NEW",
                                             0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    CComponent* comp = entry.m_pObject;
    CCountedPointer<CAction> select(new CAction);
    select->ActionType = 0x7d0;
    select->UIActionParams.push_back(new CActionParamLong(defIndex));
    select->AddParam(entry.m_pObject, select);
    CWideString model(L"UI_ITEM_MODEL_TEMPLATE_NEW");
    select->UIActionParams.push_back(new CActionParamString(model));
    entry->AddSelectedAction(select);
    CCountedPointer<CAction> focus(new CAction);
    focus->ActionType = 0x7df;
    focus->AddParam(entry.m_pObject, focus);
    comp->AddFocusAction(focus);
    CCountedPointer<CAction> view(new CAction);
    view->ActionType = 0x7db;
    view->UIActionParams.push_back(new CActionParamLong(defIndex));
    view->AddParam(viewport, view);
    view->AddParam(entry.m_pObject, view);
    entry->AddSelectedAction(view);
    CCountedPointer<CAction> focus2(new CAction);
    focus2->ActionType = 0x7e7;
    focus2->AddParam(entry.m_pObject, focus2);
    comp->AddFocusAction(focus2);
    CCountedPointer<CAction> unfocus(new CAction);
    unfocus->ActionType = 0x7d6;
    unfocus->UIActionParams.push_back(new CActionParamLong(defIndex));
    unfocus->AddParam(entry.m_pObject, unfocus);
    unfocus->UIActionParams.push_back(new CActionParamLong(1));
    comp->AddUnfocusAction(unfocus);
    list->AddEntry(entry);
}
}