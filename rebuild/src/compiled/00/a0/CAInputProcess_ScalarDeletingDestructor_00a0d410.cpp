// CAInputProcess::`scalar deleting destructor'(unsigned int) -- unregister from the manager, ~CBaseClass, optional delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0D410, length 65 bytes (+ int3 padding).
// Callees: this class's virtual slot 3 through the vtable constant (`call [0129CA1Ch]`: a virtual
// call on `this` from an inline helper, bound to the vtable the destructor just stored),
// CInputProcessManager::RemoveInputProcess(CAInputProcess*) @ 0x00A0D780, CBaseClass::~CBaseClass
// @ 0x0099A300, operator delete @ 0x00BFE9BC when bit 0 of the flag is set.
// Vtable 0x0129CA10 CAInputProcess (rtti-vtables.tsv); PPlayerInterface is +0x4 (engine header).
// Shape: the manager pointer is loaded before the vtable store (scheduling); when it is set the
// slot-3 notification runs, the manager drops this process (the pointer re-read after the virtual
// call) and the pointer is nulled; then the base destructor and the deleting tail. The vtable
// (and this `??_G`) is emitted because the TU constructs the object (MakeOne); the destructor is
// `__forceinline` so its body sits inside the `??_G` (VC7.1 would otherwise emit `??1` out of line).
struct CAInputProcess;

struct CInputProcessManager {
    void RemoveInputProcess(CAInputProcess* process);    // 0x00A0D780
};

struct CBaseClassNonCopyable {
    virtual ~CBaseClassNonCopyable();           // slot 0; 0x0099A300 (called directly from the derived dtor)
};

struct CAInputProcess : CBaseClassNonCopyable {
    CInputProcessManager* PPlayerInterface;     // +0x04
    void* InputProcessListPos[3];               // +0x08 CLinkedListPosition
    __forceinline virtual ~CAInputProcess()
    {
        if (PPlayerInterface != 0) {
            NotifyRemoved();
            PPlayerInterface->RemoveInputProcess(this);
            PPlayerInterface = 0;
        }
    }
    virtual void v01();
    virtual void v02();
    virtual void OnRemovedFromManager();        // slot 3 (+0xc)
    // A virtual call written directly in the destructor body is devirtualised by the front end
    // (a plain `call`); inside an inline helper the optimizer forwards the vtable just stored and
    // reads the slot from the constant: `call dword ptr [vtable+0Ch]`, retail's form.
    void NotifyRemoved() { OnRemovedFromManager(); }
};

// VC7.1 emits a class's vtable (and the deleting destructor it references) in every translation
// unit that constructs the object; this helper is that construction, nothing more.
void* MakeOne()
{
    return new CAInputProcess;
}