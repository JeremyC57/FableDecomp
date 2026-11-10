// NHeroStatModifierEvent::CBase::~CBase() -- the hero-stat event base's (non-virtual, empty) destructor: re-point the object at the base vtable
// Leaf: ??1CBase@NHeroStatModifierEvent@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083AD90, 7 bytes, thiscall: mov dword ptr [ecx],0127232Ch ; ret (no `mov eax,ecx`: a destructor,
//   not a constructor).
// Identity: the vtable 0x0127232C carries the RTTI .?AVCBase@NHeroStatModifierEvent@@ (complete object locator 0x01346B34, no
//   bases) and one slot, _purecall: ModifyHeroStats, pure in the base and overridden by CHitThing 0x00815DF0-family vtables
//   0x01272FA0 / CKnockedThingUnconscious 0x01272334 / CKilledThing 0x0127233C (Ego_r.pdb
//   ?ModifyHeroStats@C<...>@NHeroStatModifierEvent@@UBEXAAVCTCHeroStats@@@Z). FableWin.pdb ??1CBase@NHeroStatModifierEvent@@QAE@XZ:
//   public and NOT virtual (the vtable has no destructor slot), so the body is the vptr store alone. Ego_r.pdb: CBase is 4 bytes
//   (the vptr). Not called or referenced in retail (an unreferenced COMDAT of the TU of CHitThing's constructor 0x0083AE30).
class CTCHeroStats;
namespace NHeroStatModifierEvent {
class CBase {
public:
    ~CBase();
    virtual void ModifyHeroStats(CTCHeroStats& heroStats) const = 0;   // slot 0 (_purecall here)
};

CBase::~CBase()
{
}
}