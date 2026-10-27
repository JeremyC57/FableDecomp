// CInitBaseClass::Initialise() -- mark the object valid and report success
// Leaf: ?Initialise@CInitBaseClass@@IAE_NXZ
// Retail: Fable.exe @ 0x0099A350, 6 bytes, thiscall plain `ret`, bool in al.
// Identity (proven): Ego_r.pdb CInitBaseClass = { vptr, bool Valid +4 } with ?Initialise@CInitBaseClass@@IAE_NXZ (protected),
// ?Uninitialise@CInitBaseClass@@IAEXXZ, ?IsValid@CInitBaseClass@@QBE_NXZ; retail has the run back to back: IsValid
// 0x0099A330 (`mov al,[ecx+4]`), Validate 0x0099A340 (`ret`), this body (Valid = true), Uninitialise 0x0099A360 (clears
// Valid). Its nine callers (e.g. 0x00413120, 0x0048A210) call it first thing in derived Initialise bodies and test al.
// reloc_bindings already has ?Initialise@CInitBaseClass@@QAE_NXZ (public; the PDB says protected, IAE).
// Label correction: catalog "CXMVPlaybackInfo" (module CXMVPlaybackInfo).
// Callees: none.
// Layout: no engine header for CInitBaseClass: local model (vptr, Valid +4).
// Shape: `mov al,1 ; mov [ecx+4],al` -- the stored constant is reused as the result.
class CInitBaseClass {
public:
    virtual ~CInitBaseClass();
    bool IsValid() const;
protected:
    bool Initialise();
    void Uninitialise();

    bool Valid;                                             // +0x04
};

bool CInitBaseClass::Initialise()
{
    Valid = true;
    return true;
}