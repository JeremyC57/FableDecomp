// CWorldMap::AreaCoversOneLevel(const C2DBoxI&) const -- true when all four corners of a world-map area fall in the same (non-zero) map
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: the local C2DCoordI removed too (engine/C3DVector.h includes engine/C2DCoordI.h, Ego_r TPI 0xa59e: public long X/Y, size 8).
// Retail: Fable.exe @ 0x004FB160, 231 bytes. PDB ?AreaCoversOneLevel@CWorldMap@@QBE_NABVC2DBoxI@@@Z
// Callees: CWorldMap virtual slot 17 GetMapNumberAt(const C2DCoordI&) const (vtable 0x01244AEC -> 0x004FC080; slot 16
// is the C3DVector overload 0x004FC0C0).
// Shape: three chained corner comparisons (TL==TR, TR==BL, BL==BR) then BR != 0; each comparison builds two C2DCoordI
// temporaries (VC7.1 calls the right-hand side first), all in two 8-byte frame slots; one shared `xor al,al` exit.
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
#include "engine/C3DVector.h"

class CWorldMap { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    // VC7.1 lays overloaded virtuals out in reverse declaration order: C2DCoordI first gives slot 17.
    virtual long GetMapNumberAt(const C2DCoordI& pos) const;      // slot 17 (0x004FC080)
    virtual long GetMapNumberAt(const C3DVector& pos) const;      // slot 16 (0x004FC0C0)
    bool AreaCoversOneLevel(const C2DBoxI& area) const;
};

bool CWorldMap::AreaCoversOneLevel(const C2DBoxI& area) const
{
    if (GetMapNumberAt(C2DCoordI(area.TLX, area.TLY)) == GetMapNumberAt(C2DCoordI(area.BRX, area.TLY))
        && GetMapNumberAt(C2DCoordI(area.BRX, area.TLY)) == GetMapNumberAt(C2DCoordI(area.TLX, area.BRY))
        && GetMapNumberAt(C2DCoordI(area.TLX, area.BRY)) == GetMapNumberAt(C2DCoordI(area.BRX, area.BRY))
        && GetMapNumberAt(C2DCoordI(area.BRX, area.BRY)) != 0)
        return true;
    return false;
}