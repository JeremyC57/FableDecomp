// CEnginePrimitiveManagerStaticMeshesBase::FrameStart -- selects the static mesh buffer pointers for the active render mode (retail 00b9d100, 0xb8 bytes).
struct SRenderSettings_01436e60 { unsigned char pad[0x334]; int mode; };
extern SRenderSettings_01436e60 *g_pRenderSettings_01436e60; // 0x01436E60
struct CEnginePrimitiveManagerStaticMeshesBase { unsigned char pad[0x2f0]; void *p1,*p2,*p3,*p4,*p5,*p6,*p0; void FrameStart(); };
void CEnginePrimitiveManagerStaticMeshesBase::FrameStart() {
  if (g_pRenderSettings_01436e60->mode == 2) {
    p0=pad+0x200; p1=pad+0x208; p2=pad+0x228; p3=pad+0x248;
    p4=pad+0x268; p5=pad+0x288; p6=pad+0x2a8;
  } else {
    p0=pad+0x1f8; p1=pad+0xd8; p2=pad+0xf8; p3=pad+0x118;
    p4=pad+0x138; p5=pad+0x158; p6=pad+0x178;
  }
}
