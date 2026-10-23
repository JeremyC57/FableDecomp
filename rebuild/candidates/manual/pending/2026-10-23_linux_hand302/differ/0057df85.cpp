// CTCHeroStats::KickHeroToMostRecentSafeRegion() -- moves the hero out of the current region: to its REGION_KICK_TO_POINT marker when there is one, otherwise to the entrance of the most recently exited region where no fine is outstanding
// Grade: DIFFER(876v876) -- logic modelled (kick-to-point marker search, most-recent fine-free region fallback, move-hero event, region title); residue: frame-slot rotation of the search temporaries (currentRegion/string/HasDefName at +0x3c/+0x40/+0x38 vs retail +0x38/+0x3c/+0x40), the float AddFloat home sits in the string slot not the marker slot, and the marker load lands after the minimap store; 10 grades
// Flags: /GS
// Pragmas: size TU with the default FPO setting (`optimize("s", on)` only), which gives retail's `lea ebp,[esp-78h]` frame
#pragma optimize("s", on)

struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    CCharString(const CCharString& other);            // 0x0099EC30
    ~CCharString();                                   // 0x0099EAE0
    bool operator!=(const CCharString& other) const;  // 0x0099E900
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};
struct CGameEvent : CMemoryBuffer {
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddFloat(float value) { Copy(sizeof(value), &value); }
    void AddBool(bool value) { Copy(1, &value); }
};

struct CThingPhysics {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62();
    virtual float GetAngleXY();                     // slot 63 (+0xFC)
};
struct CThing {
    unsigned char  _pad00[0x60];
    CThingPhysics* Physics;                         // +0x60
    unsigned char  _pad64[0x8E - 0x64];
    short          MapNumber;                       // +0x8E
    signed char    PlayerNumber;                    // +0x90
    const C3DVector& GetPosition() const;           // 0x004C73D0
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0040DCE0 (CThing*)
struct CThingMarker : CThing { enum { ThingListType = 0x0b }; };

struct CThingFilter_HasDefName {
    CCharString DefName;
    CThingFilter_HasDefName(const CCharString& name) : DefName(name) { }
};
struct CThingFilter_HasScriptData {
    CCharString ScriptData;
    CThingFilter_HasScriptData(const CCharString& data) : ScriptData(data) { }
};
template <class A, class B> struct CFilter_Bind2 {
    const A& m_first;
    const B& m_second;
    CFilter_Bind2(const A& a, const B& b) : m_first(a), m_second(b) { }
};
struct CThingSearchTools {
    template <class TFilter, class TThing> bool GetPFirstLeafThing(const TFilter& filter, TThing** out);   // 0x005800D6
};

struct CRegion {
    unsigned char _pad00[0x18];
    CCharString   Name;                             // +0x18
};
struct CWorldMap {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetMapNumberAt_slot16(const C3DVector& pos);   // slot 16 (+0x40)
    long GetCurrentRegionNumber();                  // 0x004FB150
    CRegion& GetRegion(long index);                 // 0x004FC180
    long GetRegionNumberMapIsIn(long map);          // 0x004FC190
};
struct CWorld {
    unsigned char _pad000[0x12f];
    bool DisplayMiniMapOnRegionLoad;                // +0x12F
    void HandleMoveHeroToRegionGameEvent(const CGameEvent& event);   // 0x0049EAF0
};
struct CPlayerGui {
    void ShowRegion_004378b0(CRegion& region);      // 0x004378B0
};
extern CPlayerGui* g_pPlayerGui;                    // 0x013B8790
inline CPlayerGui* GetPlayerGui() { return g_pPlayerGui; }

struct MirroredCriminalStatus {
    long  Crimes[10];                               // +0x00
    long  LastTimeSeenByGuard;                      // +0x28
    long  LastTimeExited;                           // +0x2C
    bool  ExitedUsingTeleporter;                    // +0x30
    float EntrancePosX;                             // +0x34
    float EntrancePosY;                             // +0x38
    float EntrancePosZ;                             // +0x3C
    float EntranceAngle;                            // +0x40
    MirroredCriminalStatus();                       // 0x0057A5DD
};
struct _Rb_tree_node_base {
    long                color;
    _Rb_tree_node_base* parent;
    _Rb_tree_node_base* left;
    _Rb_tree_node_base* right;
};
_Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* node);   // 0x004292C0
struct SCrimeNode : _Rb_tree_node_base {
    CCharString            first;                   // +0x10
    MirroredCriminalStatus second;                  // +0x14
};
struct CCrimeStatusMap {                            // std::map<CCharString, MirroredCriminalStatus> (STLport, 0x0C bytes)
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    unsigned long       _M_compare;
    struct iterator {
        _Rb_tree_node_base* _M_node;
        iterator(_Rb_tree_node_base* n) : _M_node(n) { }
        SCrimeNode* operator->() const { return (SCrimeNode*)_M_node; }
        iterator& operator++() { _M_node = _M_increment(_M_node); return *this; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    iterator begin() { return iterator(_M_header->left); }
    iterator end() { return iterator(_M_header); }
};

struct CTCHeroStats {
    void*           __vftable;                      // +0x00
    CThingMarker*   ParentThing;                    // +0x04 CTCBase (typed so IS_THING_ALIVE<CThing*> takes a converted temporary)
    unsigned char   _pad_0x08[0x4];
    CCrimeStatusMap OutstandingCrimes;              // +0x0C
    CWorldMap* GetWorldMap() const;                 // 0x00686D20 (CTCBase)
    CWorld* GetWorld() const;                       // 0x00686A70 (CTCBase)
    CThingSearchTools* GetThingSearchTools() const; // 0x00686D90 (CTCBase)
    void GetOutstandingFineAndFramesLeft(const CCharString& region, long& fine, long& framesLeft);   // 0x0057DAF5
    void KickHeroToMostRecentSafeRegion();
};

enum { GAME_EVENT_MOVE_HERO_TO_REGION = 0x2C };

void CTCHeroStats::KickHeroToMostRecentSafeRegion()
{
    MirroredCriminalStatus best;
    long bestTime = 0;
    CCharString currentRegion(GetWorldMap()->GetRegion(GetWorldMap()->GetCurrentRegionNumber()).Name);
    {
    CThingMarker* marker;
    if (GetThingSearchTools()->GetPFirstLeafThing(
        CFilter_Bind2<CThingFilter_HasDefName, CThingFilter_HasScriptData>(
            CThingFilter_HasDefName("REGION_KICK_TO_POINT"), CThingFilter_HasScriptData(currentRegion)), &marker)) {
        if (!IS_THING_ALIVE<CThing*>(ParentThing))
            return;
        GetWorld()->DisplayMiniMapOnRegionLoad = true;
        CThingMarker* kick = marker;
        CGameEvent event(GAME_EVENT_MOVE_HERO_TO_REGION);
        event.AddFloat(kick->GetPosition().X);
        event.AddFloat(kick->GetPosition().Y);
        event.AddFloat(kick->GetPosition().Z);
        event.AddFloat(kick->Physics->GetAngleXY());
        event.AddBool(false);
        event.AddBool(false);
        event.AddBool(false);
        event.Player = ParentThing->PlayerNumber;
        long region = GetWorldMap()->GetRegionNumberMapIsIn(kick->MapNumber);
        if (region > 0)
            GetPlayerGui()->ShowRegion_004378b0(GetWorldMap()->GetRegion(region));
        GetWorld()->HandleMoveHeroToRegionGameEvent(event);
        return;
    }
    }
    {
        for (CCrimeStatusMap::iterator it = OutstandingCrimes.begin(); it != OutstandingCrimes.end(); ++it) {
            long fine;
            long framesLeft;
            GetOutstandingFineAndFramesLeft(it->first, fine, framesLeft);
            long exited;
            if (fine == 0 && it->first != currentRegion && (exited = it->second.LastTimeExited) > bestTime) {
                best = it->second;
                bestTime = exited;
            }
        }
        if (bestTime == 0)
            return;
        if (!IS_THING_ALIVE<CThing*>(ParentThing))
            return;
        GetWorld()->DisplayMiniMapOnRegionLoad = true;
        CGameEvent event(GAME_EVENT_MOVE_HERO_TO_REGION);
        event.AddFloat(best.EntrancePosX);
        event.AddFloat(best.EntrancePosY);
        event.AddFloat(best.EntrancePosZ);
        event.AddFloat(best.EntranceAngle);
        event.AddBool(false);
        event.AddBool(false);
        event.AddBool(false);
        event.Player = ParentThing->PlayerNumber;
        C3DVector pos(best.EntrancePosX, best.EntrancePosY, best.EntrancePosZ);
        long region = GetWorldMap()->GetRegionNumberMapIsIn(GetWorldMap()->GetMapNumberAt_slot16(pos));
        if (region > 0)
            GetPlayerGui()->ShowRegion_004378b0(GetWorldMap()->GetRegion(region));
        GetWorld()->HandleMoveHeroToRegionGameEvent(event);
    }
}
