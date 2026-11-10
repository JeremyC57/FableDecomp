// CWorldMap::UpdateDestroyedThemeAreas() -- walk the queued destroyed-theme positions growing one box; whenever a position would stretch the box beyond 5 cells, report the box (bordered by one cell) as a changed height area and start a new box there; report the last box and empty the queue
// Leaf: ?UpdateDestroyedThemeAreas@CWorldMap@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FEC80, 337 bytes, thiscall, `ret`, frame 0x30.
// Identity: Ego_r.pdb / FableWin.pdb ?UpdateDestroyedThemeAreas@CWorldMap@@IAEXXZ (protected).
// Callees: C2DBoxI::ExtendToContain(const C2DCoordI&) 0x00A39E20, C2DBoxI::ExtendBordersBy(long) 0x00A39E10,
//   CWorldMap::HeightAreaChanged(const C2DBoxI*) virtual slot 28 (+0x70, 0x004FEB70).
// Layout: DestroyedThemePositions (vector<C2DCoordI>) +0xB0 from engine/CWorldMap.h; the queue is emptied by STLport's inline
//   clear() (erase(begin(), end()): the copy-down of the empty tail, then the finish store).
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_UPDATEDESTROYEDTHEMEAREAS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_EXTEND
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CWorldMap.h"

void CWorldMap::UpdateDestroyedThemeAreas()
{
    long count = DestroyedThemePositions.size();
    if (count == 0)
        return;
    C2DCoordI pos = DestroyedThemePositions[0];
    C2DBoxI area(pos, pos);
    for (long i = 0; i < count; ++i)
    {
        pos = DestroyedThemePositions[i];
        C2DBoxI extended = area;
        extended.ExtendToContain(pos);
        if (extended.BRX - extended.TLX > 5 || extended.BRY - extended.TLY > 5)
        {
            area.ExtendBordersBy(1);
            HeightAreaChanged(&area);
            area = C2DBoxI(pos, pos);
        }
        else
            area = extended;
    }
    area.ExtendBordersBy(1);
    HeightAreaChanged(&area);
    DestroyedThemePositions.clear();
}