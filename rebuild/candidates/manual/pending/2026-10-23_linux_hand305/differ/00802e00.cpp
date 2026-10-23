// Grade: DIFFER(1002v990) -- register/frame: retail keeps this in ebp and re-reads the thing parameter from its slot (spilling &PTargetedThing), ours holds thing in edi and this in esi (one extra frame word)
// Tried: helper returning the readable action (1024), explicit readable blocks with a bool compare (1002).
// CTCTargetingPlayer::SetPTargetedThing(CThing* thing) -- retarget the hero: drop the targeting mode, clear the readable's region text for the old target, reset the threatened pointer, set the new target, show a readable's name and the new target's GUI text
// Leaf: SetPTargetedThing
// Retail: Fable.exe (TLC, VC7.1) @ 0x00802E00, 990 bytes, `ret 4`.
// Callees: the player-interface getter @ 0x00416359 on the engine @ 0x013B86A0, CGamePlayerInterface::IsActive @ 0x00445B80,
//   the targeting vslot +0x90 (GetPTargetedThing), CThing::GetPlayer @ 0x004C7A30, CPlayer::HasMode @ 0x00634160 /
//   RemoveMode @ 0x00634190, CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10,
//   LowerBound @ 0x0040F020, CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0, the action's type-name vslot +0x58, Compare @ 0x00411570,
//   CTCRegionDisplay::SetCurrentAreaName (CCharString overload @ 0x00639BA0, CWideString overload @ 0x00639A90),
//   CTCTargetingBase::SetPTargetedThing @ 0x004DDAB4, the interface-0x10c name getter @ 0x007F4B60, CWideString() @ 0x0099AED0,
//   ~CWideString @ 0x0099B510, the GUI target-text setter @ 0x004379D0 on g_pPlayerGui @ 0x013B8790.
// Layout: retail CTCTargetingPlayer PTargetedThing +0x25 (packed), PLastThreatenedThing +0xe4, flag +0xf4; CThing bitset +0x20,
//   map +0x44, life flags +0x91; the use-readable action's def +0x14 (show-name flag +0x25) and wide name +0x18.
#include <stddef.h>
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CCharStringData {
    const char*   m_pData;
    unsigned long m_length;
};
long __fastcall Compare(const char* a, const char* b);  // 0x00411570
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    __forceinline bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->m_length == other.PStringData->m_length)
            return Compare(PStringData->m_pData, other.PStringData->m_pData) ? false : true;
        return false;
    }
};
struct CWideString {
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
};

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char LifeFlags;                            // +0x91
    struct CPlayer* GetPlayer();                        // 0x004C7A30
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T>
    __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template <class T>
    __forceinline void GetInterface(long type, T** out)
    {
        if (HasTC(type))
            *out = GetTC<T>(type);
    }
};
struct CPlayer {
    bool HasMode(long mode);                            // 0x00634160
    void RemoveMode(long mode);                         // 0x00634190
};

struct CBaseObject;
struct CBaseIntelligentPointer {
    void*        __vftable;
    CBaseObject* PData;
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.CBaseIntelligentPointer::GetPItem());
        return *this;
    }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

struct CUseReadableDef {
    unsigned char _pad00[0x25];
    bool          ShowName;                             // +0x25
};
struct CTCAction {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual CCharString GetTypeName() const;            // +0x58
    unsigned char     _pad04[0x14 - 0x04];
    CUseReadableDef*  PDef;                             // +0x14
    CWideString       Name;                             // +0x18
};
struct CTCRegionDisplay {
    void SetCurrentAreaName(const CCharString& name, float displayTime, bool infoDisplay, bool showNow, bool updateWhilePaused);   // 0x00639BA0
    void SetCurrentAreaName(const CWideString& name, float displayTime, bool infoDisplay, bool showNow, bool updateWhilePaused);   // 0x00639A90
};
struct CTCTargetName {
    CWideString GetTargetName();                        // 0x007F4B60
};
struct CPlayerGui {
    unsigned char _pad00[0xf6];
    bool          HideTargetText;                       // +0xf6
    void SetTargetText(const CWideString& text);        // 0x004379D0
};
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790
struct CGamePlayerInterface { bool IsActive(); };      // 0x00445B80
struct CEngine { CGamePlayerInterface* GetPlayerInterface(); };   // 0x00416359
extern CEngine* g_pEngine;                              // 0x013B86A0

#pragma pack(push, 1)
struct CTCTargetingPlayer {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual CThing* GetPTargetedThing();                // +0x90
    CThing*       ParentThing;                          // +0x04
    unsigned char _pad08[0x25 - 0x08];
    CIntelligentPointer<CThing> PTargetedThing;         // +0x25
    unsigned char _pad2d[0xe4 - 0x2d];
    CIntelligentPointer<CThing> PLastThreatenedThing;   // +0xe4
    unsigned char _padec[0xf4 - 0xec];
    bool          LastThreatened;                       // +0xf4
    void BaseSetPTargetedThing(CThing* thing);          // 0x004DDAB4
    __forceinline bool IsTargetAlive() const
    {
        return PTargetedThing.GetPItem() && !(PTargetedThing.GetPItem()->LifeFlags & 1);
    }
    void SetPTargetedThing(CThing* thing);
};
#pragma pack(pop)

void CTCTargetingPlayer::SetPTargetedThing(CThing* thing)
{
    if (g_pEngine->GetPlayerInterface()->IsActive()) {
        if (!thing || thing != GetPTargetedThing()) {
            if (ParentThing->GetPlayer()->HasMode(2))
                ParentThing->GetPlayer()->RemoveMode(2);
        }
    }
    if (thing == PTargetedThing.GetPItem())
        return;
    if (IsTargetAlive()) {
        CThing* target = PTargetedThing.GetPItem();
        if (target->HasTC(0xd)) {
            CTCAction* action = target->GetTC<CTCAction>(0xd);
            bool readable = action->GetTypeName() == CCharString("CTCActionUseReadable");
            if (readable && action->PDef->ShowName) {
                CTCRegionDisplay* display;
                ParentThing->GetInterface(0x8e, &display);
                if (display)
                    display->SetCurrentAreaName(CCharString(""), -1.0f, false, false, true);
            }
        }
    }
    if (thing != GetPTargetedThing()) {
        PLastThreatenedThing = CIntelligentPointer<CThing>(0);
        LastThreatened = false;
    }
    BaseSetPTargetedThing(thing);
    if (IsTargetAlive()) {
        CThing* target = PTargetedThing.GetPItem();
        if (target->HasTC(0xd)) {
            CTCAction* action = target->GetTC<CTCAction>(0xd);
            bool readable = action->GetTypeName() == CCharString("CTCActionUseReadable");
            if (readable && action->PDef->ShowName) {
                CTCRegionDisplay* display;
                ParentThing->GetInterface(0x8e, &display);
                if (display)
                    display->SetCurrentAreaName(action->Name, 0.0f, true, false, true);
            }
        }
    }
    if (IsTargetAlive() && !g_pPlayerGui->HideTargetText && PTargetedThing.GetPItem()->HasTC(0x10c))
        g_pPlayerGui->SetTargetText(PTargetedThing.GetPItem()->GetTC<CTCTargetName>(0x10c)->GetTargetName());
    else
        g_pPlayerGui->SetTargetText(CWideString());
}
