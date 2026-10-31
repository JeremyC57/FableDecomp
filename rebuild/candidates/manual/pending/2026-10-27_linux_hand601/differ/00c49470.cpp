// CCodaFrameStreamer::SubmitFrame(CCodaFrame* frame) -- take back a frame handed out by the streamer (trace "Must have got the frame from here!?!", CCodaFrameStreamer.cpp line 20, when none is out) and either queue it for playback -- a frame with samples in the stream's channel layout becomes a CODA buffer (1, channels, rate, bytes, data) queued on the source under the sound lock, the frame goes on the in-flight ring and a stopped source is started -- or return it to the free-frame mailbox
// Grade: DIFFER(415v415) -- frame 0x28 v retail 0x2c: retail gives the CDebugBreak temporary its own lowest dword ([esp+0Bh], string at +0Ch, lock +10h, descriptor +18h); ours puts the break byte below the string (string +8, lock +0Ch, descriptor +14h), so every esp offset is 4 lower, and the descriptor stores are scheduled channels/flags/bytes where retail does channels/rate/bytes/flags. Instructions and registers otherwise match (frame in edi, the mailbox copy in the dead argument slot).
// Leaf: ?SubmitFrame@CCodaFrameStreamer@@UAEXPAUCCodaFrame@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C49470, 415 bytes, thiscall `ret 4`, EH-framed (the trace string, state 0; the sound
//   lock, state 1).
// Identity (class from its trace file, method name descriptive; GameCODA / libpk has no PDB or RTTI): slot 0 of the streamer
//   vtable 0x012B8138 (its only reference), the vtable the landed destructor 0x00C49650 stores (CCodaFrameStreamer.cpp traces,
//   frames +0x3C, free-frame mailbox +0x1C, in-flight ring +0x48, source +0x04); slot 5 of the same vtable is the landed
//   SetSourceVolume 0x00C49420. The trace names ".\CCodaFrameStreamer.cpp" (0x012B8110), line 20.
// Callees: the frame's +0x10 data getter 0x00C491D0 (landed as the propagated `DrawGetEnvironment`, __fastcall), buffer creation 0x00C36080 (stdcall wrapper
//   of 0x00C3CD00, which builds a 0x40-byte buffer object from the descriptor), the source queue call 0x00C35A10 (landed AudioObject_PlaySource), the
//   in-flight ring CLockedRingBuffer::Push 0x00C49D50 (landed leaf), CODA_IsSourcePlaying 0x00C35CA0 (landed), the play call
//   0x00C35DD0, the free-frame mailbox CMailboxQueue::ProcessMessage 0x00C49B90 (bound), PKCProtectionInstance 0x00C0FFC0 /
//   ~0x00C0FFE0 over g_soundLock 0x0143BAFC (bound), and the libpk trace stream (CDebugStream 0x00C0DEC0 / 0x00C0DF00 /
//   ~0x00C0BA40, g_pDebugBuffer 0x0139C900, g_pfnErrorReport 0x0143BB2C, CDebugBreak 0x00C16750).
// Layout: local model (no engine header; libpk is not in the PDB type set).
class PKCCriticalSection;
class PKCProtectionInstance { public:              // 8-byte scoped lock
    void* m_pSection;
    unsigned long m_state;
    PKCProtectionInstance(PKCCriticalSection* section);   // 0x00C0FFC0
    ~PKCProtectionInstance();                              // 0x00C0FFE0
};
extern PKCCriticalSection g_soundLock;                     // 0x0143BAFC
struct CDebugStream {
    char* m_pBuffer;
    CDebugStream(char* buffer) : m_pBuffer(buffer) { }
    CDebugStream& __fastcall operator<<(const char* text); // 0x00C0DEC0
    CDebugStream& __fastcall operator<<(long value);       // 0x00C0DF00
    ~CDebugStream();                                       // 0x00C0BA40
};
struct CDebugBreak { CDebugBreak(); };                     // 0x00C16750
extern char* g_pDebugBuffer;                               // 0x0139C900
extern void (__fastcall* g_pfnErrorReport)(const char*);   // 0x0143BB2C

struct SCodaBufferDesc {                   // the descriptor 0x00C36080 / 0x00C3CD00 consume (descriptive)
    long  m_flags;                         // +0x00 (1)
    long  m_channels;                      // +0x04
    long  m_rate;                          // +0x08
    long  m_bytes;                         // +0x0C
    long  m_reserved10;                    // +0x10
    long  m_reserved14;                    // +0x14
    void* m_pData;                         // +0x18
};
struct CCodaFrame {
    long  m_channels;                      // +0x00
    long  m_reserved04;                    // +0x04
    long  m_samples;                       // +0x08
    void* m_hBuffer;                       // +0x0C the CODA buffer while in flight
    void* m_pData;                         // +0x10
};
// The three CODA calls keep their landed spellings (one address, one name) although usage disputes two of them: 0x00C36080
// is landed as `operator_new` (global_operatornew_00c36080.cpp: a stdcall forwarder to 0x00C3CD00) but takes the buffer
// descriptor here; 0x00C35DD0 is `CODA_StopSource(source, bool)` in the landed CCodaPlayback::Update and is called with
// true here right after IsSourcePlaying returned 0 (a play/stop switch?). Review at landing.
struct CAudioObject;
struct CAudioSource;
int __fastcall DrawGetEnvironment(const unsigned char* self);                                  // 0x00C491D0 (landed)
void* __stdcall operator_new(unsigned int n);                                                   // 0x00C36080 (landed)
int __stdcall AudioObject_PlaySource(CAudioObject* object, CAudioSource* source, long a, long b);   // 0x00C35A10 (landed)
extern "C" long  __stdcall CODA_IsSourcePlaying(void* source);                                 // 0x00C35CA0 (landed)
extern "C" long  __stdcall CODA_StopSource(void* source, bool immediate);                      // 0x00C35DD0 (landed decl)

struct CMailboxQueue {                     // +0x1C: the free-frame mailbox (landed 0x00C49B90)
    unsigned char _body[0x20];
    long ProcessMessage(void* message);    // 0x00C49B90
};
struct CLockedRingBuffer {                 // +0x48: the in-flight ring (landed 0x00C49D50)
    unsigned char m_section[0x18];
    long          m_capacity;
    CCodaFrame**  m_pSlots;
    long          m_count;
    long          m_tail;
    void Push(CCodaFrame* frame);          // 0x00C49D50
};

class CCodaFrameStreamer {                 // vtable 0x012B8138
public:
    void*             m_pSource;           // +0x04 the CODA source
    unsigned char     _pad08[0x14];        // +0x08
    CMailboxQueue     m_freeFrames;        // +0x1C
    unsigned long     m_frames;            // +0x3C
    long              m_rate;              // +0x40
    long              m_channels;          // +0x44
    CLockedRingBuffer m_inFlight;          // +0x48
    long              m_framesOut;         // +0x70 frames handed out and not yet returned
    virtual void SubmitFrame(CCodaFrame* frame);   // slot 0 (0x00C49470)
};

void CCodaFrameStreamer::SubmitFrame(CCodaFrame* frame)
{
    if (m_framesOut == 0)
    {
        {
            CDebugStream s(g_pDebugBuffer);
            s << ".\\CCodaFrameStreamer.cpp" << "(" << 20 << ") : " << "Must have got the frame from here!?!" << "\n";
            if (g_pfnErrorReport)
                g_pfnErrorReport(s.m_pBuffer);
        }
        CDebugBreak();
    }
    --m_framesOut;
    if (frame->m_samples != 0 && frame->m_channels == m_channels)
    {
        SCodaBufferDesc desc;
        desc.m_flags = 1;
        desc.m_channels = m_channels;
        desc.m_rate = m_rate;
        desc.m_bytes = frame->m_channels * frame->m_samples * 2;
        desc.m_reserved10 = 0;
        desc.m_reserved14 = 0;
        desc.m_pData = (void*)DrawGetEnvironment((const unsigned char*)frame);
        PKCProtectionInstance lock(&g_soundLock);
        frame->m_hBuffer = operator_new((unsigned int)&desc);
        AudioObject_PlaySource((CAudioObject*)m_pSource, (CAudioSource*)frame->m_hBuffer, 0, 0);
        m_inFlight.Push(frame);
        if (!CODA_IsSourcePlaying(m_pSource))
            CODA_StopSource(m_pSource, true);
    }
    else
    {
        CCodaFrame* returned = frame;
        m_freeFrames.ProcessMessage(&returned);
    }
}
