// CCodaFrameStreamer_00c49650::~CCodaFrameStreamer() -- EH-framed destructor (vtable 0x012B8138; the manifest's "CCodaFrameStreamer_dtor"): under the playback section at 0x0143BAFC (@ 0x00C0FFC0 / 0x00C0FFE0, no state of its own since the CODA helpers cannot throw) a playing +4 source (`__stdcall` 0x00C35CA0) is stopped (0x00C35C30 with two zeros), released (0x00C35D50) and destroyed (0x00C35980) and the pointer cleared; the +0x3c frames are then popped from the +0x1c queue (@ 0x00C49FF0), a null frame raising "Null frame" (CCodaFrameStreamer.cpp line 84: an MFC-style string in a nested block handed to the log sink at 0x0143BB2C, then the empty trace temporary @ 0x00C16750 on a free frame byte; state 4) and a real one deleted (destructor @ 0x00C491B0 then `operator delete`); the compiler then destroys the +0x48 part (@ 0x00C49D00; state 2), the queue's +0x1c event (@ 0x00C29FB0; the queue address homed for the root unwind @ 0x00C1C820, state 5) and root (state 1), the +8 part (@ 0x00C49AF0; state 0) and stores the base vtable 0x012B80AC (inline empty base destructor, 0x00C493E0 on unwind)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C49650, oracle length 385 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,10h ; push ebp ; push esi ; mov esi,ecx ; push edi ; mov [esp+18h],esi ; mov dword ptr [esi],12B8138h ; push offset g_playbackSection ; lea ecx,[esp+14h] ; mov dword ptr [esp+28h],3 ; call CProfileLock::CProfileLock ; mov eax,[esi+4] ; push eax ; call IsPlaying ; test eax,eax ; je a ; mov ecx,[esi+4] ; push 0 ; push 0 ; push ecx ; call Stop ; a: mov edx,[esi+4] ; push edx ; call Release ; mov eax,[esi+4] ; push eax ; call Destroy ; xor ebp,ebp ; lea ecx,[esp+10h] ; mov [esi+4],ebp ; call ~CProfileLock ; cmp [esi+3Ch],ebp ; jbe tail ; loop: lea ecx,[esi+1Ch] ; call CFrameQueue::Pop ; mov edi,eax ; test edi,edi ; jne del ; mov ecx,[g_pchNil] ; mov [esp+10h],ecx ; mov edx,offset ".\CCodaFrameStreamer.cpp" ; lea ecx,[esp+10h] ; mov byte ptr [esp+24h],4 ; call Append ; ... mov edx,54h ; ... call AppendNumber ; ... call Append ("Null frame") ; ... call Append ("\n") ; mov eax,[g_pfnLogSink] ; test eax,eax ; je skip ; mov ecx,[esp+10h] ; call eax ; skip: lea ecx,[esp+10h] ; mov byte ptr [esp+24h],3 ; call ~CString ; lea ecx,[esp+0Fh] ; call CTrace::CTrace ; jmp next ; del: mov ecx,edi ; call CFrame::~CFrame ; push edi ; call operator delete ; add esp,4 ; next: mov eax,[esi+3Ch] ; inc ebp ; cmp ebp,eax ; jb loop ; tail: lea ecx,[esi+48h] ; mov byte ptr [esp+24h],2 ; call CPart48::~CPart48 ; lea edi,[esi+1Ch] ; mov [esp+10h],edi ; lea ecx,[edi+1Ch] ; mov byte ptr [esp+24h],5 ; call CEvent::~CEvent ; mov ecx,edi ; mov byte ptr [esp+24h],1 ; call CHandleTable::~CHandleTable ; lea ecx,[esi+8] ; mov byte ptr [esp+24h],0 ; call CPart8::~CPart8 ; mov ecx,[esp+1Ch] ; pop edi ; mov dword ptr [esi],12B80ACh ; ... ret
// The classes are named by address.
class PKCCriticalSection;
void __cdecl operator delete(void* block);
extern PKCCriticalSection g_soundLock;   // 0x0143BAFC
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c49650)(const char* text);
extern PFNLOGSINK_00c49650 g_pfnLogSink_0143bb2c;   // 0x0143BB2C
extern "C" long __stdcall CODA_IsSourcePlaying(void* source);               // 0x00C35CA0 (non-throwing)
extern "C" void __stdcall StopSource_00c49650(void* source, long a, long b);   // 0x00C35C30
extern "C" void __stdcall ReleaseSource_00c49650(void* source);             // 0x00C35D50
extern "C" void __stdcall DestroySource_00c49650(void* source);             // 0x00C35980

struct CString_00c49650 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c49650() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c49650();                       // 0x00C0BA40
    CString_00c49650* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c49650* __fastcall AppendNumber(long value);   // 0x00C0DF00
};
struct CTrace_00c49650 {                   // empty temporary (a debug-break helper): lands on a free frame byte
    CTrace_00c49650();                         // 0x00C16750
};
class PKCProtectionInstance { public:             // 8 bytes: section + entry tick
    void* m_pSection;
    unsigned long m_tick;
    PKCProtectionInstance(PKCCriticalSection* section);  // 0x00C0FFC0
    ~PKCProtectionInstance();              // 0x00C0FFE0
};

struct CFrame_00c49650 { ~CFrame_00c49650(); };   // 0x00C491B0
struct CPart8_00c49650 { unsigned char _body[0x14]; ~CPart8_00c49650(); };    // +8, 0x00C49AF0
struct CHandleTable_00c49650 { unsigned char _body[0x1c]; ~CHandleTable_00c49650(); };   // 0x00C1C820
struct CEvent_00c49650 { void* m_hEvent; ~CEvent_00c49650(); };   // 0x00C29FB0
struct CFrameQueue_00c49650 {              // +0x1c: a handle table root with its event
    CHandleTable_00c49650 m_root;
    CEvent_00c49650 m_event;               // +0x1c
    CFrame_00c49650* Pop();                // 0x00C49FF0
    ~CFrameQueue_00c49650() { }
};
struct CPart48_00c49650 { unsigned char _body[4]; ~CPart48_00c49650(); };   // +0x48, 0x00C49D00

struct CStreamerBase_00c49650 {            // vtable 0x012B80AC
    virtual ~CStreamerBase_00c49650() { }
};

struct CCodaFrameStreamer_00c49650 : CStreamerBase_00c49650 {
    void* m_pSource;                       // +4
    CPart8_00c49650 m_8;                   // +8
    CFrameQueue_00c49650 m_queue;          // +0x1c
    unsigned long m_frames;                // +0x3c
    long m_40;
    long m_44;
    CPart48_00c49650 m_48;                 // +0x48
    virtual ~CCodaFrameStreamer_00c49650();
};

CCodaFrameStreamer_00c49650::~CCodaFrameStreamer_00c49650()
{
    {
        PKCProtectionInstance lock(&g_soundLock);
        if (CODA_IsSourcePlaying(m_pSource))
            StopSource_00c49650(m_pSource, 0, 0);
        ReleaseSource_00c49650(m_pSource);
        DestroySource_00c49650(m_pSource);
        m_pSource = 0;
    }
    for (unsigned long i = 0; i < m_frames; ++i) {
        CFrame_00c49650* frame = m_queue.Pop();
        if (!frame) {
            {
                CString_00c49650 line;
                line.Append(".\\CCodaFrameStreamer.cpp")->Append("(")->AppendNumber(84)->Append(") : ")->Append("Null frame")->Append("\n");
                if (g_pfnLogSink_0143bb2c)
                    g_pfnLogSink_0143bb2c(line.m_pchData);
            }
            CTrace_00c49650();
        } else {
            delete frame;
        }
    }
}