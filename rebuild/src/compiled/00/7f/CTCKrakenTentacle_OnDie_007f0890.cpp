// CTCKrakenTentacle::OnDie() @ 0x007F0890 -- kill the ripple effect, remove the tentacle's obstruction, then the empty CTCBase::OnDie
// Leaf: ?OnDie@CTCKrakenTentacle@@UAEXXZ
// Naming wave 30 (family c699): Ego_r.pdb / FableWin.pdb ?OnDie@CTCKrakenTentacle@@UAEXXZ (vtable slot 13, OnDie in the CTCBase order);
//   callees private ?KillRippleFX@CTCKrakenTentacle@@AAEXXZ 0x007F07A0 (both PDBs; the landed 0x007F07A0 Leaf), ?RemoveObstruction@
//   CTCKrakenTentacle@@QAEXXZ 0x007F0810 and the tail ?OnDie@CTCBase@@UAEXXZ 0x006868E0. Was the free __fastcall Dest_val calling the
//   invented extern "C" Dtor_first / Dtor_node.
// Shape: 23 bytes: push esi; mov esi,ecx; call; mov ecx,esi; call; mov ecx,esi; pop esi; jmp (size TU).
#pragma optimize("s",on)

class CTCBase { public:
    virtual void OnDie();       // slot 13, 0x006868E0
};

class CTCKrakenTentacle : public CTCBase {
public:
    virtual void OnDie();
    void RemoveObstruction();   // 0x007F0810
private:
    void KillRippleFX();        // 0x007F07A0
};

void CTCKrakenTentacle::OnDie()
{
    KillRippleFX();
    RemoveObstruction();
    CTCBase::OnDie();
}