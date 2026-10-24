#pragma optimize("g",on)
// Leaf: GetNextRegionOnRouteTo
// Byte-exact reconstruction of CWorldMap::GetNextRegionOnRouteTo @ 0x00c23dd0
struct CWorldMap {
    char pad0[8];
    long count;   // +0x08
    char pad2[8];
    long* array;  // +0x14
};

// member fn @ 0xc23df0 (__fastcall, returns long index)
extern long __fastcall CWorldMap_helper_00c23df0(CWorldMap* self);

long __fastcall GetNextRegionOnRouteTo(CWorldMap* self)
{
    if (self->count == 0)
        return 0;
    long idx = CWorldMap_helper_00c23df0(self);
    long r = self->array[idx];
    self->count = self->count - 1;
    return r;
}