// CStreamProgressMonitor::UpdateProgress(void*, bool, void*, long) -- forwards a progress step to the embedded handler and accumulates the stall count, failing once it exceeds twice the limit
// Leaf: UpdateProgress
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6AF50, 112 bytes; default /O2 TU, no frame, `ret 10h`.
// Label correction: the catalog row "CTCHeroStats::UpdateEventProgress" is propagated; the body has nothing to do
// with CTCHeroStats (this is a library object of the 0xC3xxxx-0xC6xxxx block with a polymorphic member at +0x28C
// and two counters at +0x524 / +0x528; its only caller is 0x00C54759). Class and member names are descriptive.
// Callee: the embedded handler's vtable slot 2 (+0x08) with the four arguments.
// Shape: the handler's verdict stays in al: when it accepted the step and `quiet` is false, the step size is added
// to the running count and a count above `2 * limit` returns false; otherwise the count resets; in every other
// case the handler's verdict is returned.
struct CProgressHandler {
    virtual void v00();
    virtual void v01();
    virtual bool Step(void* source, bool quiet, void* context, long amount);  // slot 2 (+0x08)
};

struct CStreamProgressMonitor {
    unsigned char    _pad_0x000[0x28C];
    CProgressHandler m_handler;                      // +0x28C
    unsigned char    _pad_0x290[0x294];
    long             m_limit;                        // +0x524
    long             m_count;                        // +0x528

    bool UpdateProgress(void* source, bool quiet, void* context, long amount);
};

bool CStreamProgressMonitor::UpdateProgress(void* source, bool quiet, void* context, long amount)
{
    bool accepted = m_handler.Step(source, quiet, context, amount);
    if (accepted && !quiet) {
        m_count += amount;
        if (m_count > m_limit * 2)
            return false;
    } else {
        m_count = 0;
    }
    return accepted;
}
