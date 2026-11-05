// CGameEventPackage::Clear() -- empty one network game-event package (the event count goes to zero)
// Leaf: ?Clear@CGameEventPackage@@QAEXXZ
// Retail: Fable.exe @ 0x009F16C0, 8 bytes, __thiscall: `mov dword ptr [ecx+4],0 ; ret`.
// Name: Ego_r.pdb and FableWin.pdb both carry ?Clear@CGameEventPackage@@QAEXXZ; the landed caller
// CNetworkClient::GetLocalGameEventPackageSet 0x004AEAA0 binds it under that spelling. Layout from Ego_r.pdb
// (struct_layouts_egor.tsv): TimeStamp +0x00, NoEvents +0x04, Events CGameEvent[40] +0x08. Only the count is reset.
// No shared engine header exists for CGameEventPackage, so the class is modelled here with its PDB member names.
class CGameEventPackage
{
public:
    long TimeStamp;              // +0x00
    unsigned long NoEvents;      // +0x04
    void Clear();
};

void CGameEventPackage::Clear()
{
    NoEvents = 0;
}
