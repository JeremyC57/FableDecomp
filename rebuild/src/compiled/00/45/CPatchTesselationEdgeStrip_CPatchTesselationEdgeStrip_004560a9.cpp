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
  char pad[0x60];
  void* p60;   /* +0x60 */
  char pad2[0x70 - 0x64];
  void* p70;   /* +0x70 */
  char pad3[0x80 - 0x74];
  void* p80;   /* +0x80 */
};

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
  if (self->p80) free(self->p80);
  if (self->p70) free(self->p70);
  if (self->p60) free(self->p60);
  ((CParentDefClassBase*)(self))->CParentDefClassBase::~CParentDefClassBase();
}