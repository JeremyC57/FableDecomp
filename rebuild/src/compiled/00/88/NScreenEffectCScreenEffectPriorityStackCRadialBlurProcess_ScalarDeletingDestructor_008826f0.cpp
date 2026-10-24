// NScreenEffect::CScreenEffectPriorityStack::CRadialBlurProcess::ScalarDeletingDestructor(unsigned int flags) -- the `scalar deleting destructor': clear the stack back-pointer, restore the interface vtable, free when bit 0 is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008826F0, 38 bytes (row padded to 48). Virtual slot 0 of
// CRadialBlurProcess's vtable 0x0127803C; FableWin.pdb lists ??_GCRadialBlurProcess@CScreenEffectPriorityStack@NScreenEffect@@UAEPAXI@Z.
// Callees: operator delete @ 0x00BFE9BC. The only vtable stored is ICustomProcessInterface's 0x01278000 (landed
// ICustomProcessInterface deleting destructor 0x00880920).
// Layout: {vptr, TheStack +4} (PDB CRadialBlurProcess, 8 bytes).
// Shape: `TheStack = 0` before the base vtable store and NO store of the class's own vtable: the destructor body clears the
// pointer and VC7.1 drops the dead derived-vtable store in front of the inlined base destructor.
// Leaf: ??_GCRadialBlurProcess
namespace NScreenEffect {
class CScreenEffectPriorityStack {
public:
    struct CStackEntry;
    class ICustomProcessInterface {
    public:
        virtual ~ICustomProcessInterface() {}
        virtual void ResetToDefault() = 0;
        virtual void OnErase(CStackEntry* entry) = 0;
        virtual void Process(float weight, const CStackEntry* entry) = 0;
    };
    class CRadialBlurProcess : public ICustomProcessInterface {
    public:
        CRadialBlurProcess(CScreenEffectPriorityStack* stack) : TheStack(stack) {}
        virtual ~CRadialBlurProcess() { TheStack = 0; }
        virtual void ResetToDefault();
        virtual void OnErase(CStackEntry* entry);
        virtual void Process(float weight, const CStackEntry* entry);
        CScreenEffectPriorityStack* TheStack;      // +0x04
    };
    ICustomProcessInterface* MakeRadialBlurProcess();
};

// The vtable (and the deleting destructor it references) is emitted by the TU that constructs the object.
CScreenEffectPriorityStack::ICustomProcessInterface* CScreenEffectPriorityStack::MakeRadialBlurProcess()
{
    return new CRadialBlurProcess(this);
}
}