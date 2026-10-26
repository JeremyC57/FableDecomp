// CThingHolySite::`vector deleting destructor' @ 0x00529ab0
// Pattern: call scalar dtor (this in ecx), then if (flags&1) operator delete(this),
// return this. Modeled as a free __fastcall so it always has external linkage.

class CThingHolySite;

void __fastcall dtor_scalar_529ad0(CThingHolySite* site);  // 0x529ad0
void __fastcall op_delete_4c73b0(CThingHolySite* site);    // 0x4c73b0

void* __fastcall CThingHolySite_scalar_deleting_destructor(
    CThingHolySite* site, int /*edx*/, unsigned int flags)
{
    dtor_scalar_529ad0(site);
    if (flags & 1)
        op_delete_4c73b0(site);
    return site;
}