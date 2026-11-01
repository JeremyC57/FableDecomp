// CInputProcessManager::~CInputProcessManager() -- this class's vtable (0x0129CA38), every input process removed (0x00A0D530), then the CBase destructor (0x0099A300, a tail jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0D810, length 22 bytes.
// Leaf: ??1CInputProcessManager@@UAE@XZ
// The base label "CBase::CBase" is propagated onto the destructor. Shape: `this` in esi, `jmp` for the base.
class CBaseClassNonCopyable { public:   // CBaseClassNonCopyable (Ego_r.pdb ??1CBaseClassNonCopyable@@MAE@XZ; was stand-in CBase_0099a300, naming wave 15)
protected:
    virtual ~CBaseClassNonCopyable();   // slot 0 (retail scalar deleting dtor; was placeholder v00); 0x0099A300 (a non-virtual destructor keeps the deleting destructor out of the TU)
public:
};
class CInputProcessManager : public CBaseClassNonCopyable { public:    // vtable 0x0129CA38
    virtual void v00();
    void RemoveAllInputProcesses();                // 0x00A0D530
    ~CInputProcessManager();
};

CInputProcessManager::~CInputProcessManager()
{
    RemoveAllInputProcesses();
}