// CVtable_012b4148::CVtable_012b4148(void* context) -- constructor: own vtable, a CSemaphore member at +4 built with (0, 1), the argument stored at +8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2A230, length 48 bytes.
// Callees: CSemaphore::CSemaphore(long, long) @ 0x00C36120 (the "CActionParam_InitSemaphore" label).
// The manifest calls this CActionParam::CActionParam, a propagated label: the vtable stored,
// 0x012B4148, has no RTTI locator (the quest-script module), so the class is named CVtable_012b4148
// as the family landings do. Shape: the vtable store sinks to just before the member constructor
// call (ecx = &m_semaphore, 1 and 0 pushed right to left), then the argument is stored at +8 in
// init-list order.
struct CSemaphore {
    void* m_handle;
    CSemaphore(long initialCount, long maximumCount);            // 0x00C36120
};

struct CVtable_012b4148 {                                        // vtable 0x012B4148
    virtual void v00();
    CSemaphore m_semaphore;                                      // +0x04
    void*      m_pContext;                                       // +0x08
    CVtable_012b4148(void* context);
};

CVtable_012b4148::CVtable_012b4148(void* context)
    : m_semaphore(0, 1)
    , m_pContext(context)
{
}
