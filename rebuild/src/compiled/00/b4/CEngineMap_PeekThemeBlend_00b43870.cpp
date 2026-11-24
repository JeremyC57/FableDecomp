// Original Ego_d locals: xp, yp, level, and one block-local map.
// FTOL_SCOPE_TRAPPER is checked-build-only and generates no release operations.
// Original global: NEngineInternal::GPEngineWorldMap (Anniversary PDB and native engine forwarders).
// Leaf: ?PeekThemeBlend@CEngineMap@@QBEEJJJ@Z
// Inline-budget: original nested GetMap bounds are expanded in retail.
// Flags: /d2inlT301
#define FABLE_RETAIL_CENGINEMAP_PEEK
#define FABLE_RETAIL_CIDRAWMAP
#define FABLE_RETAIL_CIDRAWMAP_DRAW
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GETMAP_ORIGINAL_PASS26
#define FABLE_RETAIL_CENGINEWORLDMAP_GETMAPXY
#include "engine/CEngineMap.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawMap.h"

namespace NEngineInternal { extern CEngineWorldMap* GPEngineWorldMap; }

unsigned char CEngineMap::PeekThemeBlend(long xp, long yp, long level) const
{
    if (!MapHasDataAtPoint(xp,yp)) {
        CEngineMap* map = NEngineInternal::GPEngineWorldMap->GetMap(
            PeekWorldX() + xp,
            PeekWorldY() + yp);
        if (map && map->MapHasDataAtPoint(PeekWorldX() + xp - map->PeekWorldX(), PeekWorldY() + yp - map->PeekWorldY()))
            return map->PeekThemeBlend(PeekWorldX() + xp - map->PeekWorldX(), PeekWorldY() + yp - map->PeekWorldY(), level);
        if (xp < 0)
            xp = 0;
        else if (xp > PeekWidth() - 1)
            xp = PeekWidth() - 1;
        if (yp < 0)
            yp = 0;
        else if (yp > PeekHeight() - 1)
            yp = PeekHeight() - 1;
    }
    if (GameMap)
        return GameMap->DrawGetEngineBlendAt(xp, yp, level);
    return 0;
}