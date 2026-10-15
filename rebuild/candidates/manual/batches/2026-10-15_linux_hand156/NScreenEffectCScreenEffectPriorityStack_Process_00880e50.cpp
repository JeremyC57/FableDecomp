// NScreenEffect::CScreenEffectPriorityStack::Process() -- run the radial blur, displacement and screen filter priority stacks, each through its own custom-process callback object
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00880E50, length 98 bytes (+14 int3 padding).
// Callees: the three Process<list<...> > member-template instantiations @ 0x00883530 (radial blur stacks, +0x04),
// 0x00883640 (displacement, +0x6C), 0x00883750 (screen filter, +0x94) -- manifest labels all three with the
// CScreenFilterEntry instantiation (propagated). Callback objects: CRadialBlurProcess (vtable 0x0127803C),
// CDisplacementProcess (0x01278014), CScreenFilterProcess (0x01278028), each {vptr, owner stack} constructed
// inline from `this` (PDB ??0...@QAE@PAV12@@Z), all deriving from ICustomProcessInterface.
// Shape: three named callback locals in declaration order (frame +4, +0xC, +0x14); vptr and owner store after the
// two pushes; their inline virtual destructors leave no code.
namespace NScreenEffect {

class CScreenEffectPriorityStack {
public:
    struct CStackEntry;
    struct CRadialBlurEntry;
    struct CDisplacementMonochromeEffectColourEntry;
    struct CScreenFilterEntry;

    template <class T> struct list {                 // three-element arrays of these are 12 bytes in retail
        void* _M_node;
    };

    class ICustomProcessInterface {
    public:
        virtual ~ICustomProcessInterface() {}
        virtual void ResetToDefault() = 0;
        virtual void Unused() = 0;
        virtual void Process(float weight, const CStackEntry* entry) = 0;
    };
    class CRadialBlurProcess : public ICustomProcessInterface {
    public:
        CRadialBlurProcess(CScreenEffectPriorityStack* owner) : Owner(owner) {}
        virtual ~CRadialBlurProcess() {}
        virtual void ResetToDefault();
        virtual void Unused();
        virtual void Process(float weight, const CStackEntry* entry);
        CScreenEffectPriorityStack* Owner;
    };
    class CDisplacementProcess : public ICustomProcessInterface {
    public:
        CDisplacementProcess(CScreenEffectPriorityStack* owner) : Owner(owner) {}
        virtual ~CDisplacementProcess() {}
        virtual void ResetToDefault();
        virtual void Unused();
        virtual void Process(float weight, const CStackEntry* entry);
        CScreenEffectPriorityStack* Owner;
    };
    class CScreenFilterProcess : public ICustomProcessInterface {
    public:
        CScreenFilterProcess(CScreenEffectPriorityStack* owner) : Owner(owner) {}
        virtual ~CScreenFilterProcess() {}
        virtual void ResetToDefault();
        virtual void Unused();
        virtual void Process(float weight, const CStackEntry* entry);
        CScreenEffectPriorityStack* Owner;
    };

    void Process();
    template <class T> void Process(T* stacks, ICustomProcessInterface* process);   // 0x00883530 / 0x00883640 / 0x00883750

private:
    void*                                          __vftable;                   // +0x00
    list<CRadialBlurEntry>                         RadialParamStack[3];         // +0x04
    unsigned char                                  _radialKeysAndParams[0x6C - 0x10];
    list<CDisplacementMonochromeEffectColourEntry> DisplacementParamStack[3];   // +0x6C
    unsigned char                                  _displacementKeysAndParam[0x94 - 0x78];
    list<CScreenFilterEntry>                       ScreenFilterParamStack[3];   // +0x94
};

void CScreenEffectPriorityStack::Process()
{
    CRadialBlurProcess radialBlurProcess(this);
    Process(RadialParamStack, &radialBlurProcess);
    CDisplacementProcess displacementProcess(this);
    Process(DisplacementParamStack, &displacementProcess);
    CScreenFilterProcess screenFilterProcess(this);
    Process(ScreenFilterParamStack, &screenFilterProcess);
}

} // namespace NScreenEffect
