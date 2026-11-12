// Behaviour test for 0x00CAF480 CDeferredCommand::~CDeferredCommand (agentFAM08 portable twin; the catalog test spelled
// __stdcall / __declspec and 64-bit g++ cannot build it). Model: the empty destructor body; the members go in reverse declaration
// order -- the COM pointer (+0x48) releases its interface when non-null, then m_DispParams (+0x34) -- then the CUnknown base with its
// object counter (CMyBaseObject). Test-only model types.
#include <cstdio>
static int g_seq[8];
static int g_count = 0;
static int g_releases = 0;
static void Record(int v) { if (g_count < 8) g_seq[g_count++] = v; }
struct ModelInterface { virtual unsigned long Release() { ++g_releases; Record(3); return 0; } virtual ~ModelInterface() { } };
struct ModelObjectCounter { ~ModelObjectCounter() { Record(1); } };
struct ModelUnknown { ModelObjectCounter counter; long refs; virtual ~ModelUnknown() { } };
struct ModelDispParams { ~ModelDispParams() { Record(2); } };
struct ModelComPointer { ModelInterface* p; ~ModelComPointer() { if (p) p->Release(); } };
struct ModelDeferredCommand : ModelUnknown {
    ModelDispParams dispParams;
    ModelComPointer object;
    ~ModelDeferredCommand() { }
};
int main()
{
    ModelInterface target;
    {
        ModelDeferredCommand c;
        c.object.p = &target;
    }
    if (g_count != 3 || g_seq[0] != 3 || g_seq[1] != 2 || g_seq[2] != 1 || g_releases != 1) { std::printf("00caf480_TEST FAIL set\n"); return 1; }
    g_count = 0;
    {
        ModelDeferredCommand c;
        c.object.p = 0;                      // a null interface is not released
    }
    if (g_count != 2 || g_seq[0] != 2 || g_seq[1] != 1 || g_releases != 1) { std::printf("00caf480_TEST FAIL null\n"); return 1; }
    std::printf("00caf480_TEST PASS\n");
    return 0;
}
