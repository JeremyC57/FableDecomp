// CObject_00c44b40::Method_00c44b40() -- libpk forwarder: clear the embedded object at +0x80 (its counter word zeroed by 0x00C61C50)
// Leaf: ?Method_00c44b40@CObject_00c44b40@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C44B40, 11 bytes: `add ecx,80h ; jmp 0x00C61C50`.
// Identity: libpk (no PDB, no RTTI). The spelling is the one the landed caller binds to this address (CClass_00c36240::sub_00c36240
//   0x00C36240 calls it as ?Method_00c44b40@CObject_00c44b40@@QAEXXZ, rebuild/oracles/reloc_bindings.tsv); one retail address, one
//   name. The target 0x00C61C50 is landed as C00c61c50::M (`f(+8) = 0`). Both are address placeholders: no new claim (the catalog
//   "_Cons_val<..>" was propagated; module CAmbientEmitter is a manifest guess).
// Callees: ?M@C00c61c50@@QAEXXZ 0x00C61C50 (the landed spelling of that body).
// Header: none exists for these libpk placeholder classes (the landed rows model them locally); only the member this row touches.
struct C00c61c50 {
    void M();                                             // 0x00C61C50
private:
    unsigned char pad[0xc];
};
class CObject_00c44b40 {
public:
    void Method_00c44b40();
private:
    unsigned char Unmodeled_0x00[0x80];                   // +0x00
    C00c61c50 Member_0x80;                                // +0x80 (name unknown)
};
void CObject_00c44b40::Method_00c44b40()
{
    Member_0x80.M();
}