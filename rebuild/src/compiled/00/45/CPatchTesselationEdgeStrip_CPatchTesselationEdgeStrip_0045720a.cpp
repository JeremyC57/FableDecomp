#pragma optimize("s",on)
struct CSub { };
extern void __fastcall CSub_dtor(CSub* self);
void __cdecl free(void*);
extern void __fastcall CBase_dtor(void* self);

struct CPatchTesselationEdgeStrip {
    char pad0[0x60];
    void* p60;
    char pad1[0x08];
    void* p6c;
    char pad2[0x08];
    void* p78;
    char pad3[0x08];
    void* p84;
    char pad4[0x08];
    CSub sub90;
};

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    CSub_dtor(&self->sub90);
    if (self->p84) free(self->p84);
    if (self->p78) free(self->p78);
    if (self->p6c) free(self->p6c);
    if (self->p60) free(self->p60);
    CBase_dtor(self);
}