// NCreatureMode::CModeFillJug::GetType() const -- returns 0x2b
// Leaf: ?GetType@CModeFillJug@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007410C0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeFillJug's vtable 0x0126812C (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeFillJug or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeFillJug@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeFillJug {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeFillJug::GetType() const
{
    return 0x2b;
}
}
