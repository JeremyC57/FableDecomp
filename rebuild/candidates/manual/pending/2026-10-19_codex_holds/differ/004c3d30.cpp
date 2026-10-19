// Grade: HELD(bytes match retail) -- test exercises a hand-written stand-in, not the candidate | Codex source 00/4c/CEnginePrimitiveAnimation_CEnginePrimitiveAnimationinitializesanimationres_004c3d30.cpp, flags , pass pattern 004c3d30_TEST PASS
// CEnginePrimitiveAnimation::CEnginePrimitiveAnimation -- initializes animation resource and frame data (retail 004c3d30, 0xb9 bytes).
struct Ref { int pad0; int refs; };
struct AnimIn { int pad0; int pad1; Ref *first; int pad3; Ref *second; };
struct Fourteen { int x[14]; };
struct CEnginePrimitiveAnimation {
  void *v0; void *v1; Ref *first; void *v3; Ref *second;
  void *v5; int frame[6]; void *v6; int tail[12];
  CEnginePrimitiveAnimation(const AnimIn& a, const Fourteen& b, const Fourteen& c);
};
CEnginePrimitiveAnimation::CEnginePrimitiveAnimation(const AnimIn& a, const Fourteen& b, const Fourteen& c)
 {
  v0=(void*)0x1239840; v1=(void*)0x1239838; first=a.first;
  if(first) ++first->refs;
  v3=(void*)0x1239838; second=a.second;
  if(second) ++second->refs;
  v5=(void*)0x123981c;
  frame[0]=b.x[1]; frame[1]=b.x[2]; frame[2]=b.x[3]; frame[3]=b.x[4]; frame[4]=b.x[5]; frame[5]=b.x[6];
  v6=(void*)0x1239824;
  tail[0]=c.x[1]; tail[1]=c.x[2]; tail[2]=c.x[3]; tail[3]=c.x[4]; tail[4]=c.x[5]; tail[5]=c.x[6]; tail[6]=c.x[7]; tail[7]=c.x[8]; tail[8]=c.x[9]; tail[9]=c.x[10]; tail[10]=c.x[11]; tail[11]=c.x[12];
}