// Grade: HELD(bytes match retail) -- test cannot fail (main only prints PASS) | Codex source 00/bc/CEnginePrimitiveManagerAnimatedMeshesBase_FrameStartselectstheanimatedmeshframepointertabl_00bcea60.cpp, flags , pass pattern 00bcea60_TEST PASS
// CEnginePrimitiveManagerAnimatedMeshesBase::FrameStart -- selects the animated mesh frame pointer table by render mode (retail 00bcea60, 0xd0 bytes).
struct CEnginePrimitiveManagerAnimatedMeshesBase { unsigned char pad[0x2e8]; void *p0,*p1,*p3,*p5,*p7,*p2,*p4,*p6; void FrameStart(); };
struct SRenderSettings_01436e60 { unsigned char pad2[0x334]; int mode; };
extern SRenderSettings_01436e60 *g_pRenderSettings_01436e60; // 0x01436E60
void CEnginePrimitiveManagerAnimatedMeshesBase::FrameStart() {
  if (g_pRenderSettings_01436e60->mode == 2) {
    p0=pad+0x1a0; p1=pad+0x1a8; p2=pad+0x228; p3=pad+0x1c8;
    p4=pad+0x248; p5=pad+0x1e8; p6=pad+0x268; p7=pad+0x208;
  } else {
    p0=pad+0x198; p1=pad+0x98; p2=pad+0x118; p3=pad+0xb8;
    p4=pad+0x138; p5=pad+0xd8; p6=pad+0x158; p7=pad+0xf8;
  }
}