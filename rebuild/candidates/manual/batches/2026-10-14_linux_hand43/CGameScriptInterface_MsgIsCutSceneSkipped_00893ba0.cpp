// CGameScriptInterface::MsgIsCutSceneSkipped() const -- true when there is a message of type 0x32 that can be seen or heard between the current and the maximum world frame (0x006E7510, 0x006E7530; the bound filter through CWorld+0x60, 0x008ABED0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00893BA0, length 81 bytes.
// Callees: the two frame getters @ 0x006E7510 / 0x006E7530 (both labelled GetMaxWorldFrameForMessages), CMessageEventManager::FindMostRecentMessage<CFilter_Bind2<IsOfType, CanBeSeenOrHeard>> @ 0x008ABED0 (the bound filter by const reference). Shape: 0x14-byte frame: the type filter at +8, the frame filter at +0xc..+0x10, the Bind2 pair of pointers at +0x14..+0x18; `neg ; sbb ; neg` for the non-null result, `ret`.
// Leaf: MsgIsCutSceneSkipped
struct CDisplayEngine_Retail;
struct CPlayerManager_Retail;
struct CMainGameComponent_Retail;
struct CDefinitionManager_Retail;
struct CWorld;
struct CCharString;
struct CScriptThing;
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
struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};
struct CMessageEventFilter_CanBeSeenOrHeard {
    long m_frame;
    long m_maxFrame;
    CMessageEventFilter_CanBeSeenOrHeard(long frame, long maxFrame) : m_frame(frame), m_maxFrame(maxFrame) { }
};
template<class A, class B> struct CFilter_Bind2 {   // the bound filter holds POINTERS to its operands (landed 0x00939D70)
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
struct CWorld {
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
struct CGameScriptInterface {
    virtual void v000();
    CWorld*                     World;              // +0x04 (engine/CGameScriptInterface.h)
    CMainGameComponent_Retail*  Component;          // +0x08
    CDisplayEngine_Retail*      DisplayEngine;      // +0x0c
    CDefinitionManager_Retail*  DefinitionManager;  // +0x10
    CPlayerManager_Retail*      PlayerManager;      // +0x14
    long GetTextIDFromSymbol(const CCharString& symbol) const;   // 0x006E77F0 (labelled LTextSystem::GetIDFromSymbol; `this` is unused)
    long GetCurrentWorldFrameForMessages() const;   // 0x006E7510 (labelled GetMaxWorldFrameForMessages)
    long GetMaxWorldFrameForMessages() const;       // 0x006E7530
    bool MsgIsCutSceneSkipped() const;
};

bool CGameScriptInterface::MsgIsCutSceneSkipped() const
{
    long frame = GetCurrentWorldFrameForMessages();
    long maxFrame = GetMaxWorldFrameForMessages();
    CMessageEventManager_Retail* messages = World->PMessageEventManager_Data;
    CMessageEvent_Retail* message = messages->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x32) && CMessageEventFilter_CanBeSeenOrHeard(frame, maxFrame));
    return message != 0;
}
