struct CPatchTesselationEdgeStrip {
    void* vftable;
};

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* strip);
void operator delete(void*);

void* __fastcall CPatchTesselationEdgeStrip_scalar_deleting_destructor(CPatchTesselationEdgeStrip* strip, int edx_unused, unsigned int flags)
{
    CPatchTesselationEdgeStrip_dtor(strip);
    if (flags & 1)
        operator delete(strip);
    return strip;
}