#pragma optimize("s",on)
// Leaf: ??1CPatchTesselationEdgeStrip@
extern void __cdecl eng_free(void*);

struct CPatchTesselationEdgeStripBase {
};

extern void __fastcall Base_dtor_00430300(void* self);

class CPatchTesselationEdgeStrip { public:
    char pad00[0x40];
    void* p40;   /* +0x40 */
    char pad44[0x4c-0x44];
    void* p4c;   /* +0x4c */
    char pad50[0x58-0x50];
    void* p58;   /* +0x58 */
    char pad5c[0x90-0x5c];
    void* p90;   /* +0x90 */
};

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    if (self->p90)
        eng_free(self->p90);
    if (self->p58)
        eng_free(self->p58);
    if (self->p4c)
        eng_free(self->p4c);
    if (self->p40)
        eng_free(self->p40);
    Base_dtor_00430300(self);
}