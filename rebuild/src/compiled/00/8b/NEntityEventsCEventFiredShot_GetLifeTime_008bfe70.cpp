// NEntityEvents::CEventFiredShot::GetLifeTime() const -- returns 0xc8
// Leaf: ?GetLifeTime@CEventFiredShot@NEntityEvents@@UBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008BFE70, thiscall, `ret`.
// Identity: slot 3 of NEntityEvents::CEventFiredShot's vtable 0x0127F2C0 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventFiredShot or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetLifeTime(void) at this slot; Ego_r/FableWin name set: ?GetLifeTime@CEventFiredShot@NEntityEvents@@UBEJXZ; `ret` matches the signature.
namespace NEntityEvents {
class CEventFiredShot {
public:
    virtual long GetLifeTime() const;
};
long CEventFiredShot::GetLifeTime() const
{
    return 0xc8;
}
}
