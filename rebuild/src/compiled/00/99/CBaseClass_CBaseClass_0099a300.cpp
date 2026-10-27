// CBaseClass::~CBaseClass() -- the root class destructor: only resets the vtable pointer to CBaseClass's own
// Leaf: ??1CBaseClass@@UAE@XZ
// Retail: Fable.exe @ 0x0099A300, 7 bytes, thiscall plain `ret`.
// Identity (proven): the body stores 0x01231710 = the CBaseClass vtable (RTTI .?AVCBaseClass@@, rtti-vtables.tsv) and does
// nothing else; a destructor's identity is the vtable it stores. Ego_r.pdb / FableWin.pdb have ??1CBaseClass@@UAE@XZ (a
// virtual destructor) and CBaseClass is the root of every RTTI chain; the 454 call/jmp sites are derived destructors
// chaining to it (CBaseClass's ??_G 0x0099A470 among them). Its neighbours are CBaseClass(void) 0x0099A2D0 and
// CBaseClassNonCopyable() 0x0099A2F0.
// Label correction: catalog "CBase" (module CBase); reloc_bindings holds only stand-ins for this address
// (@base_dtor_99a300@4, ?baseDtor@BaseInput@@QAEXXZ, N_0099a300, ...).
// Callees: none. Layout: CBaseClass is the bare vptr (Ego_r.pdb size 4).
// Shape: a user-declared (even empty) destructor stores its own vtable; the compiler-generated one would not.
class CBaseClass {
public:
    CBaseClass();
    virtual ~CBaseClass();
};

CBaseClass::~CBaseClass()
{
}