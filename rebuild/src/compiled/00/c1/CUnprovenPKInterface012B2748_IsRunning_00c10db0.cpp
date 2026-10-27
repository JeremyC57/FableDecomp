// CUnprovenPKInterface_012B2748::IsRunning() -- 1 while the library world at +0x7BC is not paused, else 0
// Leaf: ?IsRunning@CUnprovenPKInterface_012B2748@@UAEHXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C10DB0, 19 bytes, thiscall: `xor ecx,ecx ; test dl,dl ; sete cl ; mov eax,ecx`
//   (an int result of `!Paused`, widened through a full register).
// Identity (descriptive): slot 10 of vtable 0x012B2748 (slot 7 is the landed SetMember7C0 0x00C11120, slot 8 its getter
//   0x00C11150). The object at +0x7BC is the steering-library world whose bool Paused sits at +0x18 (landed
//   CWorld::SetAsPaused 0x00C1C620). libpk has no RTTI and no PDB.
class CLibWorld_00c1c620 {                      // the steering library's world (landed as CWorld 0x00C1C620; not the game's CWorld)
public:
    unsigned char _pad[0x18];
    bool          Paused;                       // +0x18
};

class CUnprovenPKInterface_012B2748 {
public:
    virtual int IsRunning();                    // slot 10 (declared alone: only this body is emitted)
    unsigned char _body[0x7BC - 0x04];          // +0x04 .. (not touched here)
    CLibWorld_00c1c620* m_pWorld;                           // +0x7BC
};

int CUnprovenPKInterface_012B2748::IsRunning()
{
    return !m_pWorld->Paused;
}