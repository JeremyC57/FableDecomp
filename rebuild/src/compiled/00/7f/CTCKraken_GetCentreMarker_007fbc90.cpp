// CTCKraken::GetCentreMarker() -- (unless the world is loading) find the kraken chamber building and the KRACKEN_CHAMBER_CENTRE marker in the 10x10 box around the kraken and target the hero
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FBC90, length 436 bytes (+ int3 padding), esp frame, plain `ret`.
// Callees: the CTCBase accessors GetWorld @ 0x00686D60 (its slot 9, +0x24, a bool "loading" query; name descriptive),
// GetDefinitionManager @ 0x006869D0, GetThingSearchTools @ 0x00686D90 and GetPlayerManager @ 0x00686D40 (manifest labels
// "GetDisplayEngine" / "_Cons_val" are propagated), CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410,
// CCharString(const char*, long) @ 0x0099EBF0 / copy @ 0x0099EC30 / ~CCharString @ 0x0099EAE0, CThing::GetPosition @
// 0x004C73D0, __ftol2 @ 0x00BFEA70, CThingSearchTools::GetPFirstThingInRoughArea<HasDefGlobalIndex> @ 0x007FCEF0 and
// <HasTrackName> @ 0x007FD110, CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPPlayerCharacter @ 0x00487DC0,
// CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10
// (CIntelligentPointer<CThing> vtable 0x01238C6C).
// Strings: "BUILDING_KRAKEN_CHAMBER" @ 0x01272074, "KRACKEN_CHAMBER_CENTRE" @ 0x0127205C.
// Shape: the definition manager is a named local (fetched before the name temporary); the box search sits in a block that
// opens AFTER the index lookup, so the name temporary shares the frame word of the (uninitialised, address-taken) `found`
// out pointer; one box and one `found` serve both searches; the intelligent-pointer temporaries take `CThing* const&` (the
// out word is read after the base ctor) and operator= names the item in a local.
// Layout (local model): retail CTCKraken PChamber +0x10, PCentreMarker +0x18, PTarget +0x20 (PDB offsets; before the lists).
struct C3DVector { float X, Y, Z; };
struct C2DBoxI {
    long MinX, MinY, MaxX, MaxY;
    C2DBoxI(long minX, long minY, long maxX, long maxY) : MinX(minX), MinY(minY), MaxX(maxX), MaxY(maxY) { }
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
class CThing {
public:
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
struct CThingFilter_HasDefGlobalIndex {
    long m_defGlobalIndex;
    CThingFilter_HasDefGlobalIndex(long index) : m_defGlobalIndex(index) { }
};
struct CThingFilter_HasTrackName {
    CCharString Name;
    CThingFilter_HasTrackName(const CCharString& name) : Name(name) { }
};
struct CThingSearchTools {
    bool GetPFirstThingInRoughArea(const C2DBoxI& area, const CThingFilter_HasDefGlobalIndex& filter, CThing** out) const;   // 0x007FCEF0
    bool GetPFirstThingInRoughArea(const C2DBoxI& area, const CThingFilter_HasTrackName& filter, CThing** out) const;        // 0x007FD110
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool IsLoading();                           // slot 9 (+0x24) (name descriptive)
};
struct CThingPlayerCreature;
struct CPlayer {
    CThing* GetPPlayerCharacter() const;                // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                           // 0x00449970
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    void SetPItem(CThing* item);                        // 0x00A01B90
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CIntelligentPointer : CBaseIntelligentPointer {  // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* const& item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
struct CTCBase {
    void*   __vftable;                                  // +0x00
    CThing* ParentThing;                                // +0x04
    CPlayerManager*     GetPlayerManager();             // 0x00686D40
    CWorld*             GetWorld_00686d60();                     // 0x00686D60
    CThingSearchTools*  GetThingSearchTools() const;    // 0x00686D90
    CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0
};
struct CTCKraken : CTCBase {
    unsigned char       _pad08[0x10 - 0x08];
    CIntelligentPointer PChamber;                       // +0x10
    CIntelligentPointer PCentreMarker;                  // +0x18
    CIntelligentPointer PTarget;                        // +0x20
    void GetCentreMarker();
};

void CTCKraken::GetCentreMarker()
{
    if (GetWorld_00686d60()->IsLoading())
        return;
    CDefinitionManager* definitions = GetDefinitionManager();
    long chamberIndex = definitions->GetDefGlobalIndexFromName("BUILDING_KRAKEN_CHAMBER");
    {
        const C3DVector& pos = ParentThing->GetPosition();
        long x = (long)pos.X;
        long y = (long)pos.Y;
        C2DBoxI area(x - 5, y - 5, x + 5, y + 5);
        CThing* found;
        GetThingSearchTools()->GetPFirstThingInRoughArea(area, CThingFilter_HasDefGlobalIndex(chamberIndex), &found);
        PChamber = CIntelligentPointer(found);
        {
            CCharString name("KRACKEN_CHAMBER_CENTRE");
            GetThingSearchTools()->GetPFirstThingInRoughArea(area, CThingFilter_HasTrackName(name), &found);
        }
        PCentreMarker = CIntelligentPointer(found);
    }
    PTarget = CIntelligentPointer(GetPlayerManager()->GetMainPlayer()->GetPPlayerCharacter());
}