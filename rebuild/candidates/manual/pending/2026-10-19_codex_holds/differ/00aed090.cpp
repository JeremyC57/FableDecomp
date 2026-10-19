// Grade: HELD(bytes match retail) -- test uses an EmitterModel without the real bitfields | Codex source 00/ae/CParticleEmitter_Set2DPosition_00aed090.cpp, flags , pass pattern 00aed090_TEST PASS
// CParticleEmitter::Set2DPosition(const C3DVector&, long) -- set both interpolator positions and transform translations.
// Retail @ 0x00AED090: 167 bytes. The layer group is a word at +0x42; retail reads its low byte,
// XORs the incoming layer, masks seven bits, then XORs the word to update only that bitfield.
// Grade: MATCH (VC7.1 Wine lane, q.py).
struct C3DVector { float X,Y,Z; };
struct C3DPositionInterpolator { virtual void v00(); C3DVector StartPosition; C3DVector EndPosition; };
struct CMatrix3x4 { C3DVector a,b,c; float PositionX,PositionY,PositionZ; void SetPosition(const C3DVector& v){PositionX=v.X;PositionY=v.Y;PositionZ=v.Z;} };
struct CParticleEmitter {
 virtual void v00(); unsigned char _base4[0x3c];
 unsigned short Priority:3; unsigned short a:1; unsigned short b:1; unsigned short c:1; unsigned short d:1; unsigned short e:1; unsigned short f:1; unsigned short g:1; unsigned short h:1; unsigned short Emitter2D:1; unsigned short LayerWord;
 unsigned char body44[0x94-0x44]; C3DPositionInterpolator PositionInterpolator; CMatrix3x4 Transform; unsigned char gap[0x110-0xe0]; CMatrix3x4 EndTransform;
 bool IsEmitter2D() const { return Emitter2D; } void Set2DPosition(const C3DVector&, long);
};
void CParticleEmitter::Set2DPosition(const C3DVector& position, long layer) {
 if (IsEmitter2D()) {
  PositionInterpolator.StartPosition.X=position.X; PositionInterpolator.StartPosition.Y=position.Y; PositionInterpolator.StartPosition.Z=0;
  PositionInterpolator.EndPosition.X=position.X; PositionInterpolator.EndPosition.Y=position.Y; PositionInterpolator.EndPosition.Z=0;
  if(layer != -1) LayerWord ^= ((*(unsigned char*)&LayerWord ^ layer) & 0x7f);
  Transform.SetPosition(PositionInterpolator.StartPosition);
  EndTransform.SetPosition(PositionInterpolator.EndPosition);
 }
}