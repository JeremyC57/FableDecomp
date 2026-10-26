// CTCFineDialogue::`scalar deleting destructor'(unsigned int) -- the compiler-generated deleting destructor: the inline user destructor (own vtable store, then RemoveFromGlobalTCs()) and the out-of-line ~CTCBase, then operator delete when bit 0 of the flags is set; returns this
// Leaf: ??_GCTCFineDialogue@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E2D70, 43 bytes (+ int3 padding), thiscall, `ret 4`; reached only through vtable
//   0x01270274 slot 0.
// Identity (proven): CTCFineDialogue; evidence: vtable 0x01270274 slot 0, RTTI .?AVCTCFineDialogue@@ : CTCBase : CBaseClassNonCopyable : CBaseClass; 0x00686830 stores the CTCBase vtable 0x0125BE0C and tail-jumps to ~CBaseClassNonCopyable 0x0099A300, so it is ~CTCBase; 0x00686DF0 is CTCBase::RemoveFromGlobalTCs (PDB); FableWin.pdb names ??1CTCFineDialogue@@UAE@XZ.
// Label: the manifest's "`vector_deleting_destructor'" is propagated across family H025 (seven bodies with the same shape: vtable store, one call,
//   the base destructor, the flag test; retail has no identical-code folding, so each address is one class).
// Callees: CTCBase::RemoveFromGlobalTCs() 0x00686DF0, CTCBase::~CTCBase 0x00686830, operator delete 0x00BFE9BC.
// Shape: VC7.1 resets the vptr in a user-written destructor, so the store is the class's own vtable; the destructor body is
//   one call (`this` is already in ecx), the base destructor follows. The deleting destructor is emitted with the vtable, in
//   a TU that constructs the class: MakeOne is that construction, nothing more.
class CTCBase {                                         // RTTI: CTCBase : CBaseClassNonCopyable : CBaseClass
public:
    CTCBase();
    virtual ~CTCBase();                                 // 0x00686830 (stores the CTCBase vtable 0x0125BE0C, then ~CBaseClassNonCopyable)
    void RemoveFromGlobalTCs();                         // 0x00686DF0
};
class CTCFineDialogue : public CTCBase {
public:
    CTCFineDialogue() { }
    virtual ~CTCFineDialogue() { RemoveFromGlobalTCs(); }
};
void* MakeOne_007e2d70()
{
    return new CTCFineDialogue;
}