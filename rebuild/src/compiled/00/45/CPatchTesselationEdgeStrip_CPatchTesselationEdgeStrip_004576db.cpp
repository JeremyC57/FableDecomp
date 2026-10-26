#pragma optimize("s",on)
// Leaf: ??1CPatchTesselationEdgeStrip@
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
class CPatchTesselationEdgeStrip { public:
    unsigned char _pad0[0x78];
    void* p78;   // +0x78
    unsigned char _pad1[0x8c-0x7c];
    void* p8c;   // +0x8c
    unsigned char _pad2[0x98-0x90];
    void* p98;   // +0x98
};

extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    if (self->p98) free(self->p98);
    if (self->p8c) free(self->p8c);
    if (self->p78) free(self->p78);
    ((CParentDefClassBase*)(self))->CParentDefClassBase::~CParentDefClassBase();
}