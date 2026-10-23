// Grade: DIFFER(915v908) -- one store shape: after CThingGameObject::OnCreate retail clears the two 0x15c flag bits with a memory-operand 'and byte ptr [esi+0x15c],0x7d' (7 bytes), ours loads/ands/stores through al (14 bytes); everything else (mode-manager out-param lookup, tint copy + SetScale through a graphic local, three def-index lookups through block-scoped CCharString names, the DEFAULT idle anim, state resets, ability opinion source through a world local, scale override, validate/preload, will-power indicator def chain with in-place def releases) matches
// CThingCreatureBase::OnCreate() -- creature creation: cache the mode manager, copy the def's appearance tint into the
// graphic component and apply the scale, flag the demon-door / oracle-head creatures, start the DEFAULT idle anim,
// run the game-object creation, reset the per-creature state, attach the ability def's opinion source, apply the
// tiny-scale override, start the continuous sounds, validate, preload the anims and read the will-power indicator
// Leaf: ?OnCreate@CThingCreatureBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x00666310, plain `ret` (bool in al).
// Callees: CVectorMap::LowerBound 0x0040F020, CTCGraphicAppearance::SetScale 0x004BFA50, CCharString 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, GetAnimFromName 0x00662A00,
//   CAnimInit::Set 0x0070C050, CTCAnimationComplex::GetAnimation 0x0070B460, CAnimateThing::SetIdleAnim 0x0070D920,
//   CThingGameObject::OnCreate 0x00838A40, ResetCombatState 0x006691C0, GetSubDef 0x00485815, GetWorld 0x004C7AA0,
//   COpinionReactionManager::AttachOpinionSource 0x00700820, CThingPhysical::SetScale 0x006A4B80,
//   PlayCreatureContinuousSounds 0x00665F90, ValidateInitialisation 0x006A4D60, PreloadAnims 0x00663140,
//   GetDefinitionManager 0x004C7990, CDefinitionManager::GetDef 0x00669640, GetSubDef 0x006696B0.
// Layout: retail CThing interface bitset +0x20 / map +0x44, graphic TC +0x64, PBaseDef +0x70, def index +0x8c (word),
//   scale +0x9c; CThingCreatureBase (retail, PDB view 0x10 larger) def +0xe0, will-power +0xe4, +0xfc, +0x10c..+0x114,
//   +0x124, +0x128, mode manager +0x134, +0x138, flag bytes +0x15c..+0x15e.
struct CTCBase;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CWorldHolder { unsigned char _pad[0x10]; CDefinitionManager* DefMgr; };
struct CMainGameComponent { unsigned char _pad[0x24]; CWorldHolder* World; };
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
inline CDefinitionManager* PeekDefMgr() { return g_pMainGameComponent->World->DefMgr; }
extern const float g_TinyCreatureScale;                 // 0x0129BA3C

#pragma pack(push, 2)
struct CAppearanceTint { long A, B, C; short D; };     // 14 bytes
#pragma pack(pop)
struct CTCGraphicAppearance {
    unsigned char   _pad[0x44];
    CAppearanceTint Tint;                               // +0x44
    void SetScale(float scale);                         // 0x004BFA50
};
struct CDefPointee {
    virtual void v0();
    virtual void DestroyDef();                          // slot 1
    long RefCount;                                      // +4
};
struct CAbilityDef : CDefPointee {
    unsigned char _pad08[0xb4 - 0x08];
    long          OpinionSourceID;                      // +0xb4
};
struct CWillPowerIndicatorDef : CDefPointee {
    unsigned char _pad08[0x44 - 0x08];
    long          WillPower;                            // +0x44
};
struct CPhysicalSwitchDef : CDefPointee {
    bool GetSubDef(CWillPowerIndicatorDef** out);       // 0x006696B0
};
struct CDefClassBase {
    bool GetSubDef(CAbilityDef** out);                  // 0x00485815
};
struct CDefinitionManagerG {
    bool GetDef(long index, CPhysicalSwitchDef** out);  // 0x00669640
};
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CAbilityDef** out) { *out = 0; return false; }
__declspec(noinline) bool CPhysicalSwitchDef::GetSubDef(CWillPowerIndicatorDef** out) { *out = 0; return false; }
__declspec(noinline) bool CDefinitionManagerG::GetDef(long index, CPhysicalSwitchDef** out) { *out = 0; return false; }
#pragma auto_inline(on)
struct CCreatureDef {
    unsigned char   _pad00[0xf0];
    long            ScaleType;                          // +0xf0
    unsigned char   _padf4[0x100 - 0xf4];
    CAppearanceTint Tint;                               // +0x100
    unsigned char   _pad10e[0x124 - 0x10e];
    long            WillPowerIndicatorDef;              // +0x124
};
struct CAnimationEntry { long AnimIndex; };
struct CAnimInit {
    long          Mode;
    unsigned char AnimType;
    bool          Flag05;
    long          AnimIndex;
    float         Scales[3];
    float         Blend[2];
    unsigned long Flags;
    long          Arg24;
    long          Percent;
    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent);   // 0x0070C050
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    {
        Blend[0] = 0.0f;
        Blend[1] = 0.0f;
        Set(animType, arg24, animIndex, mode, flag05, flags, percent);
    }
};
struct CAnimateThing { void SetIdleAnim(const CAnimInit& init); };   // 0x0070D920
struct CTCAnimationComplex { CAnimateThing& GetAnimation(); };      // 0x0070B460
struct COpinionReactionManager {
    bool AttachOpinionSource(class CThingCreatureBase* thing, long id, void* source);   // 0x00700820
};
struct CWorld { unsigned char _pad[0x74]; COpinionReactionManager* OpinionReactionManager; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThingCreatureBase {
public:
    unsigned char         _pad00[0x20];
    unsigned long         UsingComponents[9];           // +0x20
    CInterfaceMap         Components;                   // +0x44
    unsigned char         _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* GraphicTC;                    // +0x64
    unsigned char         _pad68[0x70 - 0x68];
    CDefClassBase*        PBaseDef;                     // +0x70
    unsigned char         _pad74[0x8c - 0x74];
    unsigned short        DefIndex;                     // +0x8c
    unsigned char         _pad8e[0x9c - 0x8e];
    float                 Scale;                        // +0x9c
    unsigned char         _padA0[0xe0 - 0xa0];
    CCreatureDef*         Def;                          // +0xe0
    long                  WillPower;                    // +0xe4
    unsigned char         _padE8[0xfc - 0xe8];
    long                  Field_fc;                     // +0xfc
    unsigned char         _pad100[0x10c - 0x100];
    long                  Field_10c;                    // +0x10c
    long                  Field_110;                    // +0x110
    long                  Field_114;                    // +0x114
    unsigned char         _pad118[0x124 - 0x118];
    long                  Field_124;                    // +0x124
    long                  Field_128;                    // +0x128
    unsigned char         _pad12c[0x134 - 0x12c];
    CTCBase*              ModeManager;                  // +0x134
    long                  Field_138;                    // +0x138
    unsigned char         _pad13c[0x15c - 0x13c];
    bool                  Flag15c_0 : 1;                // +0x15c
    bool                  Flag15c_1 : 1;
    bool                  Flag15c_2 : 1;
    bool                  Flag15c_3 : 1;
    bool                  Flag15c_4 : 1;
    bool                  Flag15c_5 : 1;
    bool                  Flag15c_6 : 1;
    bool                  Flag15c_7 : 1;
    bool                  Flag15d_0 : 1;                // +0x15d
    bool                  Flag15d_1 : 1;
    bool                  Flag15d_2 : 1;
    bool                  IsDemonDoor : 1;
    bool                  IsOracleHead : 1;
    bool                  Flag15d_5 : 1;
    bool                  Flag15d_6 : 1;
    bool                  Flag15d_7 : 1;
    bool                  Flag15e_0 : 1;                // +0x15e
    template<class T> __forceinline bool GetInterface(T** out, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    CAnimationEntry* GetAnimFromName(const CCharString& name, long index);   // 0x00662A00
    unsigned char BaseOnCreate();                       // 0x00838A40 (CThingGameObject::OnCreate)
    void ResetCombatState();                            // 0x006691C0
    CWorld* GetWorld();                                 // 0x004C7AA0
    void SetScale(float scale);                         // 0x006A4B80 (CThingPhysical)
    void PlayCreatureContinuousSounds();                // 0x00665F90
    bool ValidateInitialisation();                      // 0x006A4D60
    void PreloadAnims();                                // 0x00663140
    CDefinitionManagerG* GetDefinitionManager();        // 0x004C7990
    bool OnCreate();
};
template<class T> struct CDefPointer {
    T* Obj;
    CDefPointer() : Obj(0) {}
    ~CDefPointer()
    {
        if (Obj && --Obj->RefCount == 0)
            Obj->DestroyDef();
    }
};

bool CThingCreatureBase::OnCreate()
{
    ModeManager = 0;
    GetInterface(&ModeManager, 0x31);
    CTCGraphicAppearance* graphic = GraphicTC;
    if (graphic) {
        graphic->Tint = Def->Tint;
        graphic->SetScale(Scale);
    }
    long demonDoor;
    {
        CCharString name("CREATURE_DEMON_DOOR_FACE_01");
        demonDoor = PeekDefMgr()->GetDefGlobalIndexFromName(name);
    }
    long nwDemonDoor;
    {
        CCharString name("CREATURE_NW_DEMON_DOOR_FACE_01");
        nwDemonDoor = PeekDefMgr()->GetDefGlobalIndexFromName(name);
    }
    IsDemonDoor = DefIndex == demonDoor || DefIndex == nwDemonDoor;
    long oracleHead;
    {
        CCharString name("CREATURE_ORACLE_HEAD_01");
        oracleHead = PeekDefMgr()->GetDefGlobalIndexFromName(name);
    }
    IsOracleHead = DefIndex == oracleHead;
    Flag15c_0 = false;
    CTCAnimationComplex* anim;
    if (GetInterface(&anim, 0x5a)) {
        CCharString name("DEFAULT");
        anim->GetAnimation().SetIdleAnim(CAnimInit(6, 0, GetAnimFromName(name, 0)->AnimIndex, 1, true, 0, 100));
    }
    Flag15c_2 = false;
    Flag15c_3 = false;
    if (!BaseOnCreate())
        return false;
    Flag15c_1 = false;
    Flag15c_7 = false;
    Field_114 = 0;
    Field_110 = 0;
    Field_10c = 0;
    ResetCombatState();
    Field_fc = 0;
    Flag15e_0 = false;
    Field_128 = 0;
    Field_124 = 0;
    Field_138 = 2;
    CDefPointer<CAbilityDef> ability;
    PBaseDef->GetSubDef(&ability.Obj);
    if (ability.Obj && ability.Obj->OpinionSourceID)
    {
        CWorld* world = GetWorld();
        world->OpinionReactionManager->AttachOpinionSource(this, ability.Obj->OpinionSourceID, 0);
    }
    if (Def->ScaleType == 4)
        SetScale(g_TinyCreatureScale);
    PlayCreatureContinuousSounds();
    if (ValidateInitialisation()) {
        PreloadAnims();
        CDefPointer<CPhysicalSwitchDef> def;
        CDefPointer<CWillPowerIndicatorDef> indicator;
        long indicatorIndex = Def->WillPowerIndicatorDef;
        WillPower = 50;
        if (indicatorIndex && GetDefinitionManager()->GetDef(indicatorIndex, &def.Obj)) {
            if (def.Obj->GetSubDef(&indicator.Obj))
                WillPower = indicator.Obj->WillPower;
        }
        return true;
    }
    return false;
}
