// CGameScriptInterface::MiniMapSetMarkerGraphic(const CScriptThing& thing, const CCharString& symbol) const -- when the script thing is valid and the symbol has a positive id (0x006E77F0), set the mini-map marker graphic of its thing to that id (CMiniMapDisplay::SetMarkerGraphic @ 0x0082A340 on the game UI's display @ 0x00437CE0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00891FA0, length 70 bytes.
// Callees: the script thing validated through its virtual slot 75 (+0x12c), the thing fetched through slot 11 (+0x2c), the symbol id lookup @ 0x006E77F0 (labelled LTextSystem::GetIDFromSymbol; `this` unused), CGameUI::GetMiniMapDisplay @ 0x00437CE0 (label propagated) on the singleton @ 0x013B8790, CMiniMapDisplay::SetMarkerGraphic @ 0x0082A340. Shape: the script thing in esi, the interface in edi reused for the id (`jle` on it), the thing fetched into a local before the id is pushed, `ret 8`.
// Leaf: MiniMapSetMarkerGraphic
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
    CThing_Retail* PImp_Data;                      // +0x04 (engine/CScriptThing.h: CCountedPointer<CScriptThing>::Data)
    void*          PImp_Info;                      // +0x08
    CScriptThing();                                // 0x008CFF40 (the default constructor)
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
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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
class CGameScriptInterface { public:
    virtual void v000();
    CWorld*                     World;              // +0x04 (engine/CGameScriptInterface.h)
    CMainGameComponent_Retail*  Component;          // +0x08
    CDisplayEngine_Retail*      DisplayEngine;      // +0x0c
    CDefinitionManager_Retail*  DefinitionManager;  // +0x10
    CPlayerManager_Retail*      PlayerManager;      // +0x14
    long GetTextIDFromSymbol(const CCharString& symbol) const;   // 0x006E77F0 (labelled LTextSystem::GetIDFromSymbol; `this` is unused)
protected:
    long GetMinWorldFrameForMessages() const;   // 0x006E7510
    long GetMaxWorldFrameForMessages() const;       // 0x006E7530
public:
    void MiniMapSetMarkerGraphic(const CScriptThing& thing, const CCharString& symbol) const;
};

void CGameScriptInterface::MiniMapSetMarkerGraphic(const CScriptThing& thing, const CCharString& symbol) const
{
    if (thing.IsValid()) {
        long id = GetTextIDFromSymbol(symbol);
        if (id > 0) {
            CThing_Retail* t = thing.GetThing();
            g_pGameUI_013b8790->GetMiniMapDisplay()->SetMarkerGraphic(t, id);
        }
    }
}