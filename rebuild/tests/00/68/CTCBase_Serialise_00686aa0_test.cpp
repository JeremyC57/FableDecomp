// Behaviour test for 0x00686AA0 CTCBase::Serialise: in modes 1 and 3 the transfer hook runs only when the
// component wants to be saved; in every other mode it always runs, with both arguments forwarded.
#include <cstdio>

struct CPersistContext { long Mode; };
struct CTCBase_Calls {
    virtual void Transfer(CPersistContext* context, long arg) = 0;
    virtual bool ShouldBeSaved() = 0;
    void Serialise(CPersistContext* context, long arg)
    {
        long mode = context->Mode;
        if ((mode == 1 || mode == 3) && !ShouldBeSaved())
            return;
        Transfer(context, arg);
    }
};
struct CProbe : CTCBase_Calls {
    bool m_save; int m_asked; int m_transfers; CPersistContext* m_ctx; long m_arg;
    void Transfer(CPersistContext* context, long arg) { ++m_transfers; m_ctx = context; m_arg = arg; }
    bool ShouldBeSaved() { ++m_asked; return m_save; }
};

int main()
{
    for (long mode = 0; mode < 5; ++mode) {
        for (int save = 0; save < 2; ++save) {
            CProbe probe;
            probe.m_save = save != 0; probe.m_asked = 0; probe.m_transfers = 0; probe.m_ctx = 0; probe.m_arg = 0;
            CPersistContext context; context.Mode = mode;
            probe.Serialise(&context, 42);
            bool saveMode = mode == 1 || mode == 3;
            int wantAsked = saveMode ? 1 : 0;
            int wantTransfers = (!saveMode || save) ? 1 : 0;
            if (probe.m_asked != wantAsked || probe.m_transfers != wantTransfers) { std::printf("00686aa0_TEST FAIL\n"); return 1; }
            if (wantTransfers && (probe.m_ctx != &context || probe.m_arg != 42)) { std::printf("00686aa0_TEST FAIL\n"); return 1; }
        }
    }
    std::printf("00686aa0_TEST PASS\n");
    return 0;
}