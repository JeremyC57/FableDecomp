// CTCWallMountEffects::OnWallMountUseAction(CThingCreatureBase& creature) -- when the effect's cool-down has passed, ask the teleport/heal question as a Yes/No subtitle, remember the creature and wait for confirmation; false while cooling down or for a mount with no effect question
// Leaf: ?OnWallMountUseAction@CTCWallMountEffects
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DF4B0, length 345 bytes (`ret 4`, no frame pointer; Ego_r.pdb
//   `?OnWallMountUseAction@CTCWallMountEffects@@QAE_NAAVCThingCreatureBase@@@Z`).
// Callees: CWorld::GetWorldFrame() 0x0049D870 (static, as the landed DoEffect 0x007DF6A0); CCharString(const char*, long)
//   0x0099EBF0, operator=(const char*) 0x0099EFE0, operator!=(const char*) 0x0099E960 (manifest "NotEqual"), ~CCharString
//   0x0099EAE0 (manifest "_Cons_val<...>" propagated); CSubtitleRenderer::SetText(CFontBank*, four const CCharString&,
//   ulong, float, bool, bool) 0x00492FB4 (manifest); CBaseIntelligentPointer() 0x00A01B10 (then the
//   CIntelligentPointer<CThingCreatureBase> vtable 0x0123E7D8), SetPItem 0x00A01B90, GetPItem 0x00A01B50,
//   ~CBaseIntelligentPointer 0x00A01C10.
// Data: "" 0x0122D70E, "Do you want to teleport?" 0x0126FC44, "Do you want to heal yourself?" 0x0126FC60, "Yes" 0x0126FC3C,
//   "No" 0x0126FC40; g_framesPerSecond 0x01375550; g_pMainGameComponent 0x013B86A0 (font bank +0x1614C, local view as the
//   landed DisplayGameInfoText 0x00892A80); g_pPlayerGui 0x013B8790 (SubtitleRenderer +0x18, engine/CPlayerGui.h).
// Layout (Ego_r.pdb, retail offsets equal; as the landed DoEffect): CTCWallMountEffects PDef +0x0c, WaitingForConfirmation
//   +0x10, CreaturePerformingAction +0x14, FrameEffectLastUsed +0x1c; CWallMountEffectsDef Effect +0x28,
//   SecondsBetweenEffectUsesAllowed +0x30.
// Shape: the PDB's private IsAllowedToUseEffect() const is inlined with the def and the frame rate read into named locals
//   before the frame call (def first: the frame rate lands in ebx, the def in edi as retail); the question is a CCharString
//   built empty and assigned in a switch; the No/Yes/"" substitutions are block-scoped named locals (retail builds all three
//   before pushing any argument, and destroys them before the pointer store); `CreaturePerformingAction = &creature` goes
//   through the inline operator= with a converting temporary.
class CTCBase;
class CThing;
class CThingCreatureBase;
class CFontBank;

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const char* text);               // 0x0099EFE0
    bool operator!=(const char* text) const;                // 0x0099E960
};

class CSubtitleRenderer { public:
    void SetText(CFontBank* font, const CCharString& text, const CCharString& a, const CCharString& b, const CCharString& c,
                 unsigned long colour, float time, bool b1, bool b2);   // 0x00492FB4
};
struct CPlayerGuiView {                                     // engine/CPlayerGui.h: SubtitleRenderer +0x18
    unsigned char     _pad_0x00[0x18];
    CSubtitleRenderer SubtitleRenderer;                     // +0x18
};
struct CMainGameComponentFontView { unsigned char _prefix[0x1614c]; CFontBank* Font; };   // +0x1614C
extern CMainGameComponentFontView* g_pMainGameComponent;   // 0x013B86A0
extern CPlayerGuiView* g_pPlayerGui;                        // 0x013B8790
extern long g_framesPerSecond;                              // 0x01375550

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                        // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
    ~CBaseIntelligentPointer();                             // 0x00A01C10
    CThing* GetPItem() const;                               // 0x00A01B50
    void SetPItem(CThing* item);                            // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x0123E7D8 for T = CThingCreatureBase
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};

class CWorld { public:
    static long GetWorldFrame();                            // 0x0049D870
};

enum EWallMountEffects { WALL_MOUNT_EFFECT_NONE = 0, WALL_MOUNT_EFFECT_TELEPORT = 1, WALL_MOUNT_EFFECT_HEAL = 2 };
enum EWallMountEffectsConfirmationType { WALL_MOUNT_CONFIRMATION_NONE = 0, WALL_MOUNT_CONFIRMATION_WAITING = 1 };
class CWallMountEffectsDef { public:
    unsigned char     _pad_00[0x28];
    EWallMountEffects Effect;                               // +0x28
    float             HealingAmount;                        // +0x2c
    long              SecondsBetweenEffectUsesAllowed;      // +0x30
};

struct CTCBaseModel {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    unsigned char _pad08[4];
};
class CTCWallMountEffects : public CTCBaseModel { public:
    const CWallMountEffectsDef*             PDef;                       // +0x0c CDefPointer<CWallMountEffectsDef const>
    EWallMountEffectsConfirmationType       WaitingForConfirmation;     // +0x10
    CIntelligentPointer<CThingCreatureBase> CreaturePerformingAction;   // +0x14
    long                                    FrameEffectLastUsed;        // +0x1c
    bool                                    ObjectWasGamePersistant;    // +0x20
    bool IsAllowedToUseEffect() const
    {
        const CWallMountEffectsDef* def = PDef;
        long framesPerSecond = g_framesPerSecond;
        return CWorld::GetWorldFrame() - FrameEffectLastUsed > def->SecondsBetweenEffectUsesAllowed * framesPerSecond;
    }
    bool OnWallMountUseAction(CThingCreatureBase& creature);
};

bool CTCWallMountEffects::OnWallMountUseAction(CThingCreatureBase& creature)
{
    if (!IsAllowedToUseEffect())
        return false;
    CCharString question("");
    switch (PDef->Effect) {
    case WALL_MOUNT_EFFECT_TELEPORT:
        question = "Do you want to teleport?";
        break;
    case WALL_MOUNT_EFFECT_HEAL:
        question = "Do you want to heal yourself?";
        break;
    }
    if (question != "") {
        {
            CCharString no("No");
            CCharString yes("Yes");
            CCharString empty("");
            g_pPlayerGui->SubtitleRenderer.SetText(g_pMainGameComponent->Font, question, empty, yes, no, 4, 0.0f, false, true);
        }
        CreaturePerformingAction = CIntelligentPointer<CThingCreatureBase>(&creature);
        WaitingForConfirmation = WALL_MOUNT_CONFIRMATION_WAITING;
        return true;
    }
    return false;
}