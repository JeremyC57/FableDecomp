// PKCProtectionInstance::~PKCProtectionInstance() -- scoped lock guard teardown: when the section was held longer than the global maximum, trace ".\PKCProtectionInstance.cpp(19) : Event time (N ms) over maximum (M ms)" to the assert hook and break into the debugger; then leave the section
// Grade: DIFFER(256v253) -- only the elapsed-time prologue: retail `call [GetTickCount] ; sub eax,[edi+4] ; mov esi,eax ; cmp esi,[0x0139C904]` (both memory operands folded), ours loads m_tick into ecx and the maximum into eax first (`mov ecx,[edi+4] ; mov esi,eax ; mov eax,[max] ; sub esi,ecx ; cmp esi,eax`); the rest (stream chain, hook, CDebugBreak, Leave, EH state) matches. Tried: one expression, a split `elapsed -= m_tick`, an inline GetElapsed() member, /Oa -- all identical
// Leaf: ??1PKCProtectionInstance@@QAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0FFE0, 253 bytes, thiscall, plain `ret`; EH-framed (the trace string, state 0).
// Identity (probable): the destructor of the landed scoped guard PKCProtectionInstance (constructor 0x00C0FFC0 stores the
//   section and GetTickCount at +4; this body subtracts that tick from GetTickCount); the class name is the guard's own
//   trace file string ".\PKCProtectionInstance.cpp" (libpk names its classes after their files; no PDB covers libpk).
//   Already referenced under this symbol by the landed lock users (e.g. CLockedRingBuffer::Pop 0x00C4A0E0). The
//   catalog's "EventQueue_ProcessWithTimeout" is descriptive.
// Callees: GetTickCount (IAT 0x0143FDFC), CString operator<< 0x00C0DEC0 (text) / 0x00C0DF00 (long) / 0x00C0DF10
//   (unsigned long), ~CString 0x00C0BA40, the assert hook pointer 0x0143BB2C, CDebugBreak 0x00C16750 (an empty temporary),
//   PKCCriticalSection::Leave 0x00C1C840. Globals: _afxPchNil 0x0139C900 (the empty string), the maximum hold time
//   0x0139C904 (unsigned compare).
extern "C" __declspec(dllimport) unsigned long __stdcall GetTickCount();   // KERNEL32 import 0x0143FDFC
extern char* _afxPchNil;                        // 0x0139C900
extern unsigned long g_maxProtectionTime;       // 0x0139C904
typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;           // 0x0143BB2C

class CString {                                 // the libpk MFC-style string: one word
public:
    char* m_pchData;
    CString() : m_pchData(_afxPchNil) { }
    ~CString();                                 // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);       // 0x00C0DEC0
    CString& __fastcall operator<<(long value);             // 0x00C0DF00
    CString& __fastcall operator<<(unsigned long value);    // 0x00C0DF10
};

struct CDebugBreak {                            // empty temporary whose constructor breaks into the debugger
    CDebugBreak();                              // 0x00C16750
};

class PKCCriticalSection {                      // a Win32 CRITICAL_SECTION (0x18 bytes)
public:
    unsigned char m_section[0x18];
    PKCCriticalSection();                       // 0x00C1C810
    ~PKCCriticalSection();                      // 0x00C1C820
    void Enter();                               // 0x00C1C830
    void Leave();                               // 0x00C1C840
};

class PKCProtectionInstance {                   // 8 bytes: the guarded section and the entry tick
public:
    PKCCriticalSection* m_pSection;             // +0
    unsigned long m_tick;                       // +4
    PKCProtectionInstance(PKCCriticalSection* section);   // 0x00C0FFC0
    ~PKCProtectionInstance();
};

PKCProtectionInstance::~PKCProtectionInstance()
{
    unsigned long elapsed = GetTickCount() - m_tick;
    if (elapsed > g_maxProtectionTime) {
        {
            CString line;
            line << ".\\PKCProtectionInstance.cpp" << "(" << (long)19 << ") : " << "Event time (" << elapsed
                 << " ms) over maximum (" << g_maxProtectionTime << " ms)" << "\n";
            if (g_pfnAssertHook)
                g_pfnAssertHook(line.m_pchData);
        }
        CDebugBreak();
    }
    m_pSection->Leave();
}
