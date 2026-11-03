#define FABLE_RETAIL_C2DBOXI
#include "stlport/fable_containers.h"
#include "engine/C2DBoxI.h"
class CMap { public: bool IsPassableAt(const C2DCoordI&) const; bool IsPrefNavAt(const C2DCoordI&) const; };
void probe(const C2DBoxI& area, std::vector<unsigned char>& w, const C2DBoxI& localArea, const CMap& mapRef)
{
    for (long y = localArea.TLY, j = 0; y < localArea.BRY; ++y, ++j)
        for (long x = localArea.TLX, i = 0; x < localArea.BRX; ++x, ++i)
        {
            if (mapRef.IsPassableAt(C2DCoordI(x, y)))
            {
                if (mapRef.IsPrefNavAt(C2DCoordI(x, y)))
                    w[(area.BRX - area.TLX) * j + i] = 0;
                else
                    w[(area.BRX - area.TLX) * j + i] = 0x80;
            }
            else
                w[(area.BRX - area.TLX) * j + i] = 0xFF;
        }
}
