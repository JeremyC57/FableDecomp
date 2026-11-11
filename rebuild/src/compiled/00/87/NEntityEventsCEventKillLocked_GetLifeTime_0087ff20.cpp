// NEntityEvents::CEventKillLocked::GetLifeTime() const -- returns 0x50
// Leaf: ?GetLifeTime@CEventKillLocked@NEntityEvents@@UBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087FF20, thiscall, `ret`.
// Identity: slot 3 of NEntityEvents::CEventKillLocked's vtable 0x01277CA8 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventKillLocked or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetLifeTime(void) at this slot; Ego_r/FableWin name set: ?GetLifeTime@CEventKillLocked@NEntityEvents@@UBEJXZ; `ret` matches the signature.
namespace NEntityEvents {
class CEventKillLocked {
public:
    virtual long GetLifeTime() const;
};
long CEventKillLocked::GetLifeTime() const
{
    return 0x50;
}
}
