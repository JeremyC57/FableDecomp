// CAInputProcess::CAInputProcess() -- base, null manager, this vtable, empty list position
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0D290, length 31 bytes (+ int3 padding).
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable() @ 0x0099A2F0 (vtable 0x0129A7C4).
// Vtable 0x0129CA10 CAInputProcess (rtti-vtables.tsv). Members from engine/CAInputProcess.h:
// PPlayerInterface +0x4, InputProcessListPos (CLinkedListPosition: Data, Next, Previous) +0x8.
// Shape: the manager pointer is zeroed BEFORE the vtable store, so it belongs to an inlined
// intermediate base constructor (its own vtable store is dead and dropped); the three zero stores
// after the vtable are the list position member's inline constructor, all from one zeroed eax.
class CInputProcessManager;
class CBaseClass;

struct CBaseClassNonCopyable {
    virtual ~CBaseClassNonCopyable();
    CBaseClassNonCopyable();                    // 0x0099A2F0
};

struct CInputProcessOwner : CBaseClassNonCopyable {
    CInputProcessManager* PPlayerInterface;     // +0x04
    CInputProcessOwner() : CBaseClassNonCopyable(), PPlayerInterface(0) {}
};

class CLinkedListPosition { public:
    CBaseClass* Data;                           // +0x00
    CLinkedListPosition* Next;                  // +0x04
    CLinkedListPosition* Previous;              // +0x08
    CLinkedListPosition() : Data(0), Next(0), Previous(0) {}
};

class CAInputProcess : public CInputProcessOwner { public:
    CLinkedListPosition InputProcessListPos;    // +0x08
    virtual void v01();                         // own vtable 0x0129CA10
    CAInputProcess();
};

CAInputProcess::CAInputProcess()
    : CInputProcessOwner(), InputProcessListPos()
{
}