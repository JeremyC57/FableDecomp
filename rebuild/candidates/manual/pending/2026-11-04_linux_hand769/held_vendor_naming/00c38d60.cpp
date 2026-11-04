// CInstance::Update(unsigned long frames, unsigned long samples, unsigned long prime) -- run the instance's stream state machine until it waits: (1) once the start and format ports both hold a message, take the start request (stop requested instead: detach the done port and go to 4; refused: back to 0), create the 0x74-byte CODA frame streamer for the format and the caller's frame / sample counts at the instance scale (assert "EMEM", line 202), assert that `prime` does not exceed the frame count (line 203), prime that many silenced frames, go to 2; (2) attach the ready streamer to the registry with the frame port, go to 3; (3) take the returned frame, keep streaming (2) while it is flagged and the ramp is not settled at zero, else detach and go to 4, advance the ramp by the streamer position and hand the frame back; (4) once the done port holds a message and the streamer is idle, stop, delete the streamer and reset to 0 with the three ids cleared; (5) once the final port holds a message, finish (6)
// Leaf: ?Update@CInstance@@QAEXKKK@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C38D60, 972 bytes (padded to 976), thiscall `ret 0Ch`, EH frame (state 0 around the
//   streamer's constructor, 1 / 2 around the two assert strings), 7-entry jump table 0x00C39110 (0 and 6 share the exit).
// Identity: libpk .\CInstance.cpp (assert file string 0x012B542C, lines 0xCA / 0xCB). The only caller 0x00C2D490 binds
//   ?Update@CInstance@@QAEXJJJ@Z; the counts are unsigned (`prime > frames` is `cmp ; jbe`, the prime loop counts down with
//   `dec ; jne`, frames / samples go to the ctor's unsigned slots: NAME_FIXES). Class layout as the landed CInstance::SetScale
//   0x00C38850 (frame streamer +0x16C, scale +0x17C, state +0xF8) and the landed dtor 0x00C38B20 (landed as
//   CActivationReceptor_00c38b20, the same record: +0xFC registry with Detach 0x00C48560, +0x16C streamer deleted through
//   slot 6, the Query / Current / Stop / Final helpers). The manifest label "CTCActivationReceptorBase_HandleStateTransition"
//   is propagated and wrong.
// Callees: the five mailbox ports' message tests 0x00C393F0 (+0xC), 0x00C395B0 (+0x40), 0x00C39770 (+0x74), 0x00C39930
//   (+0xAC: the landed shape, under a scoped lock the port's mailbox slot 2), 0x00C39AF0 (+0xD8); CInstance::Query 0x00C38A40
//   (start flag, stop flag out, optional 8-byte format out), CheckFrameStreamer 0x00C388A0, Current 0x00C38AC0, Stop 0x00C38AF0,
//   Final 0x00C38B10; the registry's Attach 0x00C483A0 (landed as CAIStateGroup_BirdFlyToFood_00c483a0::Update(SPair), a
//   propagated label) and Detach 0x00C48560; CRampedParam_00c3a8f0::IsSettledAtZero 0x00C3A8F0 (tested as a bool: `test al,al`)
//   and the ramp step 0x00C3A7D0; operator new 0x00BFEA1A / delete 0x00BFE9BC; CCodaFrameStreamer_00c49650 ctor 0x00C49820
//   (the landed definer's spelling); CCodaFrame::Silence 0x00C49180; streamer slots 0 (take a frame back), 2, 3 (free frame),
//   4 (position), 6 (deleting dtor); the assert stream CString 0x00C0DEC0 / 0x00C0DF00 / 0x00C0BA40, CDebugBreak 0x00C16750,
//   g_pchNil_0139c900, g_pfnLogSink_0143bb2c.
// Shape: a `done`-flag loop around the switch (VC7.1 then jumps back to the state load instead of rotating the dispatch to the
//   loop bottom), with explicit `done = true` cases 0 and 6 beside the default (so the table spans 0..6 with no `dec eax`);
//   the refusals return from inside the cases (their own epilogues); the assert blocks are the landed CheckFrameStreamer form.
void* __cdecl operator new(unsigned int size);              // 0x00BFEA1A
void __cdecl operator delete(void* block);                  // 0x00BFE9BC
extern char* g_pchNil_0139c900;                             // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c38d60)(const char* text);
extern PFNLOGSINK_00c38d60 g_pfnLogSink_0143bb2c;          // 0x0143BB2C

class CString { public:
    char* m_pchData;
    CString() : m_pchData(g_pchNil_0139c900) { }
    ~CString();                                             // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);       // 0x00C0DEC0
    CString& __fastcall operator<<(long value);             // 0x00C0DF00
};
struct CDebugBreak {
    CDebugBreak();                                          // 0x00C16750
};

struct CCodaFrame {
    void Silence();                                         // 0x00C49180
};
struct SItem_00c38b20 { long m_0; long m_4; long m_8; };

class CFrameStreamer { public:
    virtual void Take(void* frame);                         // slot 0
    virtual long IsReady();                                 // slot 1
    virtual long IsPlaying();                               // slot 2
    virtual CCodaFrame* GetFreeFrame();                     // slot 3
    virtual long GetPosition();                             // slot 4
    virtual void SetScale(float scale);                     // slot 5
    virtual ~CFrameStreamer();                              // slot 6
};
class CCodaFrameStreamer_00c49650 : public CFrameStreamer { public:
    unsigned char _body[0x70];
    CCodaFrameStreamer_00c49650(unsigned long channels, long rate, unsigned long frames, unsigned long samples, float volume);   // 0x00C49820
};

struct SStreamFormat { unsigned long m_channels; long m_rate; };

struct CMailboxPort_00c393f0 { unsigned char _body[0x34]; long HasMessage(); };   // 0x00C393F0
struct CMailboxPort_00c395b0 { unsigned char _body[0x34]; long HasMessage(); };   // 0x00C395B0
struct CMailboxPort_00c39770 { unsigned char _body[0x38]; long HasMessage(); };   // 0x00C39770
struct CMailboxPort_00c39930 { unsigned char _body[0x2c]; long HasMessage(); };   // 0x00C39930
struct CMailboxPort_00c39af0 { unsigned char _body[0x20]; long HasMessage(); };   // 0x00C39AF0
struct CRegistry_00c38b20 {
    unsigned char _body[0x3c];
    void Attach(long streamer, void* port);                // 0x00C483A0
    void Detach(void* port);                                // 0x00C48560
};
struct CRampedParam_00c3a8f0 {
    unsigned char _body[0x34];
    bool IsSettledAtZero();                                 // 0x00C3A8F0
    void Advance(SItem_00c38b20* item, long position);     // 0x00C3A7D0
};

class CInstance { public:
    unsigned char         _pad000[0xc];
    CMailboxPort_00c393f0 m_0c;                             // +0x00C
    CMailboxPort_00c395b0 m_40;                             // +0x040
    CMailboxPort_00c39770 m_74;                             // +0x074
    CMailboxPort_00c39930 m_ac;                             // +0x0AC
    CMailboxPort_00c39af0 m_d8;                             // +0x0D8
    long                  m_state;                          // +0x0F8
    CRegistry_00c38b20    m_registry;                       // +0x0FC
    CRampedParam_00c3a8f0 m_138;                            // +0x138
    CFrameStreamer*       m_pFrameStreamer;                 // +0x16C
    long                  m_170;                            // +0x170
    long                  m_174;                            // +0x174
    long                  m_178;                            // +0x178
    float                 m_scale;                          // +0x17C
    bool Query(bool* stop, SStreamFormat* format);          // 0x00C38A40
    long CheckFrameStreamer();                              // 0x00C388A0
    SItem_00c38b20* Current();                              // 0x00C38AC0
    void Stop();                                            // 0x00C38AF0
    void Final();                                           // 0x00C38B10
    void Update(unsigned long frames, unsigned long samples, unsigned long prime);
};

void CInstance::Update(unsigned long frames, unsigned long samples, unsigned long prime)
{
    bool done = false;
    while (!done) {
        switch (m_state) {
        case 0:
            done = true;
            break;
        case 1: {
            if (!m_ac.HasMessage())
                return;
            if (!m_40.HasMessage())
                return;
            bool stop;
            SStreamFormat format;
            if (!Query(&stop, &format)) {
                if (stop) {
                    m_state = 4;
                    m_registry.Detach(&m_d8);
                } else {
                    m_state = 0;
                }
                return;
            }
            m_pFrameStreamer = new CCodaFrameStreamer_00c49650(format.m_channels, format.m_rate, frames, samples, m_scale);
            if (!m_pFrameStreamer) {
                {
                    CString line;
                    line << ".\\CInstance.cpp" << "(" << 202L << ") : " << "EMEM" << "\n";
                    if (g_pfnLogSink_0143bb2c)
                        g_pfnLogSink_0143bb2c(line.m_pchData);
                }
                CDebugBreak();
            }
            if (prime > frames) {
                {
                    CString line;
                    line << ".\\CInstance.cpp" << "(" << 203L << ") : "
                         << "Cannot prime more than the streaming buffer count - it will block!" << "\n";
                    if (g_pfnLogSink_0143bb2c)
                        g_pfnLogSink_0143bb2c(line.m_pchData);
                }
                CDebugBreak();
            }
            for (unsigned long i = 0; i < prime; ++i) {
                CCodaFrame* frame = m_pFrameStreamer->GetFreeFrame();
                frame->Silence();
                m_pFrameStreamer->Take(frame);
            }
            m_state = 2;
            break;
        }
        case 2: {
            long ready = CheckFrameStreamer();
            if (!ready)
                return;
            m_registry.Attach(ready, &m_74);
            m_state = 3;
            break;
        }
        case 3: {
            if (!m_74.HasMessage())
                return;
            SItem_00c38b20* item = Current();
            if (item->m_8 && !m_138.IsSettledAtZero()) {
                m_state = 2;
            } else {
                m_state = 4;
                m_registry.Detach(&m_d8);
            }
            m_138.Advance(item, m_pFrameStreamer->GetPosition());
            m_pFrameStreamer->Take(item);
            break;
        }
        case 4:
            if (!m_d8.HasMessage())
                return;
            if (m_pFrameStreamer && m_pFrameStreamer->IsPlaying())
                return;
            Stop();
            if (m_pFrameStreamer) {
                delete m_pFrameStreamer;
                m_pFrameStreamer = 0;
            }
            m_state = 0;
            m_174 = -1;
            m_178 = -1;
            m_170 = -1;
            return;
        case 5:
            if (!m_0c.HasMessage())
                return;
            Final();
            m_state = 6;
            return;
        case 6:
            done = true;
            break;
        default:
            done = true;
            break;
        }
    }
}
