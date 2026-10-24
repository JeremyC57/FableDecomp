// NScreenEffect::CScreenEffectPriorityStack::CScreenFilterProcess::ScalarDeletingDestructor(unsigned int flags) -- the `scalar deleting destructor': clear the stack back-pointer, restore the interface vtable, free when bit 0 is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00882750, 38 bytes. Virtual slot 0 of
// CScreenFilterProcess's vtable 0x01278028; FableWin.pdb lists ??_GCScreenFilterProcess@CScreenEffectPriorityStack@NScreenEffect@@UAEPAXI@Z.
// Callees: operator delete @ 0x00BFE9BC. The only vtable stored is ICustomProcessInterface's 0x01278000 (landed
// ICustomProcessInterface deleting destructor 0x00880920).
// Layout: {vptr, TheStack +4} (PDB CScreenFilterProcess, 8 bytes).
// Shape: `TheStack = 0` before the base vtable store and NO store of the class's own vtable: the destructor body clears the
// pointer and VC7.1 drops the dead derived-vtable store in front of the inlined base destructor.
// Leaf: ??_GCScreenFilterProcess
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
    class CScreenFilterProcess : public ICustomProcessInterface {
    public:
        CScreenFilterProcess(CScreenEffectPriorityStack* stack) : TheStack(stack) {}
        virtual ~CScreenFilterProcess() { TheStack = 0; }
        virtual void ResetToDefault();
        virtual void OnErase(CStackEntry* entry);
        virtual void Process(float weight, const CStackEntry* entry);
        CScreenEffectPriorityStack* TheStack;      // +0x04
    };
    ICustomProcessInterface* MakeScreenFilterProcess();
};

// The vtable (and the deleting destructor it references) is emitted by the TU that constructs the object.
CScreenEffectPriorityStack::ICustomProcessInterface* CScreenEffectPriorityStack::MakeScreenFilterProcess()
{
    return new CScreenFilterProcess(this);
}
}