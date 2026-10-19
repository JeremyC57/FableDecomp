// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0BE10, 17 bytes. Catalog name: std::_Dest_val<...CMusicManager::CRegisteredMusicEntry...>
// (propagated label). Body: normalise two adjacent C3DVectors in place -- the one at +0x0, then the one at +0xc as a tail
// jump -- through C3DVector::Normalise() @ 0x00A14440 (x,y,z scaled by 1/sqrt(x*x+y*y+z*z), returns *this).
// __thiscall, this in ecx; the enclosing class is not attributed.
struct C3DVector { float X, Y, Z; C3DVector& Normalise(); };   // Normalise: 0x00A14440
struct S { C3DVector a; C3DVector b; void M(); };
void S::M(){ a.Normalise(); b.Normalise(); }
