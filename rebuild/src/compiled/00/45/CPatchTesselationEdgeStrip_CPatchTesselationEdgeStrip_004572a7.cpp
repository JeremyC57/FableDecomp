#pragma optimize("s",on)
// Leaf: ??1CPatchTesselationEdgeStrip@
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
struct CBase { int base_pad; };
class CPatchTesselationEdgeStrip { public:
    char pad[0x68];
    void* p68;   /* +0x68 */
    char pad2[0x74 - 0x68 - 4];
    void* p74;   /* +0x74 */
};
void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
    if (self->p74)
        free(self->p74);
    if (self->p68)
        free(self->p68);
    ((CParentDefClassBase*)((CBase*)self))->CParentDefClassBase::~CParentDefClassBase();
}