// CInputProcessManager::~CInputProcessManager() -- this class's vtable (0x0129CA38), every input process removed (0x00A0D530), then the CBase destructor (0x0099A300, a tail jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0D810, length 22 bytes.
// Leaf: ??1CInputProcessManager@@QAE@XZ
// The base label "CBase::CBase" is propagated onto the destructor. Shape: `this` in esi, `jmp` for the base.
struct CBase_0099a300 {
    virtual void v00();
    ~CBase_0099a300();                             // 0x0099A300 (a non-virtual destructor keeps the deleting destructor out of the TU)
};
class CInputProcessManager : public CBase_0099a300 { public:    // vtable 0x0129CA38
    virtual void v00();
    void RemoveAllInputProcesses();                // 0x00A0D530
    ~CInputProcessManager();
};

CInputProcessManager::~CInputProcessManager()
{
    RemoveAllInputProcesses();
}