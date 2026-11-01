// CAInputProcess::~CAInputProcess() -- leave the input-process manager (the inlined RemoveFromInterface: Cleanup, RemoveInputProcess, clear the pointer), then ~CBaseClassNonCopyable
// Leaf: ??1CAInputProcess@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0D3D0, 46 bytes, thiscall, tail `jmp` to the base destructor.
// Identity (proven): the body stores vtable 0x0129CA10 = .?AVCAInputProcess@@ (RTTI COL: CAInputProcess -> CBaseClassNonCopyable -> CBaseClass)
//   and chains to ~CBaseClassNonCopyable 0x0099A300; the 70 callers are the derived input-process destructors (0x00486330 ...) and the
//   class's own ??_G 0x00A0D410 expands the same body. Name: Ego_r.pdb ??1CAInputProcess@@UAE@XZ (the catalog label agrees).
// Callees: CAInputProcess virtual slot 3 Cleanup (0x00486370 in 0x0129CA10, a bare `ret`; read through the vtable constant,
//   `call [0x0129CA1C]`, as a virtual call from an inline helper inside a destructor compiles), CInputProcessManager::RemoveInputProcess
//   0x00A0D780 (Ego_r.pdb ?RemoveInputProcess@CInputProcessManager@@IAEXAAVCAInputProcess@@@Z: protected, by reference -- the landed
//   bindings use a public pointer form, NAME_FIXES.tsv), CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300 (protected virtual).
// Layout: engine/CAInputProcess.h: vptr, PPlayerInterface +0x04, InputProcessListPos (CLinkedListPosition: Data, Next, Previous) +0x08,
//   sizeof 0x14. The header flattens the vptr, so the class is a local polymorphic model sized from it.
// Virtual slots (vtable 0x0129CA10, Ego_r names): 0 dtor, 1 AddToInterface 0x00A0D2B0, 2 Init 0x00486360, 3 Cleanup 0x00486370.
// Shape: RemoveFromInterface (Ego_r ?RemoveFromInterface@CAInputProcess@@QAEXXZ, out of line at 0x00A0D2D0 with this exact body) is a plain
//   member defined above the destructor; VC7.1 expands it in the destructor and binds its virtual Cleanup() to the vtable just stored.
#include "engine/CBaseClassNonCopyable.h"
#define CAInputProcess CAInputProcessFields
#include "engine/CAInputProcess.h"
#undef CAInputProcess
class CAInputProcess;
class CInputProcessManager {
    friend class CAInputProcess;
protected:
    void RemoveInputProcess(CAInputProcess& process);      // 0x00A0D780
};
class CLinkedListPosition {
public:
    CBaseClass* Data;
    CLinkedListPosition* Next;
    CLinkedListPosition* Previous;
};
class CAInputProcess : public CBaseClassNonCopyable {
public:
    virtual ~CAInputProcess();
    virtual bool AddToInterface(CInputProcessManager& manager);   // slot 1  0x00A0D2B0
    virtual void Init();                                          // slot 2  0x00486360
    virtual void Cleanup();                                       // slot 3  0x00486370
    void RemoveFromInterface();                                   // 0x00A0D2D0
protected:
    CInputProcessManager* PPlayerInterface;                       // +0x04
    CLinkedListPosition InputProcessListPos;                      // +0x08
};
typedef char CAInputProcessExtent[sizeof(CAInputProcess) == sizeof(CAInputProcessFields) ? 1 : -1];

void CAInputProcess::RemoveFromInterface()
{
    if (PPlayerInterface) {
        Cleanup();
        PPlayerInterface->RemoveInputProcess(*this);
        PPlayerInterface = 0;
    }
}

CAInputProcess::~CAInputProcess()
{
    RemoveFromInterface();
}