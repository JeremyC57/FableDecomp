// NShape::CTrack::GetType() const -- returns (EShape)1
// Leaf: ?GetType@CTrack@NShape@@UBE?AW4EShape@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007650C0, thiscall, `ret`.
// Identity: slot 1 of NShape::CTrack's vtable 0x0126A288 (defined here: the slot holds this body in 1 vtable(s), all of NShape::CTrack or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 2 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CTrack@NShape@@UBE?AW4EShape@2@XZ; `ret` matches the signature.
namespace NShape { enum EShape { }; }
namespace NShape {
class CTrack {
public:
    virtual NShape::EShape GetType() const;
};
NShape::EShape CTrack::GetType() const
{
    return (EShape)1;
}
}
