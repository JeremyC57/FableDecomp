// CCombatActionBase::OnAnimationLoaded() -- once the action's animation is known: set up strike events, interrupts and action times; tell a hit-notified target (0xcd) about the attacker, record the action handedness, bump a boss phase (strikes) or notify interface 0x30; face the target; for a flourish action, give the target a CTCFlourishTarget, optionally start a scripted flourish camera on a random nearby thing, and blend the environment theme
// Grade: DIFFER(1198v1196) -- 65 blocks: first pass; frame 0x2c v 0x20 (retail packs every interface key and def out-pointer into one slot), ebx/ebp swap for &Owner and the 0x2000 mask, and slot choices for the facing delta; every call, flag test, the facing wrap and the flourish camera / theme tail line up
// Leaf: OnAnimationLoaded
// Retail 0x00859EF0, 1196 bytes, plain `ret` (virtual).
// Callees: CCombatActionBase::SetupCombatStrikeEvents 0x00859A70 / SetupCombatInterrupts 0x00858A20 / InitialiseActionTimes
//   0x00856A20 / UpdateFacingDirection 0x00856B20 / GetDefinitionManager 0x00692910 / GetPWorld 0x00692940 /
//   GetSearchTools 0x00692990; CBaseIntelligentPointer::GetPItem 0x00A01B50; CVectorMap::LowerBound 0x0040F020;
//   CTCCreatureHitNotification::SetHitNotification 0x008651B0; the action def's handedness / flourish def getters
//   0x00669CB0 / 0x00738A00; CThingCreatureBase::SetLastActionHandedness 0x006619F0; CTCBoss::IncrementBossPhase
//   0x007AED30; interface 0x30 notify 0x00733210; CThing::GetPosition 0x004C73D0 / ClearFlag 0x004C9840 / AddTC
//   0x004C9D60; fmod 0x00BFEB28; CTCFlourishTarget::SetAttacker 0x0085AB00; CDefinitionManager::GetDef 0x0085ABB0;
//   GetAllThings<CFilter_Bind2<InMap, !InArea>> 0x0085B0D0; IS_THING_ALIVE 0x004CC340; CWorld::GetWorldSeed 0x0049D810;
//   GFRandom 0x004CDAE0 (`__fastcall`); operator new 0x00BFEA1A; flourish camera mode ctor 0x00852710 / SetTarget
//   0x0072A4A0 / SetPThingOrigin 0x00852750; CCountedPointer(T*) 0x005ED270; CGameCameraManager::
//   SetGameScriptedCameraMode 0x00697B70; CIntelligentPointer(CThing*) 0x0056C860 / = 0x005ED300 / ~ 0x00A01C10;
//   CEnvironment::SetThemeWeightAllChannels 0x006B38D0; CCharString 0x0099EBF0 / ~ 0x0099EAE0; free 0x00BFEA14.
// Layout: from this function's use -- CCombatActionBase {owner +0x08, interrupt frame +0x14, def +0x74, target +0xa8,
//   flourish target +0xb0, flourish def +0xb8, strike list +0xbc, no-interrupt flag +0xde}; CThing {interface bits +0x20,
//   map +0x44, graphic +0x60, dead +0x91}.
extern "C" double __cdecl atan2(double, double);
extern "C" double __cdecl fmod(double, double);
extern "C" void __cdecl free(void* p);                // 0x00BFEA14
#pragma intrinsic(atan2, fmod)
void* __cdecl operator new(unsigned int size);
unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CThing;
struct CTCBase {
    virtual void v00();
};
struct CTCCreatureHitNotification : CTCBase {
    void SetHitNotification(CThing* attacker);         // 0x008651B0
};
struct CTCBoss : CTCBase {
    void IncrementBossPhase();                         // 0x007AED30
};
struct CTCInterface30 : CTCBase {
    void OnCombatAction();                             // 0x00733210
};
struct CTCFlourishTarget : CTCBase {
    unsigned char _pad04[0x14 - 0x04];
    bool          Started;                             // +0x14
    bool          Active;                              // +0x15
    void SetAttacker(CThing* attacker);                // 0x0085AB00
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x0040F020
};
struct CThingGraphic {
    virtual void g00();
    virtual void g01();
    virtual void g02();
    virtual void g03();
    virtual void g04();
    virtual void g05();
    virtual void g06();
    virtual void g07();
    virtual void g08();
    virtual void g09();
    virtual void g0a();
    virtual void g0b();
    virtual void g0c();
    virtual void g0d();
    virtual void g0e();
    virtual void g0f();
    virtual void g10();
    virtual void g11();
    virtual void g12();
    virtual void g13();
    virtual void g14();
    virtual void g15();
    virtual void g16();
    virtual void g17();
    virtual void g18();
    virtual void g19();
    virtual void g1a();
    virtual void g1b();
    virtual void g1c();
    virtual void g1d();
    virtual void g1e();
    virtual void g1f();
    virtual void g20();
    virtual void g21();
    virtual void g22();
    virtual void g23();
    virtual void g24();
    virtual void g25();
    virtual void g26();
    virtual void g27();
    virtual void g28();
    virtual void g29();
    virtual void g2a();
    virtual void g2b();
    virtual void g2c();
    virtual void g2d();
    virtual void g2e();
    virtual void g2f();
    virtual void g30();
    virtual void g31();
    virtual void g32();
    virtual void g33();
    virtual void g34();
    virtual void g35();
    virtual void g36();
    virtual void g37();
    virtual void g38();
    virtual void g39();
    virtual void g3a();
    virtual void g3b();
    virtual void g3c();
    virtual void g3d();
    virtual void g3e();
    virtual void g3f();
    virtual void g40();
    virtual void g41();
    virtual void SetFacingTurns(float turns);
};
struct CThing {
    virtual void v00();
    unsigned char  _pad04[0x20 - 0x04];
    unsigned long  m_presentBits[9];                   // +0x20
    CInterfaceMap  m_interfaces;                       // +0x44
    unsigned char  _pad4c[0x60 - 0x4c];
    CThingGraphic* Graphic;                            // +0x60
    unsigned char  _pad64[0x91 - 0x64];
    unsigned char  Flags91;                            // +0x91
    const C3DVector& GetPosition() const;              // 0x004C73D0
    void ClearFlag(unsigned long flag);                // 0x004C9840
    CTCBase* AddTC(const CCharString& name, bool a, long b);   // 0x004C9D60
    void SetLastActionHandedness(long handedness);     // 0x006619F0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* Data;
    CThing* GetPItem() const;                          // 0x00A01B50
    ~CBaseIntelligentPointer();                        // 0x00A01C10
};
struct CThingPointer : CBaseIntelligentPointer {
    CThingPointer(CThing* thing);                      // 0x0056C860
    CThingPointer& operator=(const CThingPointer& other);   // 0x005ED300
};
bool __fastcall IS_THING_ALIVE(const CBaseIntelligentPointer* pointer);   // 0x004CC340
struct CScriptModuleBase {
    unsigned char _data[0x64];
    CScriptModuleBase(long a, long b);                 // 0x00852710
    void SetTarget(CThing* thing);                     // 0x0072A4A0
    void SetPThingOrigin(CThing* thing);               // 0x00852750
};
struct CScriptModulePointer {                          // CCountedPointer<NCameraModes::CScriptModuleBase>
    CScriptModuleBase* Data;
    void*              Info;
    CScriptModulePointer(CScriptModuleBase* p);        // 0x005ED270
};
struct CGameCameraManager {
    void SetGameScriptedCameraMode(CScriptModulePointer mode, bool immediate);   // 0x00697B70
};
struct CPlayerCameraSource {
    virtual void p00();
    virtual void p01();
    virtual void p02();
    virtual void p03();
    virtual void p04();
    virtual void p05();
    virtual void p06();
    virtual void p07();
    virtual void p08();
    virtual void GetCameraManager(CGameCameraManager** out);
};
struct CEnvironment {
    void SetThemeWeightAllChannels(long channel, long theme, float weight);   // 0x006B38D0
};
struct CWorld {
    unsigned char        _pad00[0x18];
    CEnvironment*        Environment;                  // +0x18
    unsigned char        _pad1c[0x34 - 0x1c];
    CPlayerCameraSource* CameraSource;                 // +0x34
    unsigned long* GetWorldSeed();                     // 0x0049D810
};
struct CFlourishDef {
    virtual void v00();
    virtual void Release();                            // slot 1
    long          RefCount;                            // +0x04
    unsigned char _pad08[0x3c - 0x08];
    long          ThemeWeight;                         // +0x3c
    unsigned char _pad40[0x4c - 0x40];
    long          Theme;                               // +0x4c
    unsigned char _pad50;
    bool          UseCamera;                           // +0x51
};
struct CFlourishDefPointer { CFlourishDef* Data; };
struct CDefinitionManager {
    bool GetDef(long index, CFlourishDefPointer* out);   // 0x0085ABB0
};
struct CThingArray {
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
};
struct CThingFilterInMapNotInArea { long Map; CThing* Centre; };
struct CThingSearchTools {
    void GetAllThings(const CThingFilterInMapNotInArea* filter, CThingArray* things);   // 0x0085B0D0
};
struct SHandednessDef { unsigned char _pad00[0x0c]; long Handedness; };
struct SFlourishInfo { unsigned char _pad00[9]; bool IsFlourish; };
struct CCombatActionDef {
    void GetHandedness(SHandednessDef** out) const;    // 0x00669CB0
    void GetFlourishInfo(SFlourishInfo** out) const;   // 0x00738A00
};
struct SFlourishSource { unsigned char _pad00[0x60]; long DefIndex; };
struct SListHead { SListHead* next; };
class CCombatActionBase {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual long GetMap();
    virtual void v08();
    virtual void v09();
    virtual void v0a();
    virtual void v0b();
    virtual void v0c();
    virtual void v0d();
    virtual void v0e();
    virtual bool IsStrike();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v1a();
    virtual void v1b();
    virtual void v1c();
    virtual void v1d();
    virtual void v1e();
    virtual void v1f();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v2a();
    virtual void v2b();
    virtual void v2c();
    virtual void v2d();
    virtual void v2e();
    virtual void v2f();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v3a();
    virtual void v3b();
    virtual void v3c();
    virtual void v3d();
    virtual void v3e();
    virtual void v3f();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual bool ShouldFaceTarget();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer Owner;                     // +0x08
    unsigned char           _pad10[0x14 - 0x10];
    long                    InterruptFrame;            // +0x14
    unsigned char           _pad18[0x74 - 0x18];
    CCombatActionDef*       Def;                       // +0x74
    unsigned char           _pad78[0xa8 - 0x78];
    CBaseIntelligentPointer Target;                    // +0xa8
    CThingPointer           FlourishTarget;            // +0xb0
    SFlourishSource*        Flourish;                  // +0xb8
    SListHead*              StrikeList;                // +0xbc
    unsigned char           _padc0[0xde - 0xc0];
    bool                    NoInterrupt;               // +0xde
    void SetupCombatStrikeEvents();                    // 0x00859A70
    void SetupCombatInterrupts();                      // 0x00858A20
    void InitialiseActionTimes();                      // 0x00856A20
    void UpdateFacingDirection();                      // 0x00856B20
    CDefinitionManager* GetDefinitionManager() const;  // 0x00692910
    CWorld* GetPWorld() const;                         // 0x00692940
    CThingSearchTools* GetSearchTools() const;         // 0x00692990
    void OnAnimationLoaded();
};

void CCombatActionBase::OnAnimationLoaded()
{
    SetupCombatStrikeEvents();
    SetupCombatInterrupts();
    if (StrikeList->next != StrikeList && !NoInterrupt)
        InterruptFrame = -1;
    InitialiseActionTimes();
    {
        CTCCreatureHitNotification* notification;
        if (IsStrike() && Target.GetPItem() && !(Target.GetPItem()->Flags91 & 1) &&
            Target.GetPItem()->GetInterface(&notification, 0xcd))
            notification->SetHitNotification(Owner.GetPItem());
    }
    {
        SHandednessDef* handedness;
        Def->GetHandedness(&handedness);
        Owner.GetPItem()->SetLastActionHandedness(handedness->Handedness);
    }
    if (IsStrike()) {
        CTCBoss* boss;
        if (Owner.GetPItem()->GetInterface(&boss, 0xad))
            boss->IncrementBossPhase();
    } else {
        CTCInterface30* listener;
        if (Owner.GetPItem()->GetInterface(&listener, 0x30))
            listener->OnCombatAction();
    }
    if (ShouldFaceTarget() && Target.GetPItem() && !(Target.GetPItem()->Flags91 & 1) &&
        Owner.GetPItem() != Target.GetPItem()) {
        CThing* target = Target.GetPItem();
        const C3DVector& from = Owner.GetPItem()->GetPosition();
        const C3DVector& to = target->GetPosition();
        C3DVector delta;
        delta.X = to.X - from.X;
        delta.Y = to.Y - from.Y;
        CThingGraphic* graphic = Owner.GetPItem()->Graphic;
        float turns = (float)atan2(delta.X, delta.Y) * 0.15915494f;
        if (turns < 0.0f || turns >= 1.0f) {
            turns = (float)fmod(turns, 1.0);
            if (turns < 0.0f)
                turns += 1.0f;
        }
        graphic->SetFacingTurns(turns);
    }
    UpdateFacingDirection();
    SFlourishInfo* flourish;
    Def->GetFlourishInfo(&flourish);
    if (!flourish->IsFlourish)
        return;
    CTCFlourishTarget* flourishTarget = 0;
    if (Target.GetPItem() && !(Target.GetPItem()->Flags91 & 1)) {
        Target.GetPItem()->ClearFlag(0x100);
        flourishTarget = (CTCFlourishTarget*)Target.GetPItem()->AddTC(CCharString("CTCFlourishTarget"), true, 0);
        flourishTarget->SetAttacker(Owner.GetPItem());
    }
    if (Flourish == 0 || Flourish->DefIndex < 0)
        return;
    CFlourishDefPointer def;
    def.Data = 0;
    GetDefinitionManager()->GetDef(Flourish->DefIndex, &def);
    CFlourishDef* flourishDef = def.Data;
    if (flourishDef->UseCamera && !(Owner.GetPItem()->m_presentBits[4] & 0x40000)) {
        CGameCameraManager* camera;
        GetPWorld()->CameraSource->GetCameraManager(&camera);
        CThingArray things;
        things._M_start = 0;
        things._M_finish = 0;
        things._M_end_of_storage = 0;
        CThingFilterInMapNotInArea filter;
        CThing* owner = Owner.GetPItem();
        filter.Map = GetMap();
        filter.Centre = owner;
        GetSearchTools()->GetAllThings(&filter, &things);
        if (things._M_start != things._M_finish && IS_THING_ALIVE(&Target)) {
            CThing* chosen = things._M_start[GFRandom(things._M_finish - things._M_start, GetPWorld()->GetWorldSeed())];
            CScriptModuleBase* mode = new CScriptModuleBase(0, -1);
            mode->SetTarget(chosen);
            mode->SetPThingOrigin(Owner.GetPItem());
            camera->SetGameScriptedCameraMode(CScriptModulePointer(mode), true);
            FlourishTarget = CThingPointer(chosen);
            if (flourishTarget) {
                flourishTarget->Active = true;
                flourishTarget->Started = true;
            }
        }
        if (things._M_start)
            free(things._M_start);
    }
    if (flourishDef->Theme > 0)
        GetPWorld()->Environment->SetThemeWeightAllChannels(2, flourishDef->Theme, (float)flourishDef->ThemeWeight);
    if (--flourishDef->RefCount == 0)
        flourishDef->Release();
}
