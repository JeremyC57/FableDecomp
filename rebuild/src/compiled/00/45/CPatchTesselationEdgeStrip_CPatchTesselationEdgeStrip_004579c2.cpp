#pragma optimize("s",on)
// Leaf: ??1CPatchTesselationEdgeStrip@
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
struct CBase { };
class CPatchTesselationEdgeStrip { public:
    unsigned char _pad0[0x3c];
    void* p3c;   // +0x3c
    unsigned char _pad2[0x48-0x40];
    void* p48;   // +0x48
    unsigned char _pad3[0x54-0x4c];
    void* p54;   // +0x54
};
void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    if (self->p54) free(self->p54);
    if (self->p48) free(self->p48);
    if (self->p3c) free(self->p3c);
    ((CParentDefClassBase*)((CBase*)self))->CParentDefClassBase::~CParentDefClassBase();
}