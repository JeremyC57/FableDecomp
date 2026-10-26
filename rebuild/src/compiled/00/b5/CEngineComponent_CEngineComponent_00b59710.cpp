// CEngineComponent::CEngineComponent() -- this class's vtable (0x012A2544), the dword at +4 cleared, the byte +8 set and +9 cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B59710, length 32 bytes.
// Vtable 0x012A2544 names the class; the chunk label is propagated. Shape: no frame, one zero register, `ret`.
class CEngineComponent { public:                          // vtable 0x012A2544
    virtual void v00();
    long GlobalPointerAddress;                                      // +0x04
    bool m_enabled;                                // +0x08
    bool KeepAlive;                                      // +0x09
    CEngineComponent();
};

CEngineComponent::CEngineComponent()
    : GlobalPointerAddress(0), m_enabled(true), KeepAlive(false)
{
}