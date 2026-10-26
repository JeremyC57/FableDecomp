// (the short widened inside the accessor puts the movsx in eax and the marker-graphic load in ecx/edx, as retail)
// CMiniMapDisplay::InitialiseDemonDoorMarkers() -- for every demon door in the currently loaded region whose minimap flag is set, add an automatic marker with the demon-door marker graphic
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082ADD0, length 171 bytes (`ret`).
// Callees: CWorld::PeekWorldMap (slot 13) and PeekThingManager (slot 4) through the game singleton 0x013B86A0 (+0x24 world),
// CWorldMap::GetRegionNumberMapIsIn(long) @ 0x004FC190, CWorldMap::GetLoadedRegion @ 0x004FB150 (labelled CWorld::DrawGetEnvironment;
// chunk 178 names it), PeekGuiDef @ 0x00438570 (`__fastcall`), CMiniMapDisplay::AddAutoMarker(const CThing*, long) @ 0x0082A500
// (labelled Map_InsertMarker: it builds a CMarkerInfo with AutoMarker = true).
// Layout: retail CMiniMapDisplay PRegion +0xB8 (see 0x0082BA00); CPlayerGuiDef::MiniMapMarkerGraphics (vector<long>) at retail +0x958
// (PDB +0x9E0: 34 earlier 16-byte vectors are 12 bytes in retail, and the def base sits 4 low); element 10 is the demon-door graphic.
// The thing manager's demon-door registry (+4), its thing list (+0x14) and the door's minimap flag (+0x15D bit 3) carry descriptive
// names; the map number is Ego_r.pdb CThing::CurrentMapNumber (short, PDB +0x9E, retail +0x8E; setter SetCurrentMapNumber(long)).
// Shape: `this`, the world map and the door's region number live in the 12-byte frame; the vector element is reached through the
// STLport `*(begin() + 10)` (the dead `add eax,958h` is the vector's `this`).
// Layout (supersedes any local-model note above): engine/CMiniMapDisplay.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CMiniMapDisplay CMiniMapDisplayDonor
#include "engine/CMiniMapDisplay.h"
#undef CMiniMapDisplay
class CThing { public:
    unsigned char _pad00[0x8e];
    short         CurrentMapNumber;             // +0x8e (Ego_r.pdb +0x9e)
    long GetCurrentMapNumber() const { return CurrentMapNumber; }
    unsigned char _pad90[0x15d - 0x90];
    unsigned char MinimapFlags;                 // +0x15d (bit 3: show a marker)
};
template<class T> struct vector {               // STLport vector
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    const T& operator[](unsigned long i) const { return *(begin() + i); }
};
class CPlayerGuiDef { public:
    unsigned char _pad00[0x958];
    vector<long>  MiniMapMarkerGraphics;        // +0x958
};
class CPlayerGui {
public:
    static const CPlayerGuiDef& __fastcall PeekGuiDef();   // 0x00438570
};         // 0x00438570
struct CThingList {                             // retail one-pointer list of CThing*
    struct node { node* next; node* prev; CThing* value; };
    node* head;
};
struct CDemonDoorRegistry {
    unsigned char _pad00[0x14];
    CThingList    DemonDoors;                   // +0x14
};
class CThingManager { public:
    void*               __vftable;
    CDemonDoorRegistry* PDemonDoorRegistry;     // +0x04
};
class CWorldMap { public:
    long GetRegionNumberMapIsIn(long map) const;    // 0x004FC190
    long GetLoadedRegion() const;                   // 0x004FB150
};
class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual CThingManager* PeekThingManager();  // slot 4 (+0x10)
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12();
    virtual CWorldMap* PeekWorldMap();          // slot 13 (+0x34)
};
class CGame { public:
    unsigned char _pad00[0x24];
    CWorld*       World;                        // +0x24
};
extern CGame* g_pGame;                          // 0x013B86A0
class CRegion;
class CMiniMapDisplay : public CMiniMapDisplayDonor {
 public:
    void AddAutoMarker(const CThing* thing, long graphicIndex);   // 0x0082A500
    void InitialiseDemonDoorMarkers();
};

void CMiniMapDisplay::InitialiseDemonDoorMarkers()
{
    if (!PRegion)
        return;
    CWorld* world = g_pGame->World;
    CWorldMap* map = world->PeekWorldMap();
    CThingList& doors = world->PeekThingManager()->PDemonDoorRegistry->DemonDoors;
    for (CThingList::node* n = doors.head->next; n != doors.head; n = n->next) {
        CThing* door = n->value;
        if (map->GetLoadedRegion() == map->GetRegionNumberMapIsIn(door->GetCurrentMapNumber()) && (door->MinimapFlags & 8))
            AddAutoMarker(door, CPlayerGui::PeekGuiDef().MiniMapMarkerGraphics[10]);
    }
}