// CTimerHost::CTimerHost(float rate, long arg, PKCTimerTarget* target, const SThreadParams_00c355e0* params) -- build the timer part at +0 (rate, arg, target; vtable 0x012B3FA0), clear the worker pointer at +0x14, create the worker thread over this object with the caller's thread parameters, and raise the "EMEM" warning (PKCTimerManager.cpp line 10) when the allocation fails
// Leaf: ??0CTimerHost@@QAE@MJPAUPKCTimerTarget@@PBUSThreadParams_00c355e0@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C25B10, 268 bytes (+ int3 padding), thiscall `ret 10h`, EH-framed (state 0: the
//   allocation freed if the worker constructor throws; state 1: the warning string).
// Identity (descriptive; libpk has no PDB or RTTI): the 0x18-byte object the CSoundEventManager constructor 0x00C11D00
//   allocates at 0x00C11E97 and keeps at +0x7C4 (`new` 0x18, then this constructor with 50.0f, 0, the manager's +0x7BC
//   part as the timer target and the {0x3000, 15} thread parameters).
//   Its first 0x14 bytes are the timer part whose constructor 0x00C25A20 is the only store of vtable 0x012B3FA0 (slot 0
//   PKCTimerManager::ThreadMain 0x00C259E0: the loop over +4 shutdown flag, +8 target, +0xC float rate); this constructor
//   stores no vtable of its own, so that part is a MEMBER, not a base (the chunk 587 rule). The worker pointer +0x14 is
//   what 0x00C25C20 destroys after PKCTimerManager::InitiateShutdown 0x00C25A50; ~CSoundEventManager 0x00C13470 calls
//   0x00C25C20 and then operator delete on +0x7C4, so that row is ~CTimerHost (landed as CTimerHost_00c25c20::Close; the
//   class name is that landed row's without the address suffix; NAME_FIXES.tsv).
// Callees: PKCTimerManager::PKCTimerManager 0x00C25A20 (landed under a placeholder; respelled here, NAME_FIXES.tsv),
//   operator new 0x00BFEA1A, CWorkerThread_00c355e0::CWorkerThread_00c355e0 0x00C355E0 (the landed row's own spelling);
//   the libpk trace string of the sweep1 model (PKCDefaultFileManager 0x00C1A230): CString::operator<< 0x00C0DEC0 / 0x00C0DF00,
//   ~CString 0x00C0BA40, CDebugBreak 0x00C16750, _afxPchNil 0x0139C900, g_pfnAssertHook 0x0143BB2C.
// Shape: `m_pThread(0)` in the init list, then `m_pThread = new ...` (the zero store before the allocation); the warning
//   is the library's block (`{ CString line; ...; if (hook) hook(line); } CDebugBreak();`).
void* __cdecl operator new(unsigned int size);   // 0x00BFEA1A

extern char* _afxPchNil;                        // 0x0139C900
typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;           // 0x0143BB2C

class CString {                                 // the libpk MFC-style string: one word
public:
    char* m_pchData;
    CString() : m_pchData(_afxPchNil) { }
    ~CString();                                 // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CString& __fastcall operator<<(long value);         // 0x00C0DF00
};

struct CDebugBreak {                            // empty temporary whose constructor breaks into the debugger
    CDebugBreak();                              // 0x00C16750
};

struct SThreadParams_00c355e0 {                 // 8 bytes: stack size and priority
    unsigned long m_stackSize;
    long m_priority;
};

struct CWorkerThread_00c355e0 {                 // 12 bytes (landed constructor 0x00C355E0)
    void* m_pOwner;                             // +0
    void* m_hThread;                            // +4
    unsigned char m_event[4];                   // +8
    CWorkerThread_00c355e0(void* owner, const SThreadParams_00c355e0* params);   // 0x00C355E0
};

struct PKCTimerTarget {
    virtual void Tick();                        // slot 0
};

struct PKCTimerManager {                        // vtable 0x012B3FA0, 0x14 bytes (the landed ThreadMain model)
    virtual void ThreadMain();                  // slot 0: 0x00C259E0
    bool            m_bShutdown;                // +0x04
    PKCTimerTarget* m_pTarget;                  // +0x08
    float           m_rate;                     // +0x0c ticks per second
    long            m_10;                       // +0x10
    PKCTimerManager(float rate, long arg, PKCTimerTarget* target);   // 0x00C25A20
};

class CTimerHost {                              // 0x18 bytes
public:
    PKCTimerManager         m_timer;            // +0x00
    CWorkerThread_00c355e0* m_pThread;          // +0x14
    CTimerHost(float rate, long arg, PKCTimerTarget* target, const SThreadParams_00c355e0* params);
};

CTimerHost::CTimerHost(float rate, long arg, PKCTimerTarget* target, const SThreadParams_00c355e0* params)
    : m_timer(rate, arg, target), m_pThread(0)
{
    m_pThread = new CWorkerThread_00c355e0(this, params);
    if (!m_pThread) {
        {
            CString line;
            line << ".\\PKCTimerManager.cpp" << "(" << (long)10 << ") : " << "EMEM" << "\n";
            if (g_pfnAssertHook)
                g_pfnAssertHook(line.m_pchData);
        }
        CDebugBreak();
    }
}