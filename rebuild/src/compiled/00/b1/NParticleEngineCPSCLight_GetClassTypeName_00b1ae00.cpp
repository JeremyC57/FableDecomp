// NParticleEngine::CPSCLight::GetClassTypeName() const @ 0x00B1AE00 -- returns the class type name "CPSCLight" as a CCharString by value
// Leaf: ?GetClassTypeName@CPSCLight@NParticleEngine@@UBE?AVCCharString@@XZ
// Identity (NF385): retail vftable 0x012A0624 (RTTI NParticleEngine::CPSCLight) slot 3 = FableWin slot 3
// ?GetClassTypeName@CPSCLight@NParticleEngine@@UBE?AVCCharString@@XZ; both PDB name sets (psc_light.obj; Ego_r 25 B = retail).
// The literal: retail pushes 0x0129F5E8 = "CPSCLight" (the row had modelled it as GetChunkDescription returning "Bounding Sphere",
// which is 0x00AA7100's literal; the relocation masked the difference).
// Body: CCharString::CCharString(char const*, int) at 0x0099EBF0 into the hidden return slot, `ret 4`.
class CCharString {
public:
    // ctor at 0x0099ebf0 : CCharString(char const* s, int n)
    CCharString(const char* s, long n);
private:
    char* m_p;
};
namespace NParticleEngine {
class CPSCLight {
public:
    virtual CCharString GetClassTypeName() const;
};
}
CCharString NParticleEngine::CPSCLight::GetClassTypeName() const
{
    return CCharString("CPSCLight", -1);
}
