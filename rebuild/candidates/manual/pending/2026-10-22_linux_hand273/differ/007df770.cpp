// CTCWallMountEffects::vfunc_9() -- per-frame wall-mount confirmation: when the hero answers the wall-mount prompt, start the use-wall-mount creature action or clear the subtitle
// Grade: DIFFER(436v437) -- lane gap (register-cached frame local): an argument is pushed before `mov ecx,<callee-saved>` for a pointer local that retail spills at its definition and reloads after the region; event/constant register swap. Fable 5.1 advisor: 27 grades, every control-flow-preserving spelling gives identical bytes
// Leaf: vfunc_9
// PDB (advisor): vfunc_9 = CTCWallMountEffects::FrameUpdate(); WaitingForConfirmation is EWallMountEffectsConfirmationType (+0x10); PDef is CDefPointer<CWallMountEffectsDef const> (+0x0c).
// Label: 0x00843610 is the CCreatureAction_UseWallMount ctor.
struct CBaseObject;
struct CThingCreatureBase;
struct CThingPlayerCreature;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
struct CFontBank;
struct CSubtitleRenderer {
    void SetText(const CFontBank& font, const CCharString& a, const CCharString& b, const CCharString& c, const CCharString& d,
                 unsigned long colour, float time, bool f, bool g);   // 0x00492FB4
};
struct CPlayerGui { unsigned char Data[0x18]; CSubtitleRenderer Subtitles; };
extern CPlayerGui* g_pPlayerGui;                            // 0x013B8790
struct CMainGameComponent { unsigned char Data[0x1614c]; CFontBank* PFont; };
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                                   // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
    ~CBaseIntelligentPointer();                             // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerCreature(CBaseObject* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other) { SetPItem(other.GetPItem()); return *this; }
    CThingCreatureBase* operator->() const { return (CThingCreatureBase*)GetPItem(); }
    bool IS_THING_ALIVE() const;                            // 0x006961F0
};

struct CCreatureActionBase {
    virtual void v00();
    unsigned char _pad04[0xa8 - 0x04];
};
struct CCreatureAction_DeclarePlayerACriminal : CCreatureActionBase {
    CBaseIntelligentPointer PCriminal;                      // +0xa8
    CCreatureAction_DeclarePlayerACriminal(CThingCreatureBase* owner, CThingPlayerCreature* criminal);   // 0x00843610
    ~CCreatureAction_DeclarePlayerACriminal();              // 0x007DF380
};
struct CThingCreatureBase {
    bool SetCurrentAction(CCreatureActionBase* action);     // 0x006644F0
};

struct CMessageEvent {
    unsigned char _pad[0x3c];
    long ExtraData;                                         // +0x3c
};
struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) {}
};
struct CMessageEventManager {
    CMessageEvent* FindMostRecentMessage(const CMessageEventFilter_IsOfType& filter);   // 0x004AB8B0
    void RemoveMessage(const CMessageEvent* message);       // 0x00493B5B
};
template<class T> struct CCountedPointer {
    T* Data;
    void* Info;
    T* operator->() const { return Data; }
};
struct CWorld {
    virtual void w000();
    unsigned char _pad04[0x5c];
    CCountedPointer<CMessageEventManager> PMessageEventManager;   // +0x60
};
struct CThing;
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    CWorld* GetWorld() const;                               // 0x00686A70
};
struct CTCWallMountEffects : CTCBase {
    void* PDefData;                                         // +0x08
    void* PDefInfo;                                         // +0x0c
    long WaitingForConfirmation;                            // +0x10
    CIntelligentPointerCreature CreaturePerformingAction;   // +0x14
    void DoEffect();                                        // 0x007DF6A0
    void vfunc_9();
};

void CTCWallMountEffects::vfunc_9()
{
    if (WaitingForConfirmation != 0) {
        CMessageEventManager* manager = GetWorld()->PMessageEventManager.Data;
        CMessageEvent* event = manager->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x10));
        if (event) {
            if (event->ExtraData == 1) {
                if (WaitingForConfirmation == 1)
                    DoEffect();
                else if (WaitingForConfirmation == 2 && CreaturePerformingAction.IS_THING_ALIVE())
                    CreaturePerformingAction->SetCurrentAction(
                        &CCreatureAction_DeclarePlayerACriminal(CreaturePerformingAction.operator->(), (CThingPlayerCreature*)ParentThing));
                CreaturePerformingAction = CIntelligentPointerCreature(0);
                WaitingForConfirmation = 0;
                manager->RemoveMessage(event);
            } else if (WaitingForConfirmation == 1) {
                g_pPlayerGui->Subtitles.SetText(*g_pMainGameComponent->PFont, "Do you want to take the item?", "", "Yes", "No", 4, 0.0f, false, true);
                WaitingForConfirmation = 2;
                manager->RemoveMessage(event);
            } else {
                CreaturePerformingAction = CIntelligentPointerCreature(0);
                WaitingForConfirmation = 0;
                manager->RemoveMessage(event);
            }
        }
    }
}
