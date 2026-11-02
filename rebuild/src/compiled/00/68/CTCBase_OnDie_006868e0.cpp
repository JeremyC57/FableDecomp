// CTCBase::OnDie() @ 0x006868E0 -- the empty base slot (a bare `ret`)
// Leaf: ?OnDie@CTCBase@@UAEXXZ
// Naming wave 30 (family c699): Ego_r.pdb / FableWin.pdb ?OnDie@CTCBase@@UAEXXZ, CTCBase vtable 0x0125BE0C slot 13 (OnDie in the CTCBase
//   virtual order: OnDie 13, OnKill 14 (chunk 695); every derived slot-13 body of the hierarchy is an OnDie and tails here: CTCInventoryItem
//   0x005D8EC0, CTCScriptedControl 0x00713740, CTCKrakenTentacle 0x007F0890). Was the placeholder struct C006868e0 (?OnDie@C006868e0@@QAEXXZ).
// Layout: CTCBase {vptr, ParentThing +0x04, ToBeRemoved +0x08, IsInGlobalUpdateTCs +0x09} (Ego_r.pdb, 0xc); only the slot is modelled.
class CTCBase { public:
    virtual void OnDie();   // slot 13
};

void CTCBase::OnDie()
{
}