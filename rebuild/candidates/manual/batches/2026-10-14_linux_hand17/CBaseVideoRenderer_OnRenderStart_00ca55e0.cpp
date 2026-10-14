// CBaseVideoRenderer::OnRenderStart(IMediaSample*) -- the DirectShow base-class member: record the
// lateness of the frame about to be drawn (virtual RecordFrameLateness(m_trLate, m_trFrame)) and stamp
// m_tRenderStart with timeGetTime(). The sample argument is unused.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA55E0, length 43 bytes.
// Callees: vtable slot 45 (+0xb4) RecordFrameLateness, WINMM!timeGetTime through the import @ 0x014404B4.
// Members: m_tRenderStart +0xfc, m_trLate +0x150, m_trFrame +0x154. Manifest label
// (CChunkedFileRawDataChunk::WriteToFile) is propagated garbage.
struct IMediaSample;
extern "C" __declspec(dllimport) unsigned long __stdcall timeGetTime(void);   // import @ 0x014404B4

struct CBaseVideoRenderer {
    virtual void VSlot0();
    virtual void VSlot1();
    virtual void VSlot2();
    virtual void VSlot3();
    virtual void VSlot4();
    virtual void VSlot5();
    virtual void VSlot6();
    virtual void VSlot7();
    virtual void VSlot8();
    virtual void VSlot9();
    virtual void VSlot10();
    virtual void VSlot11();
    virtual void VSlot12();
    virtual void VSlot13();
    virtual void VSlot14();
    virtual void VSlot15();
    virtual void VSlot16();
    virtual void VSlot17();
    virtual void VSlot18();
    virtual void VSlot19();
    virtual void VSlot20();
    virtual void VSlot21();
    virtual void VSlot22();
    virtual void VSlot23();
    virtual void VSlot24();
    virtual void VSlot25();
    virtual void VSlot26();
    virtual void VSlot27();
    virtual void VSlot28();
    virtual void VSlot29();
    virtual void VSlot30();
    virtual void VSlot31();
    virtual void VSlot32();
    virtual void VSlot33();
    virtual void VSlot34();
    virtual void VSlot35();
    virtual void VSlot36();
    virtual void VSlot37();
    virtual void VSlot38();
    virtual void VSlot39();
    virtual void VSlot40();
    virtual void VSlot41();
    virtual void VSlot42();
    virtual void VSlot43();
    virtual void VSlot44();
    virtual void RecordFrameLateness(int trLate, int trFrame);   // slot 45 (+0xb4)
    unsigned char _pad_0x04[0xf8];              // +0x004
    long m_tRenderStart;                        // +0x0fc
    unsigned char _pad_0x100[0x50];             // +0x100
    int m_trLate;                               // +0x150
    int m_trFrame;                              // +0x154
    void OnRenderStart(IMediaSample* pMediaSample);
};

void CBaseVideoRenderer::OnRenderStart(IMediaSample* pMediaSample)
{
    RecordFrameLateness(m_trLate, m_trFrame);
    m_tRenderStart = timeGetTime();
}
