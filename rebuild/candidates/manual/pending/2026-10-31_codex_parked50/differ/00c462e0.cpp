// 00c462e0 CUnprovenClass_00c462e0::CUnprovenMethod_00c462e0
// Codex parked-50 best attempt: DIFFER(51v51); residue: 51/51 bytes; second corner load is delayed until after the first interpolation spill. Original function/class identity remains unproven.
// Flags:
// Leaf: ?CUnprovenMethod_00c462e0@CUnprovenClass_00c462e0@@QBEMMM@Z
// Layout: four scalar reads at0/4/8/12. Original class and member names unproved.
class CUnprovenClass_00c462e0 {
 float CUnprovenCorner0_00c462e0,CUnprovenCorner1_00c462e0;
 float CUnprovenCorner2_00c462e0,CUnprovenCorner3_00c462e0;
public:
 float CUnprovenMethod_00c462e0(float u,float v)const;
};
float CUnprovenClass_00c462e0::CUnprovenMethod_00c462e0(float u,float v)const {
 float c0=CUnprovenCorner0_00c462e0;
 float c1=CUnprovenCorner1_00c462e0;
 float c2=CUnprovenCorner2_00c462e0;
 float c3=CUnprovenCorner3_00c462e0;
 struct {float bottom,top;} edge;
 edge.bottom=(c3-c2)*u+c2;
 edge.top=(c1-c0)*u+c0;
 return(edge.top-edge.bottom)*v+edge.bottom;
}
