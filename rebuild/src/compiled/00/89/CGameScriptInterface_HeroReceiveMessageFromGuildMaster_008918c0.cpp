// CGameScriptInterface::HeroReceiveMessageFromGuildMaster(const CCharString& text, const CCharString& symbol, bool flag, bool force) const -- when creature generation is enabled in the world (CWorld+0xe0) and either `force` is set or the user profile allows it (the singleton @ 0x0040D2A0: the +0xd8 setting when a profile is loaded (+0xc), else the +0x18 default), show the guild master message in the game UI (0x013B8790, 0x00437800) with the text, the id of the symbol (0x006E77F0) and the flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008918C0, length 87 bytes.
// Callees: CTBaseSingleton<CUserProfileManager>::Get @ 0x0040D2A0, the symbol id lookup @ 0x006E77F0 (labelled LTextSystem::GetIDFromSymbol; `this` unused), the game UI call @ 0x00437800 (the label is propagated). Shape: the interface in esi, the profile setting chosen in al through a two-way branch, the id call before the flag and text are pushed, `ret 10h`.
// Leaf: HeroReceiveMessageFromGuildMaster
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
typedef unsigned short wchar_t;                // VC7.1 without /Zc:wchar_t: the type of L""
struct CCharStringData_Retail { const char* m_pData; unsigned long m_length; };   // CBasicString<char>: PData, StringLength
class CWideString { public:
    void* PStringData;
    CWideString(const CWideString& other);         // 0x0099B720
    ~CWideString();                                // 0x0099B510
    bool operator!=(const wchar_t* text) const;    // 0x0099B150
};
class CCharString { public:
    CCharStringData_Retail* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    CWideString ToWideString() const;               // 0x0099E480
    const char* GetString() const { return m_pData == 0 ? "" : m_pData->m_pData; }   // inline: "" @ 0x0129AAF4 without a data block (the null case falls through)
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
};
struct CThing_Retail;
struct CFontBank_Retail;
class CRGBColour;
struct CSubtitleRenderer_Retail {               // embedded at CGameUI+0x18 (`add ecx,18h`)
    void SetText(CFontBank_Retail* font, const CCharString& text, unsigned long colour, float time, bool a, bool b);   // 0x00492ED1
    void SetText(CFontBank_Retail* font, const CCharString& text, const CCharString& name, const CCharString& a, const CCharString& b, unsigned long colour, float time, bool c, bool d);   // 0x00492FB4
};
struct CMiniMapDisplay_Retail {
    void AddMarker(CThing_Retail* thing, long id);         // 0x0082A890 (CMiniMapDisplay::AddMarker)
    void SetMarkerGraphic(CThing_Retail* thing, long id);  // 0x0082A340 (CMiniMapDisplay::SetMarkerGraphic)
    void RemoveMarker(CThing_Retail* thing);               // 0x0082A380 (the label is propagated)
};
struct CDrawQuestInfo_Retail {
    long AddBarHealth(CThing_Retail* thing, const CRGBColour& colour, const CCharString& name, float value);   // 0x0064F885 (CDrawQuestInfo::AddBarHealth)
};
struct CGameUI_Retail {                          // the game UI singleton behind the pointer at 0x013B8790
    unsigned char            _pad00[0x18];
    CSubtitleRenderer_Retail m_subtitles;        // +0x18
    unsigned char            _pad19[0x1af];
    CDrawQuestInfo_Retail*   m_pQuestInfo;       // +0x1c8
    CDrawQuestInfo_Retail* GetQuestInfo() { return m_pQuestInfo; }
    CMiniMapDisplay_Retail* GetMiniMapDisplay(); // 0x00437CE0 (+0x160 -> +0x28; the label "CVertexBufferWin32::DoSizeof" is propagated)
    void ShowGuildMasterMessage(const CCharString& text, long id, bool flag);   // 0x00437800 (the label is propagated)
    void ShowOnScreenMessage(const CWideString& text, long a, long b, long c);   // 0x0043A2A0 (labelled ShowOnScreenMessage_FormatDisplay)
};
extern CGameUI_Retail* g_pGameUI_013b8790;      // 0x013B8790
struct CMainGameComponent_Retail {
    unsigned char    _pad[0x1614c];
    CFontBank_Retail* m_pFontBank;               // +0x1614c
};
struct CUserProfileManager_Retail {
    unsigned char _pad00[0xc];
    bool          m_profileLoaded;               // +0x0c
    unsigned char _pad0d[0xb];
    bool          m_defaultSetting;              // +0x18
    unsigned char _pad19[0xbf];
    bool          m_profileSetting;              // +0xd8
    static CUserProfileManager_Retail& Get();    // 0x0040D2A0 (CTBaseSingleton<CUserProfileManager>::Get)
};
class CGameScriptInterface { public:
    virtual void v000();
    CWorld*                     World;              // +0x04 (engine/CGameScriptInterface.h)
    CMainGameComponent_Retail*  Component;          // +0x08
    CDisplayEngine_Retail*      DisplayEngine;      // +0x0c
    CDefinitionManager_Retail*  DefinitionManager;  // +0x10
    CPlayerManager_Retail*      PlayerManager;      // +0x14
    long GetTextIDFromSymbol(const CCharString& symbol) const;   // 0x006E77F0 (labelled LTextSystem::GetIDFromSymbol; `this` is unused)
    long GetCurrentWorldFrameForMessages() const;   // 0x006E7510 (labelled GetMaxWorldFrameForMessages)
    long GetMaxWorldFrameForMessages() const;       // 0x006E7530
    void HeroReceiveMessageFromGuildMaster(const CCharString& text, const CCharString& symbol, bool flag, bool force) const;
};

void CGameScriptInterface::HeroReceiveMessageFromGuildMaster(const CCharString& text, const CCharString& symbol, bool flag, bool force) const
{
    if (World->CreatureGenerationEnabled) {
        bool allowed = force;
        if (!allowed) {
            CUserProfileManager_Retail& profile = CUserProfileManager_Retail::Get();
            allowed = profile.m_profileLoaded ? profile.m_profileSetting : profile.m_defaultSetting;
        }
        if (allowed) {
            long id = GetTextIDFromSymbol(symbol);
            g_pGameUI_013b8790->ShowGuildMasterMessage(text, id, flag);
        }
    }
}