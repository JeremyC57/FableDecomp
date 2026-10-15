// CGameScriptInterface::AddCreatureScriptedMode(const CScriptThing& thing, const CCharString& scriptName) const -- gives a valid creature that has a mode manager (kind 0x31) a new scripted creature mode running the named script
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A1A10, length 157 bytes (padded to 160).
// Callees: CScriptThing::IsValid (slot 75) and GetThing (slot 11), the component-map LowerBound @ 0x0040F020 (kind 0x31),
//          operator new @ 0x00BFEA1A (0x74 bytes), NCreatureMode::CModeBase::CModeBase(CThing*) @ 0x00861E00 (manifest
//          "CTCCreatureNavigation" ctor is propagated), the scripted-mode info constructor @ 0x0073E8F0 at +0x68 (manifest
//          "CConsole::SendMessageToUser" is propagated), CCountedPointer<CModeBase>::CCountedPointer(T*) @ 0x00718A00,
//          CTCCreatureModeManager::AddMode @ 0x0073ACC0.
// Vtable: NCreatureMode::CModeScripted 0x01267670.
// Shape: one GetThing for the mask test and the lookup; the mode manager is a named local (ebx) kept across the
// allocation; the mode is an inline `new CModeScripted(thing.GetThing(), scriptName)` (base, vtable store, member);
// the counted pointer is a by-value argument built in the argument area (`sub esp,8 ; mov ecx,esp`).
// Leaf: ?AddCreatureScriptedMode@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCCharString@@@Z
struct CTCBase_Retail;
struct STCEntry_Retail {                           // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind);         // 0x0040F020 (CVectorMap<ETCInterfaceType,...>::LowerBound; the key by address)
    STCEntry_Retail* LowerBound(const unsigned long& kind) const;   // 0x004365B0 (the const instantiation)
    __forceinline CTCBase_Retail* Find(long kind)          // the inlined map `find`: lower bound (the key in a stack temporary), end when the key is absent
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
struct CThing_Retail {
    unsigned char  _pad00[0x10];
    unsigned char  m_type;                         // +0x10 (1 = a creature)
    unsigned char  _pad11[0xf];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x41];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }   // the masked word
};
__forceinline CTCBase_Retail* GetTC(CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
__forceinline CTCBase_Retail* GetTCConst(const CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing_Retail* GetThing() const;       // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsValid() const;                  // slot 75 (+0x12c)
    CScriptThing();                                // 0x008CFF40 (the null script thing)
};
struct CPlayer_Retail {
    CThing_Retail* GetHeroCreature();              // 0x00487DC0
    CThing_Retail* GetControlledCreature();        // 0x00487DD0
};
struct CPlayerManager_Retail {
    CPlayer_Retail* GetMainPlayer();               // 0x00449970
};
struct CCharString;
struct CDefinitionManager_Retail {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CWorld; struct CMainGameComponent; struct CDisplayEngine; struct CThingSearchTools;
__forceinline CTCBase_Retail* GetTCIfPresent(CThing_Retail* thing, long kind)
{
    CTCBase_Retail* tc;                            // deliberately unset when the component is absent (retail reads a dead slot)
    if (thing->HasTC(kind))
        tc = GetTC(thing, kind);
    return tc;
}
__forceinline CTCBase_Retail* GetTCIfPresentConst(const CThing_Retail* thing, long kind)
{
    CTCBase_Retail* tc;
    if (thing->HasTC(kind))
        tc = GetTCConst(thing, kind);
    return tc;
}
template <class T> __forceinline bool GetTCOut(CThing_Retail* thing, long kind, T** out)   // the out-parameter lookup: the component stays in eax
{
    if (thing->HasTC(kind)) {
        *out = (T*)GetTC(thing, kind);
        return true;
    }
    return false;
}
struct CCharString {                               // fable_string.h layout: one data-block pointer
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
__forceinline bool IsThingAlive(const CScriptThing& thing)   // the thing exists and is not dead
{
    CThing_Retail* t = thing.GetThing();
    return t && !t->m_dead;
}
namespace NCreatureMode {
struct CModeBase {
    virtual void v00();
    unsigned char _pad04[0x64];
    CModeBase(CThing_Retail* thing);               // 0x00861E00
};
struct CScriptedModeInfo {                         // +0x68: vptr + the script name
    virtual void v00();
    CCharString m_name;
    CScriptedModeInfo(const CCharString& name);    // 0x0073E8F0
    unsigned char _pad08[0x74 - 0x68 - 8];
};
struct CModeScripted : CModeBase {                 // 0x74 bytes
    CScriptedModeInfo m_info;                      // +0x68
    CModeScripted(CThing_Retail* thing, const CCharString& name) : CModeBase(thing), m_info(name) {}
    virtual void v00();
};
}
void* operator new(unsigned int size);             // 0x00BFEA1A
template <class T> struct CCountedPointer {
    T*    Data;
    void* Info;
    CCountedPointer(T* p);                         // 0x00718A00
    CCountedPointer(const CCountedPointer& other);  // (declared: a by-value argument is then built in its stack slot)
};
struct CTCCreatureModeManager_Retail {
    bool AddMode(CCountedPointer<NCreatureMode::CModeBase> mode);   // 0x0073ACC0
};
struct CGameScriptInterface {
    virtual void v000();
    CWorld*                     World;             // +0x04 (engine/CGameScriptInterface.h)
    CMainGameComponent*         Component;         // +0x08
    CDisplayEngine*             DisplayEngine;     // +0x0c
    CDefinitionManager_Retail*  DefinitionManager; // +0x10
    CPlayerManager_Retail*      PlayerManager;     // +0x14
    CThingSearchTools*          ThingSearchTools;  // +0x18
    virtual void AddCreatureScriptedMode(const CScriptThing& thing, const CCharString& scriptName) const;
};

void CGameScriptInterface::AddCreatureScriptedMode(const CScriptThing& thing, const CCharString& scriptName) const
{
    if (!thing.IsValid())
        return;
    CThing_Retail* t = thing.GetThing();
    if (!t->HasTC(0x31))
        return;
    CTCCreatureModeManager_Retail* modes = (CTCCreatureModeManager_Retail*)GetTC(t, 0x31);
    modes->AddMode(CCountedPointer<NCreatureMode::CModeBase>(new NCreatureMode::CModeScripted(thing.GetThing(), scriptName)));
}
