// CTCInventoryMap::GetGuildSealRecallRegion() const -- the region number that holds the world's guild-seal recall position, or 0 while no recall position is set (a zero vector)
// Leaf: ?GetGuildSealRecallRegion@CTCInventoryMap@@ABEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005FA990, length 111 bytes, thiscall, plain `ret`.
// Identity: FableWin.pdb ?GetGuildSealRecallRegion@CTCInventoryMap@@ABEJXZ (private const, returns long).
// Callees: CTCBase::PeekWorld() const 0x00686A80 (twice; PDB ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ, the bound spelling; its
//   identical twin 0x00686A70 is GetWorld, in FableWin donor order), CWorld::PeekWorldMap() const (virtual, slot 13 +0x34 of
//   vtable 0x012390F0), CWorldMap::GetMapNumberAt(const C3DVector&) const (virtual, slot 16 +0x40 of vtable 0x01244AEC: the
//   C3DVector overload sits below the C2DCoordI one, overloads are laid out in reverse declaration order),
//   CWorldMap::GetRegionNumberMapIsIn(long) const 0x004FC190. Float: the 1e-4 epsilon 0x0129BA3C (squared at run time).
// Shape: the recall position is taken by const reference (CWorld::GetGuildSealRecallPos() const returns const C3DVector&, PDB)
//   and its inline GetSquaredMagnitude() reads it straight into x87 registers: Z, Y, X loads with X on top, X*X first.
//   Copying the vector to a local (implicit copy, a 3-float constructor, or a Z/Y/X body copy constructor; the parked
//   2026-10-15 forms) spills it or squares Y first. The map lookups pass the same reference (`push edi`, edi = world+0x154).
// Layout: `this` is CTCInventoryMap : NInventory::CTCInventoryBase : CTCBase (rtti_bases.tsv), CTCBase from engine/CTCBase.h
//   (`#define` include pattern; no member is touched here). CWorld / CWorldMap are local views: engine/CWorld.h flattens the
//   vptr and the C3DVector (GuildSealRecallPos_X/Y/Z at +0x154), so it cannot carry the virtual call; only the slots called
//   are named (CWorldMap slots as in the landed NCameraModes::IsLandscapeToBlockCameraBetween 0x00874F80).
#define CTCBase CTCBaseLayout
#include "engine/CTCBase.h"
#undef CTCBase

class C3DVector {
public:
    float X, Y, Z;
    float GetSquaredMagnitude() const { return X * X + Y * Y + Z * Z; }
};

extern const float EPSILON;          // 0x0129BA3C (1e-4; descriptive, an existing bound spelling of this address)

class CWorldMap {
public:
    virtual ~CWorldMap();                                // slot 0
    virtual void DeleteAllLevels();                      // slot 1
    virtual void DeleteAllRegions();                     // slot 2
    virtual void LoadFromFile();                         // slot 3
    virtual void SaveToFile();                           // slot 4
    virtual void RemoveLevel(long map);                  // slot 5
    virtual void LoadTopologyFromFile();                 // slot 6
    virtual void PostLoadTopology(long map);             // slot 7
    virtual void ForceLoad(long map);                    // slot 8
    virtual void ForceUnload(long map);                  // slot 9
    virtual bool IsMapLoaded(long map) const;            // slot 10
    virtual bool IsMapUsed(long map) const;              // slot 11
    virtual long GetGranularity() const;                 // slot 12
    virtual void GetDimensions() const;                  // slot 13
    virtual void GetMapAt();                             // slot 14
    virtual void PeekMapAt() const;                      // slot 15
    virtual long GetMapNumberAt(const long* coord) const;      // slot 17 (+0x44, the C2DCoordI form)
    virtual long GetMapNumberAt(const C3DVector& pos) const;   // slot 16 (+0x40)
    long GetRegionNumberMapIsIn(long map) const;         // 0x004FC190
};

class CWorld {                                           // vtable 0x012390F0 (local view, see Layout)
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual const CWorldMap& PeekWorldMap() const;       // slot 13 (+0x34)
    const C3DVector& GetGuildSealRecallPos() const { return GuildSealRecallPos; }
    unsigned char _pad_0x04[0x150];
    C3DVector     GuildSealRecallPos;                    // +0x154
};

class CTCBase : public CTCBaseLayout {
protected:
    const CWorld& PeekWorld() const;                     // 0x00686A80
};

namespace NInventory {
class CTCInventoryBase : public CTCBase {
};
}

class CTCInventoryMap : public NInventory::CTCInventoryBase {
private:
    long GetGuildSealRecallRegion() const;
};

long CTCInventoryMap::GetGuildSealRecallRegion() const
{
    const C3DVector& recallPos = PeekWorld().GetGuildSealRecallPos();
    if (recallPos.GetSquaredMagnitude() > EPSILON * EPSILON) {
        const CWorldMap& worldMap = PeekWorld().PeekWorldMap();
        return worldMap.GetRegionNumberMapIsIn(worldMap.GetMapNumberAt(recallPos));
    }
    return 0;
}