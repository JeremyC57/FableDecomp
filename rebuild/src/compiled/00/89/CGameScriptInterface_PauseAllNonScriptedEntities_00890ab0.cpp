// CGameScriptInterface::PauseAllNonScriptedEntities(bool pause) -- tell the world's thing manager (CWorld slot 3) to pause the non-scripted entities (0x0051EFC0), then add the 0xBEEFBEEF bullet-time slowdown (CWorld+0x68, priority 0xFFFF, multiplier 1.0f; 0x004C6170) or remove it (0x004C6140)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00890AB0, length 80 bytes.
// Callees: CWorld virtual slot 3 (+0xc) GetThingManager, CThingManager::SetNonScriptedEntitesPaused @ 0x0051EFC0, CBulletTimeManager::SetBulletTimeSlowdownMultiplier @ 0x004C6170 / RemoveBulletTimeSlowdown @ 0x004C6140 (label propagated). Shape: the flag kept in ebx across the first call, the bullet-time manager re-read through World in both arms, `ret 4`.
// Leaf: PauseAllNonScriptedEntities
struct CDisplayEngine_Retail;
struct CPlayerManager_Retail;
struct CMainGameComponent_Retail;
struct CDefinitionManager_Retail;
class CWorld;
class CCharString;
class CScriptThing;
struct CDefPointer_Retail;
struct CTCBase_Retail;
struct CInterfaceMap_Retail {                 // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase_Retail* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020 (a second copy of the same body sits at 0x004365B0)
};
struct CThingPhysics_Retail;
struct CThing_Retail {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual CThing_Retail* GetStandingOnThing();    // slot 38 (+0x98)
    unsigned char         _pad04[0xc];              // +0x04 .. +0x10
    unsigned char         m_type;                   // +0x10 (1 = the player creature, 2 = an AI creature, 3 = a building)
    unsigned char         _pad11[0xf];
    unsigned long         m_presentBits[9];         // +0x20 std::bitset of the interface types present
    CInterfaceMap_Retail  m_interfaces;             // +0x44 the sorted {type, component} map
    unsigned char         _pad4c[0x14];
    CThingPhysics_Retail* m_pPhysics;               // +0x60
    CThingPhysics_Retail* GetPhysics() { return m_pPhysics; }
    unsigned char         _pad64[8];
    unsigned char         m_flags;                  // +0x6c (bit 1 = physical, bit 3 = creature)
    unsigned char         _pad6d[0x24];
    unsigned char         m_dead : 1;               // +0x91 bit 0
    unsigned char         m_91_1 : 7;
    unsigned char         m_92_0 : 8;               // +0x92
    unsigned char         m_93_0 : 1;               // +0x93
    unsigned char         m_killedByScript : 1;     //       bit 1
    unsigned char         m_93_2 : 6;
    void Kill(bool flag);                           // 0x004C9B80 (CThing::Kill)
    CScriptThing GetScriptThing();                  // 0x004C7E50 (CThing::GetScriptThing, by value through the hidden result pointer)
    void AssignHomeBuilding(CThing_Retail* building);   // 0x00512210 (landed as AssignTarget_00512210; the label "CVillageTaskCarryCrate::AssignCreatureToTask" is propagated)
    void SetNewBrain(CDefPointer_Retail brain);     // 0x00833010 (CThingAICreature::SetNewBrain, the counted def pointer by value)
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap_Retail* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap_Retail::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    unsigned long HasInterface(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }   // the masked word (a bool would narrow the test)
};
struct CThingManager_Retail {
    unsigned char _pad[0x55];
    bool          m_allEntitiesPaused;              // +0x55
    void SetNonScriptedEntitesPaused(bool paused);  // 0x0051EFC0
};
struct CBulletTimeManager_Retail {
    void SetBulletTimeSlowdownMultiplier(long priority, unsigned long key, float multiplier);   // 0x004C6170
    void RemoveBulletTimeSlowdown(unsigned long key);   // 0x004C6140 (the label "CTCInventoryExperience::ProcessButtonBReleased" is propagated)
};
struct CMessageEvent_Retail;
class CMessageEventFilter_IsOfType { public:
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};
class CMessageEventFilter_CanBeSeenOrHeard { public:
    long m_frame;
    long m_maxFrame;
    CMessageEventFilter_CanBeSeenOrHeard(long frame, long maxFrame) : m_frame(frame), m_maxFrame(maxFrame) { }
};
template<class A, class B> class CFilter_Bind2 { public:   // the bound filter holds POINTERS to its operands (landed 0x00939D70)
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard> CSeenOrHeardFilter;
struct CMessageEventManager_Retail {
    CMessageEvent_Retail* FindMostRecentMessage(const CSeenOrHeardFilter& filter);   // 0x008ABED0
    void RemoveMessage(const CMessageEvent_Retail* message);   // 0x00493B5B
};
struct CBaseIntelligentPointer_Retail {
    virtual void v00();
    void* m_pItem;                                  // +0x04
    void* GetPItem();                               // 0x00A01B50 (CBaseIntelligentPointer::GetPItem)
};
struct CTCOpinionSource_Retail;
struct COpinionReactionManager_Retail {
    unsigned char                  _pad[0x714];
    CBaseIntelligentPointer_Retail m_exclusiveSource;   // +0x714 (a CIntelligentPointer)
    void* GetExclusiveOpinionSource() { return m_exclusiveSource.GetPItem(); }   // inline: the intelligent pointer's item
    const CCharString& GetThingAttitudeToPlayerAsString(CThing_Retail* thing);   // 0x007018B0
    float GetOpinionValue(CThing_Retail* thing, long opinion);   // 0x00701750 (labelled COpinionReactionManager_GetOpinionValue)
    bool AttachOpinionSource(CThing_Retail* thing, const CCharString& name, CTCOpinionSource_Retail** ppSource);   // 0x00700780
    bool AttachOpinionSource(CThing_Retail* thing, long id, CTCOpinionSource_Retail** ppSource);   // 0x00700820
    void ClearOpinionSource(CThing_Retail* thing, bool flag);   // 0x007008C0
    void SetExclusiveOpinionSource(CThing_Retail* thing);   // 0x00700770 (16 bytes before AttachOpinionSource; the manifest label is propagated)
};
class CWorld { public:
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual CThingManager_Retail* GetThingManager();   // slot 3 (+0x0c)
    unsigned char                   _pad04[0x5c];               // +0x04 .. +0x60
    CMessageEventManager_Retail*    PMessageEventManager_Data;  // +0x60 (engine/CWorld.h)
    unsigned char                   _pad64[4];
    CBulletTimeManager_Retail*      PBulletTimeManager_ptr;     // +0x68
    unsigned char                   _pad6c[8];
    COpinionReactionManager_Retail* POpinionReactionManager_Data;   // +0x74
    unsigned char                   _pad78[0x68];
    bool                            CreatureGenerationEnabled;  // +0xe0
    COpinionReactionManager_Retail* GetOpinionReactionManager() { return POpinionReactionManager_Data; }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::PauseAllNonScriptedEntities(bool pause)
{
    World->GetThingManager()->SetNonScriptedEntitesPaused(pause);
    if (pause)
        World->PBulletTimeManager_ptr->SetBulletTimeSlowdownMultiplier(0xFFFF, 0xBEEFBEEF, 1.0f);
    else
        World->PBulletTimeManager_ptr->RemoveBulletTimeSlowdown(0xBEEFBEEF);
}