// CTCHeroStats::KickHeroToMostRecentSafeRegion() -- moves the hero out of the current region: to its REGION_KICK_TO_POINT marker when there is one, otherwise to the entrance of the most recently exited region where no fine is outstanding
// Grade: DIFFER(876v876) -- residue: one frame-slot rotation only (17 mnemonic diffs): currentRegion / the "REGION_KICK_TO_POINT" literal / the HasDefName filter temporary sit at +0x40/+0x38/+0x3c where retail has +0x38/+0x3c/+0x40 (the loop iterator and framesLeft follow their packing partners); lane gap: frame-slot order | tried: same-TU noinline GetPFirstLeafThing body (32 -> 17: the marker word now shares the AddFloat homes and the marker load precedes the minimap store), GFFilter_Bind2 vs direct CFilter_Bind2, declaration orders of best/bestTime/currentRegion, currentRegion as reference/copy-init/GetName/region local, filter user dtors, explicit CCharString literal, found bool local, if/else and block-brace layouts, loop-form variants (postfix, alias, nested if), same-TU IS_THING_ALIVE and GetOutstandingFineAndFramesLeft bodies (inert / worse), named ScriptData filter / named literal (worse, 61/55), found in its own block (inert); 1 RTM grade this pass (11 total) | next: no source lever left that moves the slot; treat as a VC7.1 packing-order lane gap unless a sibling with the same currentRegion-plus-filter shape lands
// Flags: /GS
// Leaf: ?KickHeroToMostRecentSafeRegion@CTCHeroStats@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057DF85, 876 bytes, thiscall, /GS, size TU with the default FPO
//   setting (`optimize("s", on)` only: retail's `lea ebp,[esp-78h]` frame).
// Identity: Ego_r.pdb / FableWin.pdb ?KickHeroToMostRecentSafeRegion@CTCHeroStats@@QAEXXZ.
// Callees: CTCHeroStats::MirroredCriminalStatus() 0x0057A5DD, CTCBase::PeekWorldMap() const 0x00686D20 (the
//   PeekWorld/GetWorld, PeekWorldMap/GetWorldMap pairs sit in PDB order at 0x00686A70/80 and 0x00686D20/30),
//   CWorldMap::GetLoadedRegion() const 0x004FB150, CWorldMap::GetRegion(long) const 0x004FC180 (the const twin sits
//   right before GetRegionNumberMapIsIn 0x004FC190, as in Ego_r.pdb), CCharString(const char*, long) 0x0099EBF0 /
//   CCharString(const CCharString&) 0x0099EC30 / ~CCharString 0x0099EAE0 / operator!= 0x0099E900,
//   CTCBase::PeekThingSearchTools() const 0x00686D90, CThingSearchTools::GetPFirstLeafThing<CFilter_Bind2<
//   CThingFilter_HasDefName,CThingFilter_HasScriptData>,CThingMarker> const 0x005800D6, IS_THING_ALIVE<CThing*>
//   0x0040DCE0, CTCBase::GetWorld() const 0x00686A70, CThing::GetPos() const 0x004C73D0, CGameEvent::AddData
//   0x009F1760, the physics TC's GetFacingAngleXY (slot 63, +0xFC), CWorldMap::GetMapNumberAt(const C3DVector&) const
//   (slot 16, +0x40), CPlayerGui::SetMiniMapRegion(const CRegion&) 0x004378B0 (Ego_r order: SetMiniMapAlpha then
//   SetMiniMapRegion; retail 0x00437890 / 0x004378B0), CWorld::HandleMoveHeroToRegionGameEvent 0x0049EAF0,
//   CTCHeroStats::GetOutstandingFineAndFramesLeft 0x0057DAF5, _Rb_global<bool>::_M_increment 0x004292C0.
// Layout: retail CTCHeroStats OutstandingCrimes +0x0C (STLport map, 12 bytes); CThing PhysicsTC +0x60, CurrentMapNumber
//   +0x8E, PlayerNumber +0x90; CWorld DisplayMiniMapOnRegionLoad +0x12F (engine/CWorld.h); CRegion Name +0x18.
//   Local model: engine/CTCHeroStats.h declares no member functions and holds the map as opaque bytes (header debt).
#pragma optimize("s", on)

class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    bool operator!=(const CCharString& other) const;    // 0x0099E900
};
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

class CGameEvent {
public:
    long          Type;                                 // +0x00
    signed char   Player;                               // +0x04
    unsigned char Data[32];                             // +0x05
    unsigned char EndPos;                               // +0x25
    bool          Valid;                                // +0x26
    bool          Replacement;                          // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddData(long length, const void* data);        // 0x009F1760
    void AddFloat(float value) { AddData(sizeof(value), &value); }
    void AddUBYTE(unsigned char value) { AddData(sizeof(value), &value); }
    void SetPlayer(long player) { Player = (signed char)player; }
};

class CTCPhysicsBase {
public:
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
    virtual float GetFacingAngleXY() const;             // slot 63 (+0xFC)
};
class CThing {
public:
    unsigned char   _pad_0x00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    unsigned char   _pad_0x64[0x8E - 0x64];
    short           CurrentMapNumber;                   // +0x8E
    signed char     PlayerNumber;                       // +0x90
    const C3DVector& GetPos() const;                    // 0x004C73D0
    bool IsIncludedInThingScans() const;                // 0x0048BC40
};
class CThingMarker : public CThing {
public:
    enum { ThingListType = 0x0b };
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0040DCE0 (T = CThing*)

class CThingFilter_HasDefName {
public:
    CCharString Name;                                   // +0x00
    CThingFilter_HasDefName(const CCharString& name) : Name(name) { }
    bool operator()(const CThing& thing) const;         // 0x006A7BE0
};
class CThingFilter_HasScriptData {
public:
    CCharString Data;                                   // +0x00
    CThingFilter_HasScriptData(const CCharString& data) : Data(data) { }
    bool operator()(const CThing& thing) const;         // 0x006A7E70
};
template <class A, class B> class CFilter_Bind2 {
public:
    const A& First;
    const B& Second;
    CFilter_Bind2(const A& a, const B& b) : First(a), Second(b) { }
    template <class T> __forceinline bool operator()(const T& thing) const { return First(thing) && Second(thing); }
};
template <class A, class B> inline CFilter_Bind2<A, B> __fastcall GFFilter_Bind2(const A& a, const B& b)
{
    return CFilter_Bind2<A, B>(a, b);
}

class CRegion {
public:
    unsigned char _pad_0x00[0x18];
    CCharString   Name;                                 // +0x18
    const CCharString& GetName() const { return Name; }
};
class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetMapNumberAt(const C3DVector& pos) const;   // slot 16 (+0x40)
    long GetLoadedRegion() const;                       // 0x004FB150
    const CRegion& GetRegion(long index) const;         // 0x004FC180
    long GetRegionNumberMapIsIn(long mapNumber) const;  // 0x004FC190
};
class CWorld {
public:
    unsigned char _pad_0x00[0x12F];
    bool          DisplayMiniMapOnRegionLoad;           // +0x12F
    void SetToDisplayMiniMapOnRegionLoad(bool display) { DisplayMiniMapOnRegionLoad = display; }
    void HandleMoveHeroToRegionGameEvent(const CGameEvent& event);   // 0x0049EAF0
};
class CPlayerGui {
public:
    void SetMiniMapRegion(const CRegion& region);       // 0x004378B0
};
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790

namespace std {
template <class _Tp> class allocator { };
struct _List_node_base {
    _List_node_base* _M_next;                           // +0x00
    _List_node_base* _M_prev;                           // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                        // +0x08
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    const _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_iterator<_Tp> const_iterator;
    _List_node_base* _M_node;                           // +0x00 (STLport, 4 bytes)
    const_iterator begin() const { return const_iterator(_M_node->_M_next); }
    const_iterator end() const { return const_iterator(_M_node); }
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Pair> struct _Select1st { };
template <class _Tp> struct less { };
template <class _Tp> struct _Nonconst_traits { };
struct _Rb_tree_node_base {
    bool                _M_color;                       // +0x00
    _Rb_tree_node_base* _M_parent;                      // +0x04
    _Rb_tree_node_base* _M_left;                        // +0x08
    _Rb_tree_node_base* _M_right;                       // +0x0c
};
template <class _Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* __x);   // 0x004292C0
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};
template <class _Value, class _Traits> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) { }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Value* operator->() const { return &(operator*()); }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    _Rb_tree_node_base* _M_header;                      // +0x00
    unsigned int        _M_node_count;                  // +0x04
    unsigned char       _M_key_compare;                 // +0x08
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator begin() { return iterator(_M_t._M_header->_M_left); }
    iterator end() { return iterator(_M_t._M_header); }
};
}

class CClass;
class CThingSearchTools {
protected:
    const std::list<CClass*>& PeekTypeList(long type) const;   // 0x0049C780
public:
    template <class TFilter, class TThing> bool GetPFirstLeafThing(const TFilter& filter, TThing** out) const;   // 0x005800D6
};
// Same-TU body of the real callee 0x005800D6 (retail instantiation; under auto_inline(off) + noinline it lets VC7.1
// keep the out-pointer local's frame word shared with the AddFloat homes, as retail does).
#pragma auto_inline(off)
template <class TFilter, class TThing> __declspec(noinline) bool CThingSearchTools::GetPFirstLeafThing(const TFilter& filter, TThing** out) const
{
    const std::list<CClass*>& things = PeekTypeList(TThing::ThingListType);
    for (std::list<CClass*>::const_iterator it = things.begin(); it != things.end(); ++it) {
        CThing* thing = (CThing*)*it;
        if (thing->IsIncludedInThingScans() && filter(*(TThing*)thing)) {
            *out = (TThing*)thing;
            return true;
        }
    }
    return false;
}
#pragma auto_inline(on)

class CTCBase {
public:
    void*   __vftable;                                  // +0x00
    CThing* ParentThing;                                // +0x04
    CThing& GetParentThing() { return *ParentThing; }
protected:
    CWorld& GetWorld() const;                           // 0x00686A70
    const CWorldMap& PeekWorldMap() const;              // 0x00686D20
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x00686D90
};

enum EGameEventType { GAME_EVENT_MOVE_HERO_TO_REGION = 0x2C };

class CTCHeroStats : public CTCBase {
public:
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
    unsigned char _pad_0x08[0x4];
    std::map<CCharString, MirroredCriminalStatus> OutstandingCrimes;   // +0x0C
    void GetOutstandingFineAndFramesLeft(const CCharString& region, long& fine, long& framesLeft);   // 0x0057DAF5
    void KickHeroToMostRecentSafeRegion();
};

void CTCHeroStats::KickHeroToMostRecentSafeRegion()
{
    MirroredCriminalStatus best;
    long bestTime = 0;
    CCharString currentRegion(PeekWorldMap().GetRegion(PeekWorldMap().GetLoadedRegion()).GetName());
    {
    CThingMarker* marker;
    if (PeekThingSearchTools().GetPFirstLeafThing(
            GFFilter_Bind2(CThingFilter_HasDefName("REGION_KICK_TO_POINT"), CThingFilter_HasScriptData(currentRegion)),
            &marker)) {
        if (!IS_THING_ALIVE(&GetParentThing()))
            return;
        GetWorld().SetToDisplayMiniMapOnRegionLoad(true);
        CGameEvent event(GAME_EVENT_MOVE_HERO_TO_REGION);
        event.AddFloat(marker->GetPos().X);
        event.AddFloat(marker->GetPos().Y);
        event.AddFloat(marker->GetPos().Z);
        event.AddFloat(marker->PhysicsTC->GetFacingAngleXY());
        event.AddUBYTE(false);
        event.AddUBYTE(false);
        event.AddUBYTE(false);
        event.SetPlayer(GetParentThing().PlayerNumber);
        long region = PeekWorldMap().GetRegionNumberMapIsIn(marker->CurrentMapNumber);
        if (region > 0) {
            CPlayerGui* gui = g_pPlayerGui;
            gui->SetMiniMapRegion(PeekWorldMap().GetRegion(region));
        }
        GetWorld().HandleMoveHeroToRegionGameEvent(event);
        return;
    }
    }
    {
    for (std::map<CCharString, MirroredCriminalStatus>::iterator it = OutstandingCrimes.begin(); it != OutstandingCrimes.end(); ++it) {
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
    if (!IS_THING_ALIVE(&GetParentThing()))
        return;
    GetWorld().SetToDisplayMiniMapOnRegionLoad(true);
    CGameEvent event(GAME_EVENT_MOVE_HERO_TO_REGION);
    event.AddFloat(best.EntrancePosX);
    event.AddFloat(best.EntrancePosY);
    event.AddFloat(best.EntrancePosZ);
    event.AddFloat(best.EntranceAngle);
    event.AddUBYTE(false);
    event.AddUBYTE(false);
    event.AddUBYTE(false);
    event.SetPlayer(GetParentThing().PlayerNumber);
    C3DVector pos(best.EntrancePosX, best.EntrancePosY, best.EntrancePosZ);
    long region = PeekWorldMap().GetRegionNumberMapIsIn(PeekWorldMap().GetMapNumberAt(pos));
    if (region > 0) {
        CPlayerGui* gui = g_pPlayerGui;
        gui->SetMiniMapRegion(PeekWorldMap().GetRegion(region));
    }
    GetWorld().HandleMoveHeroToRegionGameEvent(event);
    }
}
