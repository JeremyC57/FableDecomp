// CGameScriptInterface::MsgOnHeroHairTypeChanged(EClothingCoversArea area, CCharString& name) const -- the def name of the new hair type in the most recent "hair type changed" message for the area (0x10 -> type 0x39, 0x80 -> 0x3A, else 0x3B) seen or heard this frame
// Leaf: ?MsgOnHeroHairTypeChanged@CGameScriptInterface@@UBE_NW4EClothingCoversArea@@AAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00893C00, length 373 bytes (`ret 8`, bool in al).
// Callees: the frame getters, FindMostRecentMessage @ 0x008ABED0, CDefinitionManager::GetDefNameFromGlobalIndex @ 0x009ACCC0,
//   CDefStringTable::GetString @ 0x009D49B0 on the global table @ 0x013CA828, CCharString::operator= @ 0x0099EFB0,
//   ~CCharString @ 0x0099EAE0.
// Shape: the landed MsgOnExpressionPerformed 0x00893B00 body (same TU) once per area arm, each arm with its own success
//   tail; the IsOfType filter is homed in the dead area slot.
class CCharString { public:
    void* m_pData;
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    const CCharString& operator=(const char* text);           // 0x0099EFE0
    ~CCharString();                                  // 0x0099EAE0
};
struct CMessageEvent_Retail {
    unsigned char _pad[0x3c];
    long          m_extraData;                      // +0x3c (the message's payload word: region/quest index, feat id, record pointer ...)
};
class CMessageEventFilter_IsOfType { public:
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};
class CMessageEventFilter_CanBeSeenOrHeard { public:
    long m_frame;
    long m_maxFrame;
    CMessageEventFilter_CanBeSeenOrHeard(long frame, long maxFrame) : m_frame(frame), m_maxFrame(maxFrame) { }
};
template<class A, class B> class CFilter_Bind2 { public:   // the bound filter holds POINTERS to its operands
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard> CSeenOrHeardFilter;
struct CMessageEventManager_Retail {
    CMessageEvent_Retail* FindMostRecentMessage(const CSeenOrHeardFilter& filter);   // 0x008ABED0
};
class CDefString { public:                              // a def name: its offset into the def string table
    long m_index;
    CDefString(const CDefString& other) : m_index(other.m_index) { }   // user copy ctor: returned through the hidden pointer
};
struct CDefinitionManager_Retail {
    CDefString GetDefNameFromGlobalIndex(long index);   // 0x009ACCC0
};
struct CDefStringTable_Retail {
    CCharString GetString(long index);              // 0x009D49B0
};
extern CDefStringTable_Retail g_defStringTable;     // 0x013CA828
struct CWorldMap_Retail;
struct CDefinitionManager_Retail;
struct CPlayerManager_Retail;
struct CRegion_Retail {
    unsigned char _pad[0x18];
    CCharString   m_name;                           // +0x18
};
struct CWorldMap_Retail {
    CRegion_Retail& GetRegion(long index);          // 0x004FC180
};
class CWorld { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012();
    virtual CWorldMap_Retail* PeekWorldMap();       // slot 13 (+0x34)
    unsigned char                _pad04[0x5c];
    CMessageEventManager_Retail* PMessageEventManager_Data;   // +0x60 (engine/CWorld.h)
};
enum EClothingCoversArea { CLOTHING_COVERS_HAIR = 0x10, CLOTHING_COVERS_FACIAL_HAIR = 0x80 };   // the two tested bits (names descriptive)
class CGameScriptInterface { public:
    virtual void v000();
    CWorld* World;                                  // +0x04 (engine/CGameScriptInterface.h)
    void*   Component;                              // +0x08
    void*   DisplayEngine;                          // +0x0c
    CDefinitionManager_Retail* DefinitionManager;   // +0x10
    CPlayerManager_Retail*     PlayerManager;       // +0x14
protected:
    long GetMinWorldFrameForMessages() const;   // 0x006E7510
    long GetMaxWorldFrameForMessages() const;       // 0x006E7530
public:
    virtual bool MsgOnHeroHairTypeChanged(EClothingCoversArea area, CCharString& name) const;
};


bool CGameScriptInterface::MsgOnHeroHairTypeChanged(EClothingCoversArea area, CCharString& name) const
{
    long frame = GetMinWorldFrameForMessages();
    long maxFrame = GetMaxWorldFrameForMessages();
    CMessageEventManager_Retail* messages = World->PMessageEventManager_Data;
    if (area == CLOTHING_COVERS_HAIR) {
        CMessageEvent_Retail* message = messages->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x39) && CMessageEventFilter_CanBeSeenOrHeard(frame, maxFrame));
        if (message) {
            CCharString text = g_defStringTable.GetString(DefinitionManager->GetDefNameFromGlobalIndex(message->m_extraData).m_index);
            name = text;
            return true;
        }
    } else if (area == CLOTHING_COVERS_FACIAL_HAIR) {
        CMessageEvent_Retail* message = messages->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x3a) && CMessageEventFilter_CanBeSeenOrHeard(frame, maxFrame));
        if (message) {
            CCharString text = g_defStringTable.GetString(DefinitionManager->GetDefNameFromGlobalIndex(message->m_extraData).m_index);
            name = text;
            return true;
        }
    } else {
        CMessageEvent_Retail* message = messages->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x3b) && CMessageEventFilter_CanBeSeenOrHeard(frame, maxFrame));
        if (message) {
            CCharString text = g_defStringTable.GetString(DefinitionManager->GetDefNameFromGlobalIndex(message->m_extraData).m_index);
            name = text;
            return true;
        }
    }
    return false;
}