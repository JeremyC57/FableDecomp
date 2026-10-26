#pragma optimize("s",on)
// Leaf: ??1CPatchTesselationEdgeStrip@
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

class CPatchTesselationEdgeStrip { public:
    unsigned char _pad_0x0[0x48];
    void* p48;   // +0x48
    unsigned char _pad_0x4c[0x8];
    void* p54;   // +0x54
};

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    if (self->p54)
        free(self->p54);
    if (self->p48)
        free(self->p48);
    ((CParentDefClassBase*)(self))->CParentDefClassBase::~CParentDefClassBase();
}