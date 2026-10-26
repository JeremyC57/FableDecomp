#pragma optimize("s",on)
// Leaf: ??1CPatchTesselationEdgeStrip@
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
extern "C" void __cdecl free(void* block);   // MSVCR71 import thunk 0x00BFEA14
class CPatchTesselationEdgeStrip { public:
    char pad0[0x3c];
    void* p3c;
    char pad1[0x48 - 0x3c - 4];
    void* p48;
};


void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    if (self->p48)
        free(self->p48);
    if (self->p3c)
        free(self->p3c);
    ((CParentDefClassBase*)(self))->CParentDefClassBase::~CParentDefClassBase();
}