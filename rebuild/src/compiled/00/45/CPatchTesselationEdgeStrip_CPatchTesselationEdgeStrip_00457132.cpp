#pragma optimize("s",on)
// Leaf: ??1CPatchTesselationEdgeStrip@
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

struct CPatchTesselationEdgeStripBase {
};


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
        free(self->p90);
    if (self->p58)
        free(self->p58);
    if (self->p4c)
        free(self->p4c);
    if (self->p40)
        free(self->p40);
    ((CParentDefClassBase*)(self))->CParentDefClassBase::~CParentDefClassBase();
}