// CLoadingMemoryFailureHandler::CLoadingMemoryFailureHandler() -- this class's vtable (0x0129B8C4), then the embedded state at +4 constructed inline: its three dwords cleared and its critical section at +0x10 initialised (InitializeCriticalSection @ IAT 0x0143FEE4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D5EB0, length 48 bytes.
// Shape: the vtable stored before the zero stores (so they belong to a member with an inline constructor), one zero register, `ret`.
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(void* section);   // IAT 0x0143FEE4

struct SFailureState_009d5eb0 {                    // the embedded state: three counters and the lock guarding them
    long          m_count;                         // +0x00
    long          m_limit;                         // +0x04
    long          m_pending;                       // +0x08
    unsigned char m_section[24];                   // +0x0c: a CRITICAL_SECTION
    SFailureState_009d5eb0() : m_count(0), m_limit(0), m_pending(0) { InitializeCriticalSection(m_section); }
};

class CLoadingMemoryFailureHandler { public:               // vtable 0x0129B8C4
    virtual void v00();
    SFailureState_009d5eb0 m_state;                // +0x04
    CLoadingMemoryFailureHandler();
};

CLoadingMemoryFailureHandler::CLoadingMemoryFailureHandler()
    : m_state()
{
}