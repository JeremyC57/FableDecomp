#pragma optimize("s",on)
struct CMember { void dtor(); };
extern void __fastcall CMember_dtor_0045185f(CMember* self);

struct CBase { void dtor(); };
extern void __fastcall CBase_dtor(CBase* self);

void __cdecl free(void*);

struct CPatchTesselationEdgeStrip {
    char pad0[0x50];
    void* p50;
    char pad54[0x8];
    void* p5c;
    char pad60[0x8];
    void* p68;
    char pad6c[0x8];
    void* p74;
    char pad78[0x8];
    void* p80;
    char pad84[0x8];
    CMember m8c;
};

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    CMember_dtor_0045185f(&self->m8c);
    if (self->p80) free(self->p80);
    if (self->p74) free(self->p74);
    if (self->p68) free(self->p68);
    if (self->p5c) free(self->p5c);
    if (self->p50) free(self->p50);
    CBase_dtor((CBase*)self);
}