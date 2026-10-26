// CGameNavigatorManagerExternalInterface::Clone() -- heap copy of this 8-byte interface object (vtable + world pointer)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AB210, length 35 bytes.
// Callees: operator new @ 0x00BFEA1A. Vtable 0x01238C58 (CGameNavigatorManagerExternalInterface slot 2 = this function).
// Shape: `new CGameNavigatorManagerExternalInterface(*this)` with the implicit copy constructor inlined (vtable store, World copy)
// under operator new's null test. Layout from the PDB (size 8: vptr, CWorld* World).
class CWorld;

class CGameNavigatorManagerExternalInterface { public:
    virtual ~CGameNavigatorManagerExternalInterface();                   // slot 0 @ 0x004A9260
    virtual void Initialise();                                           // slot 1
    virtual CGameNavigatorManagerExternalInterface* Clone() const;       // slot 2 @ 0x004AB210
    CWorld* World;                                                       // +0x04
};

CGameNavigatorManagerExternalInterface* CGameNavigatorManagerExternalInterface::Clone() const
{
    return new CGameNavigatorManagerExternalInterface(*this);
}