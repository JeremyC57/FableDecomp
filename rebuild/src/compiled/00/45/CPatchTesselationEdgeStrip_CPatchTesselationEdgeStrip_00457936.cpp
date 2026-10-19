#pragma optimize("s",on)

struct CSub60 { void dtor(); };
struct CBase { void dtor(); };

struct CPatchTesselationEdgeStrip {
    unsigned char _pad_0x0[0x3c];
    void* p3c;   // +0x3c
    unsigned char _pad_0x40[0x8];
    void* p48;   // +0x48
    unsigned char _pad_0x4c[0x8];
    void* p54;   // +0x54
    unsigned char _pad_0x58[0x8];
    CSub60 sub60; // +0x60
};

void __cdecl free(void*);

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    self->sub60.dtor();
    if (self->p54) free(self->p54);
    if (self->p48) free(self->p48);
    if (self->p3c) free(self->p3c);
    ((CBase*)self)->dtor();
}