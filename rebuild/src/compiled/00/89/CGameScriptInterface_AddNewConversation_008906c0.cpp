// CGameScriptInterface::AddNewConversation(const CScriptThing& thing, bool a, bool b) const -- when the script thing is valid, make a new conversation for its thing in the world's script conversation manager (CWorld+0x7c, 0x006E61A0 with the two flags); else 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008906C0, length 80 bytes.
// Callees: the script thing validated through its virtual slot 75 (+0x12c), the thing fetched through slot 11 (+0x2c), CScriptConversationManager::MakeNewConversation @ 0x006E61A0. Shape: the interface in edi then the manager, the two flags pushed before the thing fetch, `xor eax,eax` on the invalid path, `ret 0Ch`.
// Leaf: ?AddNewConversation@CGameScriptInterface@@UBEJABVCScriptThing@@_N1@Z
struct CDisplayEngine_Retail;
struct CPlayerManager_Retail;
class CWorld;
struct CThingPhysics_Retail;
struct CThing_Retail {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void SetAsDrawable(bool drawable);      // slot 28 (+0x70)
    unsigned char        _pad04[0xc];              // +0x04 .. +0x10
    unsigned char        m_type;                   // +0x10 (1 = the player creature, 2 = an AI creature)
    unsigned char        _pad11[0x4f];
    CThingPhysics_Retail* m_pPhysics;              // +0x60
    unsigned char        _pad64[8];
    CThingPhysics_Retail* GetPhysics() { return m_pPhysics; }
    unsigned char        m_flags;                  // +0x6c (bit 1 = physical, bit 3 = creature)
    unsigned char        _pad6d[0x24];
    unsigned char        m_dead : 1;               // +0x91 bit 0
    unsigned char        m_91_1 : 7;
    unsigned char        m_92_0 : 4;               // +0x92
    unsigned char        m_gameStatePersisted : 1; //       bit 4
    unsigned char        m_92_5 : 1;
    unsigned char        m_limboKeepsScript : 1;   //       bit 6
    unsigned char        _pad93[0x29];
    unsigned char        m_damageable : 1;         // +0xbc bit 0
    unsigned char        m_bc_1 : 1;
    unsigned char        m_respondToHit : 1;       //       bit 2
    unsigned char        _padbd[0x17];
    unsigned char        m_houseLockingOverridden : 1;   // +0xd4 bit 0
    unsigned char        _padd5[0xf];
    long                 m_shotAccuracyPercentage; // +0xe4
    unsigned char        _pade8[0x74];
    unsigned char        m_15c_0 : 6;              // +0x15c
    unsigned char        m_followsHeroThroughTeleporters : 1;   // bit 6
    unsigned char        m_15c_7 : 1;
    unsigned char        m_useMovementInActions : 1;            // +0x15d bit 0
    unsigned char        m_15d_1 : 7;
    unsigned char        _pad15e[0x32];
    float                m_aiMaxWalkingSpeed;      // +0x190
    unsigned char        _pad194[4];
    float                m_aiMaxRunningSpeed;      // +0x198
    unsigned char        _pad19c[0x1c];
    float                m_playerMaxWalkingSpeed;  // +0x1b8
    unsigned char        _pad1bc[4];
    float                m_playerMaxRunningSpeed;  // +0x1c0
    void SetMaxHealth(float health, bool flag);    // 0x006A5260 (CThingPhysical)
    bool IsPickPocketable();                       // 0x00665E90 (CThingCreatureBase)
    void SetHiddenOnMiniMap(bool hidden);          // 0x006626A0 (CThingCreatureBase)
    void SetHasInformation(long kind, bool a, bool b, bool c);   // 0x00830EB0 (CThingAICreature)
    void SetAsGameStatePersisted(bool persisted);  // 0x004C7880 (CThing)
    void ResetMaxRunningSpeedToDefault();          // 0x006A9BC0 (CThingPlayerCreature)
    void ResetMaxWalkingSpeedToDefault();          // 0x006A9B70 (CThingPlayerCreature; the manifest repeats the running label)
    void RemoveMovementBlockingModes();            // 0x0085F600
    void SetCutscenePauseBehaviour(long behaviour);   // 0x004C78A0 (CThing)
    long GetSex();                                 // 0x00661FF0 (CThingCreatureBase; the label "CPixelShader::CountReferences" is propagated)
    void PreloadBasicAnimations();                 // 0x00663610 (CThingCreatureBase)
    void SetInLimbo(bool inLimbo);                 // 0x004C8CF0 (CThing)
};
class CScriptThing { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing_Retail* GetThing() const;        // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsValid() const;                  // slot 75 (+0x12c)
};
class CCharString;
struct CRegion_Retail {
    unsigned char _pad[0x18];
    unsigned char m_name[4];                       // +0x18 (a CCharString)
};
struct CThingManager_Retail {
    CThing_Retail* GetThingWithUID(__int64 uid);   // 0x0051ED80 (the label "CStopWatch::GetTicks" is propagated)
    void OnThingKill(CThing_Retail* thing);        // 0x00520C80
};
struct CWorldMap_Retail {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009();
    virtual bool IsMapLoaded(long map);            // slot 10 (+0x28)
    long GetMapNumberFromScriptName(const CCharString& name);   // 0x004FB880
    bool IsMapLoadedOnPlayerProximity(long map);   // 0x004FD000
    long GetCurrentRegionNumber();                 // 0x004FB150 (the label "CWorld::DrawGetEnvironment" is propagated)
    CRegion_Retail* GetRegion(long region);        // 0x004FC180
    void SetRegionsAsConnectedInGraph(long a, long b, long c);   // 0x004FC970
    long GetRegionNumberFromName(const CCharString& name);      // 0x004FC210
    void ApplyScriptBrush(const CCharString& name, long flag);  // 0x004FE510 (the label "CDrawQuestInfo::OracleSymbolPushBack" is propagated)
};
struct CEnvironmentSpellEffectInterpolator_Retail;
struct CEnvironment_Retail {
    unsigned char _pad[0x1964];
    CEnvironmentSpellEffectInterpolator_Retail* m_pSpellEffects;   // +0x1964
    void TransitionThemeToDefault(long priority, long flag, float time);   // 0x006B3880 (labelled "CEnvironmentThemePriorityStackEntry_TransitionToDefault")
    CEnvironmentSpellEffectInterpolator_Retail* GetSpellEffects() { return m_pSpellEffects; }
};
enum EOpinionAttitudeType { OPINION_ATTITUDE_NONE = 0 };
struct COpinionReactionManager_Retail {
    EOpinionAttitudeType GetOpinionAttitudeToPlayer(CThing_Retail* thing);   // 0x00701840
};
struct CScriptConversationManager_Retail {
    long MakeNewConversation(CThing_Retail* thing, bool a, bool b);   // 0x006E61A0
    void AddPersonToConversation(long conversation, CThing_Retail* thing);   // 0x006E5800
};
class CWorld { public:
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual CThingManager_Retail* GetThingManager();   // slot 3 (+0x0c)
        virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual CWorldMap_Retail* GetWorldMapForEdit();  // slot 12 (+0x30)
    virtual CWorldMap_Retail* GetWorldMap();         // slot 13 (+0x34)
    unsigned char                        _pad04[0x10];
    CWorldMap_Retail*                    PWorldMap_ptr;          // +0x14 (engine/CWorld.h)
    CEnvironment_Retail*                 PEnvironment_ptr;       // +0x18
    unsigned char                        _pad1c[0x58];
    COpinionReactionManager_Retail*      POpinionReactionManager_Data;   // +0x74
    unsigned char                        _pad78[4];
    CScriptConversationManager_Retail*   PScriptConversationManager_ptr; // +0x7c
    CEnvironment_Retail* GetEnvironment() { return PEnvironment_ptr; }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

long CGameScriptInterface::AddNewConversation(const CScriptThing& thing, bool a, bool b) const
{
    if (thing.IsValid()) {
        CScriptConversationManager_Retail* conversations = World->PScriptConversationManager_ptr;
        return conversations->MakeNewConversation(thing.GetThing(), a, b);
    }
    return 0;
}