// Grade: DIFFER(308v302) -- the negated search result is materialised (mov bl,al ; neg bl ; sbb bl,bl ; inc bl) where retail keeps the raw bool in bl and tests it with jne, and the function-scope marker takes its own frame word so framesLeft moves from [ebp-18h] to [ebp-10h]
// CTCHeroStats::PrepareToKickHeroToMostRecentSafeRegion() -- unless the current region has a REGION_KICK_TO_POINT marker, find the most recently exited region with no outstanding fine and, if the current region was exited before, stamp its exit time with the current frame
// Leaf: PrepareToKickHeroToMostRecentSafeRegion
//
// Retail 0x0057DE57, 302 bytes (plain `ret`; frame pointer, `leave`). Ego_r.pdb
// `?PrepareToKickHeroToMostRecentSafeRegion@CTCHeroStats@@QAEXXZ`. Size TU with a frame pointer: `#pragma optimize("y", off)`
// + `("s", on)` (`and [ebp-8],0`, `pop ecx` after the cdecl increment).
// Callees: CTCBase::GetWorldMap @ 0x00686D20 (twice), CWorldMap::GetCurrentRegionNumber @ 0x004FB150, CWorldMap::GetRegion
// @ 0x004FC180, CCharString(const CCharString&) @ 0x0099EC30 (the region name and both filter copies), CCharString(const char*,
// long) @ 0x0099EBF0 ("REGION_KICK_TO_POINT" @ 0x0124F780), CTCBase::GetThingSearchTools @ 0x00686D90,
// CThingSearchTools::GetPFirstLeafThing<CFilter_Bind2<CThingFilter_HasDefName,CThingFilter_HasScriptData>,CThingMarker>
// @ 0x005800D6, ~CCharString @ 0x0099EAE0, CTCHeroStats::GetOutstandingFineAndFramesLeft @ 0x0057DAF5, CCharString::operator!=
// @ 0x0099E900, the STLport tree increment @ 0x004292C0 (`__cdecl`, label "GetNoAllocatedAreas" is propagated),
// map<CCharString,MirroredCriminalStatus>::operator[] @ 0x005812DF (twice), CTCBase::GetWorld @ 0x00686A70, CWorld::GetWorldFrame
// @ 0x0049D870 (static).
// Layout: retail CTCHeroStats OutstandingCrimes +0x0c (12-byte STLport map: header-node pointer first; node key +0x10, value
// +0x14); MirroredCriminalStatus LastTimeExited +0x2c; CRegion Name +0x18.
// Shape: the marker search is the `if` condition itself (its result rides in bl across the temporaries' destructors; the literal is an implicit conversion temporary built first, the two filter
// predicates hold copies and are destroyed in reverse); the fine and frames-left outputs share frame words with that block's
// temporaries; the most recent exit time is a frame local compared `x > best`.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    bool operator!=(const CCharString& other) const;        // 0x0099E900
};

struct CRegion {
    unsigned char _pad_0x00[0x18];
    CCharString   Name;                                     // +0x18
};
struct CWorldMap {
    long GetCurrentRegionNumber();                          // 0x004FB150
    CRegion& GetRegion(long index);                         // 0x004FC180
};
struct CWorld {
    static long GetWorldFrame();                            // 0x0049D870
};

struct CThingFilter_HasDefName {
    CCharString Name;
    CThingFilter_HasDefName(const CCharString& name) : Name(name) { }
};
struct CThingFilter_HasScriptData {
    CCharString ScriptData;
    CThingFilter_HasScriptData(const CCharString& data) : ScriptData(data) { }
};
template <class A, class B> struct CFilter_Bind2 {
    const A* PA;
    const B* PB;
    CFilter_Bind2(const A& a, const B& b) : PA(&a), PB(&b) { }
};
class CThingMarker;
struct CThingSearchTools {
    template <class F, class T> bool GetPFirstLeafThing(const F& filter, T** ppThing) const;   // 0x005800D6
};

struct MirroredCriminalStatus {
    long  Crimes[10];                                       // +0x00
    long  LastTimeSeenByGuard;                              // +0x28
    long  LastTimeExited;                                   // +0x2C
    bool  ExitedUsingTeleporter;                            // +0x30
    float EntrancePosX;                                     // +0x34
    float EntrancePosY;                                     // +0x38
    float EntrancePosZ;                                     // +0x3C
    float EntranceAngle;                                    // +0x40
};
struct SCrimeNode {                                         // STLport _Rb_tree_node<pair<const CCharString, MirroredCriminalStatus>>
    long        _M_color;                                   // +0x00
    SCrimeNode* _M_parent;                                  // +0x04
    SCrimeNode* _M_left;                                    // +0x08
    SCrimeNode* _M_right;                                   // +0x0c
    CCharString            first;                           // +0x10
    MirroredCriminalStatus second;                          // +0x14
};
SCrimeNode* __cdecl RbTreeIncrement(SCrimeNode* node);      // 0x004292C0 (_Rb_global<bool>::_M_increment)
struct CCrimeStatusMap {                                    // std::map<CCharString, MirroredCriminalStatus> (retail 0x0C bytes)
    SCrimeNode*   _M_header;
    unsigned long _M_node_count;
    unsigned long _M_compare;
    MirroredCriminalStatus& operator[](const CCharString& key);  // 0x005812DF
};

struct CTCHeroStats {
    void*           __vftable;                              // +0x00
    void*           ParentThing;                            // +0x04 CTCBase
    unsigned char   _pad_0x08[0x4];
    CCrimeStatusMap OutstandingCrimes;                      // +0x0C
    CWorld* GetWorld() const;                               // 0x00686A70
    CWorldMap* GetWorldMap() const;                         // 0x00686D20
    const CThingSearchTools* GetThingSearchTools() const;   // 0x00686D90
    void GetOutstandingFineAndFramesLeft(const CCharString& region, long& fine, long& framesLeft);   // 0x0057DAF5
    void PrepareToKickHeroToMostRecentSafeRegion();
};

void CTCHeroStats::PrepareToKickHeroToMostRecentSafeRegion()
{
    long mostRecentExit = 0;
    CCharString currentRegion = GetWorldMap()->GetRegion(GetWorldMap()->GetCurrentRegionNumber()).Name;
    CThingMarker* marker;
    if (!GetThingSearchTools()->GetPFirstLeafThing(
            CFilter_Bind2<CThingFilter_HasDefName, CThingFilter_HasScriptData>(
                CThingFilter_HasDefName("REGION_KICK_TO_POINT"), CThingFilter_HasScriptData(currentRegion)),
            &marker)) {
        for (SCrimeNode* node = OutstandingCrimes._M_header->_M_left; node != OutstandingCrimes._M_header;
             node = RbTreeIncrement(node)) {
            long fine;
            long framesLeft;
            GetOutstandingFineAndFramesLeft(node->first, fine, framesLeft);
            if (fine == 0 && node->first != currentRegion) {
                if (node->second.LastTimeExited > mostRecentExit)
                    mostRecentExit = node->second.LastTimeExited;
            }
        }
        if (mostRecentExit != 0 && OutstandingCrimes[currentRegion].LastTimeExited > 0)
            OutstandingCrimes[currentRegion].LastTimeExited = GetWorld()->GetWorldFrame();
    }
}
