// CTCInventory::AssignThingPC(long, unsigned long, NUISystem::CComponent*) -- puts a thing into a quick-access slot: records it, clears the old contents' action parameter, creates a fresh assignment icon showing the item's graphic, and gives it the drop, use (ability or item) and select actions
// Grade: DIFFER(854v854) 81 difflines (was 105) -- sec1b 2026-10-30: the old-contents lookup as an if/else with a FindChild call in each arm gives retail's pushes-in-the-arms (the ternary was a register select); left: the named `param` local costs one frame word (frame 0x38 v 0x34, every slot -4) and GetDef's args before PeekWorld. Tried also: inverted ternary (105), unnamed new-temps in both arms (873 bytes: duplicated push_back), unnamed slot param only (79 lines, same frame). Next: one push_back of a conditional `new` expression, so the param needs no named slot.
#include "engine/CCountedPointer.h"
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1); // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                  // 0x0099EAE0
};

struct CEngineGraphic {                              // PDB 14 bytes
    unsigned long BankIndex;                         // +0x00
    float         AnimStep;                          // +0x04
    float         RenderSizeX;                       // +0x08
    bool          AdditiveAlpha;                     // +0x0C
    unsigned char Type;                              // +0x0D
    CEngineGraphic(const CEngineGraphic& other);
};
struct CTCInventoryItem {
    static CEngineGraphic __fastcall GetEngineGraphic(long thingID);   // 0x005D96A0
};

namespace NUISystem { struct CComponent; }
struct SComponentListNode;
struct CComponentList {                              // STLport list<NUISystem::CComponent*>
    SComponentListNode* m_pNode;
    ~CComponentList();                               // 0x0042ABCA
};
struct SActionParamPair {                            // pair<unsigned long, list<NUISystem::CComponent*> >
    unsigned long  first;
    CComponentList second;
    SActionParamPair();                              // 0x0042BED4
    SActionParamPair(const SActionParamPair& other); // 0x0042CD84
};


namespace NUISystem {
struct CActionParam {
    long Type;                                       // +0x04
    CActionParam(long type) : Type(type) {}
    virtual void v00();
};
struct CActionParamEvent : CActionParam {            // vtable 0x0122F844, type 5
    unsigned long Value;                             // +0x08
    CActionParamEvent(unsigned long value) : CActionParam(5), Value(value) {}
    virtual void v00();
};
struct CActionParamLong : CActionParam {             // vtable 0x012366F4, type 3
    long Value;                                      // +0x08
    CActionParamLong(long value) : CActionParam(3), Value(value) {}
    virtual void v00();
};
struct CActionParamList {                            // STLport list<CActionParam*>
    void* _M_node;
    void push_back(CActionParam* const& value);      // 0x0042B646
};
struct CActionPointer;
struct CAction {                                     // retail 0x10 bytes
    long             ActionType;                     // +0x00
    CActionParamList UIActionParams;                 // +0x04
    void*            ThingActionParams;              // +0x08
    bool             Valid;                          // +0x0C
    CAction();                                       // 0x0042BE50
    void SetTarget(CActionParam* target, const CActionPointer& self);   // 0x0053FDE0
    void AddUIActionParam(CActionParam* param);      // 0x0053FDB0
};
struct CActionPointer {
    CAction* m_pObject;
    void*    m_pCount;
    CActionPointer(CAction* object);                 // 0x0042AA29
    ~CActionPointer();                               // 0x00429160
    void Reset(CAction* object);                     // 0x005791E5
    CAction* operator->() const { return m_pObject; }
};

struct CComponentBase {
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual CComponent* FindChild(CCharString name);           // slot 3 (+0xC)
    virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021();
    virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033();
    virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045();
    virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057();
    virtual void v058(); virtual void v059();
    virtual void SetContents_slot60(const CCountedPointer<CComponent>& child); // slot 60 (+0xF0)
    virtual void v061();
    virtual void SetActionParameter_slot62(SActionParamPair parameter); // slot 62 (+0xF8)
    virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068();
    virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073();
    virtual void AddAction_slot74(const CActionPointer& action); // slot 74 (+0x128)
    virtual void AddAction_slot75(const CActionPointer& action); // slot 75 (+0x12C)
    virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081();
    virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093();
    virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105();
    virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111();
    virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117();
    virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123();
    virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135();
    virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141();
    virtual void v142(); virtual void v143(); virtual void v144();
    virtual void SetGraphic_slot145(unsigned long bankIndex);  // slot 145 (+0x244)
};
struct CObserver {
    virtual void o00(); virtual void o01(); virtual void o02();
    virtual void ObserveEvent(long eventType);       // slot 3 (+0x0C)
    unsigned char _pad08[0x18 - 0x08];
};
struct CComponent : CComponentBase, CObserver, CActionParam {   // CObserver +0x04, action target +0x18
};
struct CManager {
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
};
}

struct CAbilityDef {
    virtual void d00();
    virtual void DeleteThis();                       // slot 1 (+4)
    long          RefCount;                          // +0x04
    unsigned char _pad08[0x28 - 0x08];
    long          AbilityID;                         // +0x28
};
struct CAbilityDefPointer {                          // CDefPointer<const CAbilityDef>
    CAbilityDef* m_p;
    CAbilityDefPointer() : m_p(0) {}
    __forceinline ~CAbilityDefPointer() { if (m_p && --m_p->RefCount == 0) m_p->DeleteThis(); }
    CAbilityDef* operator->() const { return m_p; }
};
struct CThingDef {
    virtual void d00();
    virtual void DeleteThis();                       // slot 1 (+4)
    long          RefCount;                          // +0x04
    bool GetSubDef(CAbilityDefPointer* out) const;   // 0x005936C2 GetSubDef<CAbilityDef>
};
struct CThingDefPointer {                            // CDefPointer<const CThingDef>
    CThingDef* m_p;
    CThingDefPointer() : m_p(0) {}
    __forceinline ~CThingDefPointer() { if (m_p && --m_p->RefCount == 0) m_p->DeleteThis(); }
    CThingDef* operator->() const { return m_p; }
};
struct CDefinitionManager {
    bool GetDef(long index, CThingDefPointer* out) const;   // 0x0042B040 GetDef<...>
};
#pragma auto_inline(off)
__declspec(noinline) bool CThingDef::GetSubDef(CAbilityDefPointer* out) const { out->m_p = 0; return false; }
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CThingDefPointer* out) const { out->m_p = 0; return index != 0; }
#pragma auto_inline(on)

struct CWorld {
    unsigned char       _pad00[0x10];
    CDefinitionManager* PDefinitionManager;          // +0x10
};

struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long& operator[](unsigned long n) { return _M_start[n]; }
};

struct CTCInventory {
    void*         __vftable;                         // +0x000
    unsigned char _pad_0x004[0x144];
    CLongVector   QuickAccessItems;                  // +0x148 (PDB +0x168)
    const CWorld& PeekWorld() const;                 // 0x00686A80 (CTCBase)
    void SetThingAsQuickAccessItem(long thingID, long slot, bool assign);   // 0x00584157
    void AssignThingPC(long thingID, unsigned long slot, NUISystem::CComponent* container);
};

void CTCInventory::AssignThingPC(long thingID, unsigned long slot, NUISystem::CComponent* container)
{
    long previous = QuickAccessItems[slot];
    SetThingAsQuickAccessItem(thingID, slot, true);
    NUISystem::CFrontEndManager* manager = NUISystem::CFrontEndManager::GetInstance();
    NUISystem::CComponent* old;
    if (previous == 0)
        old = container->FindChild("PC_EXPRESSION_ITEM_INSIDE_CONTAINER");
    else
        old = container->FindChild("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_INSIDE_CONTAINER");
    if (old) {
        SActionParamPair parameter;
        parameter.first = 1;
        old->SetActionParameter_slot62(parameter);
    }
    CCountedPointer<NUISystem::CComponent> item(manager->CreateComponent("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_INSIDE_CONTAINER", false));
    NUISystem::CComponent* sprite = item->FindChild("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_SPRITE");
    if (sprite)
        sprite->SetGraphic_slot145(CTCInventoryItem::GetEngineGraphic(thingID).BankIndex);
    NUISystem::CActionPointer drop(new NUISystem::CAction);
    drop->ActionType = 0xfc;
    drop->SetTarget(container, drop);
    drop->UIActionParams.push_back(new NUISystem::CActionParamEvent(slot));
    item->AddAction_slot75(drop);
    bool isAbility = false;
    CThingDefPointer def;
    const CDefinitionManager* definitions = PeekWorld().PDefinitionManager;
    definitions->GetDef(thingID, &def);
    CAbilityDefPointer ability;
    if (def.m_p && def->GetSubDef(&ability)) {
        isAbility = true;
        drop.Reset(new NUISystem::CAction);
        drop->ActionType = 0xf6;
        drop->UIActionParams.push_back(new NUISystem::CActionParamLong(0));
    } else {
        drop.Reset(new NUISystem::CAction);
        drop->ActionType = 0xfd;
        drop->UIActionParams.push_back(new NUISystem::CActionParamLong(0));
    }
    item->AddAction_slot75(drop);
    item->ObserveEvent(0x19);
    NUISystem::CActionPointer use(new NUISystem::CAction);
    NUISystem::CActionParam* param;
    if (!isAbility) {
        use->ActionType = 0x147;
        param = new NUISystem::CActionParamLong(thingID);
    } else {
        use->ActionType = 0x149;
        param = new NUISystem::CActionParamLong(ability->AbilityID);
    }
    use->UIActionParams.push_back(param);
    param = new NUISystem::CActionParamLong(slot);
    use->UIActionParams.push_back(param);
    use->AddUIActionParam(container);
    item->AddAction_slot74(use);
    container->SetContents_slot60(item);
}
