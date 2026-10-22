// CMap::GetMemoryFootprint @ 0x00c1ea80
// __fastcall long GetMemoryFootprint(CMap* this)
// sums two base/member footprint helpers plus [this+8]

struct CMap {
    char pad0[8];
    long CachedVolumes;              // [this+8]
    long BaseFootprintA();
    long BaseFootprintB();
    long GetMemoryFootprint();
};

long CMap::GetMemoryFootprint()
{
    long a = this->BaseFootprintA();
    a += this->BaseFootprintB();
    return a + this->CachedVolumes;
}