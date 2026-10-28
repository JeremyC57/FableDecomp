// CTCInventory::AssignThingPC(long, unsigned long, NUISystem::CComponent*) -- puts a thing into a PC quick-access slot: records it, marks the slot's old contents for deletion, creates a fresh assignment icon showing the item's graphic, and gives it the dropped-nowhere (unassign, plus ability or item) and dropped (use) actions
// Grade: DIFFER(873v854) -- residue (79 mnemonic diffs, +19 bytes): the two 'use' arms each keep their own push_back where retail cross-jumps them into one (retail homes both arms' new CActionParamSLONG temporaries in the dead thingID argument word, ours gives the first arm the dead manager word), the previous-contents name select is a register select where retail pushes in the arms, and the GetDef argument pushes precede PeekWorld (retail after); lane gap: temporary packing / cross-jumping | tried: replaced the old invented GetDef/GetSubDef stand-in bodies with the real landed bodies of 0x0042B040 / 0x005936C2 as same-TU noinline callees (same 103 as the stand-ins, now legitimate), full PDB respell (CDeletion + SetDeletion, CActionParamULONG/SLONG from RTTI 0x0122F844/0x012366F4, CAction::AddParam overloads, AddActionOnDroppedNowhere/AddActionOnDropped/AddChild slots, CSprite::SetGraphicIndex, CManager::GetInstance, CTCBase::PeekWorld, CCountedPointer<CAction>::Reset), per-arm push_back without the named param local plus the reversed name ternary (103 -> 79), ActionType/value ternaries and a value local (worse); 1 RTM grade this pass (17 total) | next: a source form whose two use-arms leave identical tails (same temporary word) so VC7.1 cross-jumps them
// Leaf: ?AssignThingPC@CTCInventory@@QAEXJKPAVCComponent@NUISystem@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00584B86, 854 bytes, thiscall `ret 0Ch`, frame-pointer size TU
//   (`optimize("y", off)` + `optimize("s", on)`, as the landed CTCInventory siblings), `leave ; ret`.
// Identity: Ego_r.pdb / FableWin.pdb ?AssignThingPC@CTCInventory@@QAEXJKPAVCComponent@NUISystem@@@Z.
// Callees: CTCInventory::SetThingAsQuickAccessItem(long, long, bool) 0x00584157, NUISystem::CManager::GetInstance()
//   0x0041E5F2, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CComponent::FindChild(CCharString)
//   (slot 3), NUISystem::CDeletion() 0x0042BED4 / CDeletion(const CDeletion&) 0x0042CD84 into CComponent::SetDeletion
//   (slot 62) and the list destructor 0x0042ABCA, CManager::CreateComponent(const CCharString&, bool) 0x0041DB1D,
//   CCountedPointer<CComponent>(CComponent*) 0x00429C15 / ~ 0x004291DE, CTCInventoryItem::GetEngineGraphic(long)
//   0x005D96A0 (static fastcall, CEngineGraphic by hidden pointer), CSprite::SetGraphicIndex(unsigned long) (slot 145),
//   operator new 0x00BFEA1A, NUISystem::CAction() 0x0042BE50, CCountedPointer<CAction>(CAction*) 0x0042AA29 / Reset
//   0x005791E5 / ~ 0x00429160, CAction::AddParam(CActionParam*, CCountedPointer<CAction>&) 0x0053FDE0 / AddParam(
//   CActionParam*) 0x0053FDB0, list<CActionParam*>::push_back 0x0042B646, CActionParamULONG (vtable 0x0122F844) /
//   CActionParamSLONG (vtable 0x012366F4) inline constructors, CComponent::AddActionOnDroppedNowhere (slot 75) /
//   AddActionOnDropped (slot 74), CObserver::ObserveEvent (slot 3, through the +4 base), CComponent::AddChild (slot 60),
//   CTCBase::PeekWorld() const 0x00686A80, CDefinitionManager::GetDef<CThingObjectDef> 0x0042B040,
//   CDefClassBase::GetSubDef<CAbilityDef> 0x005936C2.
// Layout: retail CTCInventory QuickAccessItems +0x148 (PDB +0x168); CWorld DefinitionManager +0x10; CAbilityDef Ability
//   +0x28; NUISystem::CComponent's CObserver base +0x04, CActionParam base +0x18.
//   Local model: engine/CTCInventory.h declares no member functions (header debt).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

class CEngineGraphic {                                  // PDB 14 bytes
public:
    unsigned long BankIndex;                            // +0x00
    float         AnimStep;                             // +0x04
    float         RenderSizeX;                          // +0x08
    bool          AdditiveAlpha;                        // +0x0C
    unsigned char Type;                                 // +0x0D
    CEngineGraphic(const CEngineGraphic& other);
};
class CTCInventoryItem {
public:
    static CEngineGraphic __fastcall GetEngineGraphic(long thingID);   // 0x005D96A0
};

namespace NUISystem { class CComponent; class CAction; class CActionParam; }

namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    void* _M_node;                                      // +0x00 (STLport, 4 bytes)
    ~list();                                            // 0x0042ABCA (list<CComponent*>)
    void push_back(const _Tp& __x);                     // 0x0042B646 (list<CActionParam*>)
};
}

class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    CCountedPointer(T* data);                           // 0x00429C15 (CComponent) / 0x0042AA29 (CAction)
    ~CCountedPointer();                                 // 0x004291DE (CComponent) / 0x00429160 (CAction)
    void Reset(T* data);                                // 0x005791E5 (CAction)
    T* operator->() const { return Data; }
};

namespace NUISystem {
enum EActionParamType { ACTION_PARAM_SLONG = 3, ACTION_PARAM_ULONG = 5 };
class CActionParam {
public:
    virtual ~CActionParam();                            // slot 0 (vtable 0x0122F83C)
    EActionParamType ActionParamType;                   // +0x04
    CActionParam(EActionParamType type) : ActionParamType(type) { }
};
class CActionParamULONG : public CActionParam {         // vtable 0x0122F844
public:
    unsigned long Value;                                // +0x08
    CActionParamULONG(unsigned long value) : CActionParam(ACTION_PARAM_ULONG), Value(value) { }
    virtual ~CActionParamULONG();
};
class CActionParamSLONG : public CActionParam {         // vtable 0x012366F4
public:
    long Value;                                         // +0x08
    CActionParamSLONG(long value) : CActionParam(ACTION_PARAM_SLONG), Value(value) { }
    virtual ~CActionParamSLONG();
};
enum EActionType {
    ACTION_TYPE_0F6 = 0xf6, ACTION_TYPE_0FC = 0xfc, ACTION_TYPE_0FD = 0xfd,
    ACTION_TYPE_147 = 0x147, ACTION_TYPE_149 = 0x149
};
class CAction {                                         // retail 0x10 bytes
public:
    EActionType                   ActionType;           // +0x00
    std::list<CActionParam*>      UIActionParams;       // +0x04
    std::list<long>               ThingActionParams;    // +0x08
    bool                          Valid;                // +0x0C
    CAction();                                          // 0x0042BE50
    void AddParam(CActionParam* param, CCountedPointer<CAction>& self);   // 0x0053FDE0
    void AddParam(CActionParam* param);                 // 0x0053FDB0
};
enum EDeletionMethod { DELETION_METHOD_NONE, DELETION_METHOD_1 };
class CDeletion {
public:
    EDeletionMethod        Method;                      // +0x00
    std::list<CComponent*> AssociatedParents;           // +0x04
    CDeletion();                                        // 0x0042BED4
    CDeletion(const CDeletion& other);                  // 0x0042CD84
};
class CComponentBase {
public:
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual CComponent* FindChild(CCharString name);    // slot 3 (+0x0C)
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
    virtual void AddChild(CCountedPointer<CComponent>& child);   // slot 60 (+0xF0)
    virtual void v061();
    virtual void SetDeletion(CDeletion deletion);       // slot 62 (+0xF8)
    virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068();
    virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073();
    virtual void AddActionOnDropped(CCountedPointer<CAction>& action);         // slot 74 (+0x128)
    virtual void AddActionOnDroppedNowhere(CCountedPointer<CAction>& action);  // slot 75 (+0x12C)
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
    virtual void SetGraphicIndex(unsigned long index);  // slot 145 (+0x244, CSprite)
};
class CObserver {
public:
    virtual void o00(); virtual void o01(); virtual void o02();
    virtual void ObserveEvent(unsigned long eventType); // slot 3 (+0x0C)
    unsigned char _pad_0x04[0x14 - 0x04];
};
class CComponent : public CComponentBase, public CObserver, public CActionParam {   // CObserver +0x04, CActionParam +0x18
};
class CManager {
public:
    CComponent* CreateComponent(const CCharString& name, bool independent);   // 0x0041DB1D
    static CManager* __fastcall GetInstance();          // 0x0041E5F2
};
}

class CRefCounted {                                     // second base: the count sits at +4 behind the vptr
public:
    mutable long m_count;
    void DecRef() const { --m_count; }
    long GetRefCount() const { return m_count; }
};
class CDefVirtual {
public:
    virtual void v00();
    virtual void Destroy() const;                       // slot 1
};
class CDefBase : public CDefVirtual, public CRefCounted {
public:
    void Release() const { if (--m_count == 0) Destroy(); }
    void ReleaseTemporary() const { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
class CDefClassBase;
template <class T> class CDefPointer;
template <> class CDefPointer<const CDefClassBase> {    // CDefinitionManager's public const lookup result
public:
    const CDefBase* m_p;
    CDefPointer() { }
    ~CDefPointer() { if (m_p) m_p->ReleaseTemporary(); }
};
template <> class CDefPointer<CDefClassBase> {          // the protected lookup result
public:
    CDefBase* m_p;
    CDefPointer() { }
    __forceinline ~CDefPointer() { if (m_p) { m_p->ReleaseTemporary(); m_p = 0; } }
};
template <class T> class CDefPointer {
public:
    T* m_p;
    CDefPointer() : m_p(0) { }
    CDefPointer(const CDefPointer<CDefClassBase>& other) : m_p((T*)other.m_p) { if (m_p) ++m_p->m_count; }
    __forceinline ~CDefPointer() { if (m_p) m_p->Release(); }
    __forceinline CDefPointer& operator=(const CDefPointer& other)
    {
        if (m_p != other.m_p) {
            if (m_p) {
                m_p->Release();
                m_p = 0;
            }
            m_p = other.m_p;
            if (m_p)
                ++m_p->m_count;
        }
        return *this;
    }
    __forceinline CDefPointer& operator=(const CDefPointer<const CDefClassBase>& other)
    {
        if (m_p != other.m_p) {
            if (m_p) {
                m_p->Release();
                m_p = 0;
            }
            m_p = static_cast<T*>(other.m_p);
            if (m_p)
                ++m_p->m_count;
        }
        return *this;
    }
    T* operator->() const { return m_p; }
};
enum EHeroAbility { };
class CAbilityDef : public CDefBase {
public:
    unsigned char _pad_0x08[0x28 - 0x08];
    EHeroAbility  Ability;                              // +0x28
};
class CDefinitionManager {
    friend class CDefClassBase;
public:
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long index) const;   // 0x009AD9E0
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0042B040 (T = CThingObjectDef)
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);               // 0x009ADA10
};
class CDefClassBase : public CDefBase {
public:
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual const long* GetSubDefIndex(const CCharString& name) const;   // slot 14 (+0x38)
    unsigned char       _pad_0x08[0x1c - 0x08];
    CDefinitionManager* PDefManager;                    // +0x1C
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x005936C2 (T = CAbilityDef)
};
class CThingObjectDef : public CDefClassBase { };
class CGameDefinitionManager : public CDefinitionManager { };

// Same-TU bodies of the two real callees (retail 0x0042B040 / 0x005936C2, as landed), under auto_inline(off) +
// noinline: VC7.1 then homes the def pointers as retail does.
#pragma auto_inline(off)
template <>
__declspec(noinline) bool CDefinitionManager::GetDef<CThingObjectDef>(long index, CDefPointer<const CThingObjectDef>* out) const
{
    if (index <= 0)
        return false;
    CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
    if (def.m_p) {
        *out = def;
        return true;
    }
    return false;
}
template <>
__declspec(noinline) bool CDefClassBase::GetSubDef<CAbilityDef>(CDefPointer<const CAbilityDef>* out) const
{
    const long* pIndex;
    {
        CCharString name("CAbilityDef", -1);
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<const CAbilityDef> result(PDefManager->GetPDefFromGlobalIndex(*pIndex));
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
class CWorld {
public:
    unsigned char           _pad_0x00[0x10];
    CGameDefinitionManager* DefinitionManager;          // +0x10
};

class CTCBase {
public:
    void*         __vftable;                            // +0x000
    void*         ParentThing;                          // +0x004
protected:
    const CWorld& PeekWorld() const;                    // 0x00686A80
};
class CTCInventory : public CTCBase {
public:
    unsigned char _pad_0x008[0x140];
    long*         QuickAccessItems[3];                  // +0x148 vector<long> (PDB +0x168)
    void SetThingAsQuickAccessItem(long thingID, long slot, bool assign);   // 0x00584157
    void AssignThingPC(long thingID, unsigned long slot, NUISystem::CComponent* container);
};

void CTCInventory::AssignThingPC(long thingID, unsigned long slot, NUISystem::CComponent* container)
{
    long previous = QuickAccessItems[0][slot];
    SetThingAsQuickAccessItem(thingID, slot, true);
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    NUISystem::CComponent* old = container->FindChild(previous != 0 ? "PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_INSIDE_CONTAINER" : "PC_EXPRESSION_ITEM_INSIDE_CONTAINER");
    if (old) {
        NUISystem::CDeletion deletion;
        deletion.Method = NUISystem::DELETION_METHOD_1;
        old->SetDeletion(deletion);
    }
    CCountedPointer<NUISystem::CComponent> item(manager->CreateComponent("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_INSIDE_CONTAINER", false));
    NUISystem::CComponent* sprite = item->FindChild("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_SPRITE");
    if (sprite)
        sprite->SetGraphicIndex(CTCInventoryItem::GetEngineGraphic(thingID).BankIndex);
    CCountedPointer<NUISystem::CAction> action(new NUISystem::CAction);
    action->ActionType = NUISystem::ACTION_TYPE_0FC;
    action->AddParam(container, action);
    action->UIActionParams.push_back(new NUISystem::CActionParamULONG(slot));
    item->AddActionOnDroppedNowhere(action);
    bool isAbility = false;
    CDefPointer<const CThingObjectDef> def;
    PeekWorld().DefinitionManager->GetDef(thingID, &def);
    CDefPointer<const CAbilityDef> ability;
    if (def.m_p && def->GetSubDef(&ability)) {
        isAbility = true;
        action.Reset(new NUISystem::CAction);
        action->ActionType = NUISystem::ACTION_TYPE_0F6;
        action->UIActionParams.push_back(new NUISystem::CActionParamSLONG(0));
    } else {
        action.Reset(new NUISystem::CAction);
        action->ActionType = NUISystem::ACTION_TYPE_0FD;
        action->UIActionParams.push_back(new NUISystem::CActionParamSLONG(0));
    }
    item->AddActionOnDroppedNowhere(action);
    item->ObserveEvent(0x19);
    CCountedPointer<NUISystem::CAction> use(new NUISystem::CAction);
    if (!isAbility) {
        use->ActionType = NUISystem::ACTION_TYPE_147;
        use->UIActionParams.push_back(new NUISystem::CActionParamSLONG(thingID));
    } else {
        use->ActionType = NUISystem::ACTION_TYPE_149;
        use->UIActionParams.push_back(new NUISystem::CActionParamSLONG(ability->Ability));
    }
    use->UIActionParams.push_back(new NUISystem::CActionParamSLONG(slot));
    use->AddParam(container);
    item->AddActionOnDropped(use);
    container->AddChild(item);
}
