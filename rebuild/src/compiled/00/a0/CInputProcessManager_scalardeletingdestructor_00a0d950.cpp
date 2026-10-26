// CInputProcessManager::`scalar deleting destructor'(unsigned int) -- the compiler-generated deleting destructor: the inline user destructor (own vtable store, then RemoveAllInputProcesses()) and the out-of-line ~CBaseClassNonCopyable, then operator delete when bit 0 of the flags is set; returns this
// Leaf: ??_GCInputProcessManager@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0D950, 43 bytes (+ int3 padding), thiscall, `ret 4`; reached only through vtable
//   0x0129CA38 slot 0.
// Identity (proven): CInputProcessManager; evidence: vtable 0x0129CA38 slot 0, RTTI .?AVCInputProcessManager@@ : CBaseClassNonCopyable : CBaseClass; 0x00A0D530 is the landed CInputProcessManager::RemoveAllInputProcesses; 0x0099A300 is ~CBaseClassNonCopyable (the destructor CTCBase's 0x00686830 also chains to; it leaves the CBaseClass vtable, the non-copyable vtable store being dead); FableWin.pdb names ??1CInputProcessManager@@UAE@XZ.
// Label: the manifest's "`vector_deleting_destructor'" is propagated across family H025 (seven bodies with the same shape: vtable store, one call,
//   the base destructor, the flag test; retail has no identical-code folding, so each address is one class).
// Callees: CInputProcessManager::RemoveAllInputProcesses() 0x00A0D530, CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300, operator delete 0x00BFE9BC.
// Shape: VC7.1 resets the vptr in a user-written destructor, so the store is the class's own vtable; the destructor body is
//   one call (`this` is already in ecx), the base destructor follows. The deleting destructor is emitted with the vtable, in
//   a TU that constructs the class: MakeOne is that construction, nothing more.
class CBaseClassNonCopyable {                           // RTTI: CBaseClassNonCopyable : CBaseClass
protected:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();                   // 0x0099A300 (PDB ??1CBaseClassNonCopyable@@MAE@XZ: protected)
};
class CInputProcessManager : public CBaseClassNonCopyable {
public:
    CInputProcessManager() { }
    virtual ~CInputProcessManager() { RemoveAllInputProcesses(); }
    void RemoveAllInputProcesses();                     // 0x00A0D530
};
void* MakeOne_00a0d950()
{
    return new CInputProcessManager;
}