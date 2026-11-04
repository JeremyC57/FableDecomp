// CGameMap::~CGameMap() -- the user destructor (empty body): reset the CGameMap vtable, free the Cells block, then ~CBaseClassNonCopyable
// Leaf: ??1CGameMap@@UAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B7570, 33 bytes, thiscall, tail `jmp ~CBaseClassNonCopyable`.
// Identity (proven): vtable 0x0125D558 stored first (RTTI .?AVCGameMap@@ : CBaseClassNonCopyable : CBaseClass); its slot-0
//   `??_G` 0x006B81A0 inlines the same body. Ego_r.pdb (0x006F3890) and FableWin.pdb (0x0249FF30) spell ??1CGameMap@@UAE@XZ
//   inside the class's own TU (Ego_r order Clear / SetDimensions / ~CGameMap / ctor): a user-declared destructor, as the
//   vtable store shows. Ego_r.pdb CGameMap: Cells (vector<CGameMapCell>) +0x04, Map, World, WorldSeed, SizeX, SizeY (all
//   trivially destructible); the landed-style ctor 0x006B7760 zeroes Cells at +0x04/+0x08/+0x0C. No callers (the deleting
//   destructor has its own copy). The catalog label "CTCInventoryItem::OnDie" is propagated.
// Callees: free 0x00BFEA14 (~_Vector_base's deallocate), CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300 (MAE).
// Shape: `virtual ~CGameMap() {}` defined out of line in the class TU: vtable store, Cells' STLport ~vector (trivial
//   CGameMapCell _Destroy, null-tested free), then the base destructor as the tail jump.
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14
#include "engine/CArray.h"

#include "engine/CBaseClassNonCopyable.h"           // ~CBaseClassNonCopyable 0x0099A300 (protected virtual)

class CMap;
class CWorld;
class CGameMapInit;
class CGameMapCell {                                    // Ego_r.pdb CGameMapCell, 7 bytes (trivially destructible)
public:
    unsigned char AtmosThemes[3];                       // +0x00
    unsigned char AtmosBlends[2];                       // +0x03
    signed char   SoundTheme;                           // +0x05
    unsigned char Music;                                // +0x06
};
class CGameMap : public CBaseClassNonCopyable {         // vtable 0x0125D558, 0x24 bytes in retail
public:
    virtual ~CGameMap();
private:
    CGameMap(CWorld& world, CMap& map, const CGameMapInit& init);   // 0x006B7760 (Ego_r AAE)
    std::vector<CGameMapCell> Cells;                    // +0x04
    CMap*          Map;                                 // +0x10
    CWorld*        World;                               // +0x14
    unsigned long* WorldSeed;                           // +0x18
    long           SizeX;                               // +0x1c
    long           SizeY;                               // +0x20
};

CGameMap::~CGameMap()
{
}