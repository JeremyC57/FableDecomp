// CEnvironmentSpellEffectInterface::GetRenderFrameInterpolatedDisplacementParameters(float) -- blend current and previous monochrome colour.
// Retail 00885900, 226 bytes. No calls. PDB interface has no generated header; its native offsets agree with donor.
struct CRGBFloatColour {
 float R,G,B,A;
 CRGBFloatColour() {}
 CRGBFloatColour(float r,float g,float b,float a):R(r),G(g),B(b),A(a) {}
 CRGBFloatColour operator*(float s) const { return CRGBFloatColour(s*R,s*G,s*B,s*A); }
 CRGBFloatColour operator+(const CRGBFloatColour& x) const { return CRGBFloatColour(R+x.R,G+x.G,B+x.B,A+x.A); }
};
struct CIDrawEnvironmentDisplacementParameters { CRGBFloatColour MonochromeEffectColour; };
struct CEnvironmentSpellEffectInterface {
 unsigned char beforeDisplacement[0x54];
 CRGBFloatColour DisplacementParams;
 unsigned char beforePrevious[0x70];
 CRGBFloatColour PrevFrameDisplacementParams;
 CIDrawEnvironmentDisplacementParameters GetRenderFrameInterpolatedDisplacementParameters(float t);
};
CIDrawEnvironmentDisplacementParameters CEnvironmentSpellEffectInterface::GetRenderFrameInterpolatedDisplacementParameters(float t) {
 if(t<0.0f) t=0.0f; else if(t>1.0f) t=1.0f;
 CIDrawEnvironmentDisplacementParameters result;
 result.MonochromeEffectColour=PrevFrameDisplacementParams*(1.0f-t)+DisplacementParams*t;
 return result;
}