// CBaseVideoRenderer::ResetStreamingTimes() -- DirectShow base-class quality statistics reset (renbase.cpp), returns NOERROR
// Leaf: ResetStreamingTimes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA52A0, length 191 bytes (+ int3 padding). Vtable 0x0129D08C slot 47
// (the CTextureRenderer video renderer derived from the DirectShow SDK's CBaseVideoRenderer).
// Callee: winmm timeGetTime through its import slot @ 0x014404B4.
// Identity / label correction: the catalog label "CAIStateGroup_MinionHasInformation::OnActivate" is propagated over an
// unrelated body (that class is 0x24 bytes; this one writes up to +0x15c). The store sequence is exactly the SDK body
// of ResetStreamingTimes in source order (m_trLastDraw = -1000 as a 64-bit REFERENCE_TIME, timeGetTime, ...,
// m_trTarget = -300000, m_trThrottle, m_trRememberStampForPerf); the shared `xor eax,eax` zero is also the NOERROR result.
// Audio/video owner note: strmbase row -- retail offsets of the renderer statistics are listed in the model below.
// Shape: the vfptr is a plain pointer member (MSVC aligns the first member after a real vfptr to 8); `extern "C" __declspec(dllimport)` import for the indirect call; one zero register for every 0 store.
extern "C" __declspec(dllimport) unsigned long __stdcall timeGetTime(void);   // 0x014404B4 import slot

typedef long HRESULT;
typedef __int64 LONGLONG;
typedef __int64 REFERENCE_TIME;

struct CBaseVideoRenderer {
    void*         __vftable;                 // +0x00 (a declared virtual would pad the vfptr to 8 for the __int64 members)
    unsigned char _pad_0x04[0xe4];           // CBaseRenderer state
    int           m_nNormal;                 // +0xe8
    unsigned char _pad_0xec[0x4];
    int           m_trThrottle;              // +0xf0
    int           m_trRenderAvg;             // +0xf4
    int           m_trRenderLast;            // +0xf8
    int           m_tRenderStart;            // +0xfc
    int           m_trEarliness;             // +0x100
    int           m_trTarget;                // +0x104
    int           m_trWaitAvg;               // +0x108
    int           m_trFrameAvg;              // +0x10c
    int           m_trDuration;              // +0x110
    unsigned char _pad_0x114[0x4];
    REFERENCE_TIME m_trRememberStampForPerf; // +0x118
    int           m_cFramesDropped;          // +0x120
    int           m_cFramesDrawn;            // +0x124
    LONGLONG      m_iTotAcc;                 // +0x128
    LONGLONG      m_iSumSqAcc;               // +0x130
    REFERENCE_TIME m_trLastDraw;             // +0x138
    LONGLONG      m_iSumSqFrameTime;         // +0x140
    LONGLONG      m_iSumFrameTime;           // +0x148
    int           m_trLate;                  // +0x150
    int           m_trFrame;                 // +0x154
    int           m_tStreamingStart;         // +0x158
    HRESULT ResetStreamingTimes();
};

HRESULT CBaseVideoRenderer::ResetStreamingTimes()
{
    m_trLastDraw = -1000;
    m_tStreamingStart = timeGetTime();
    m_trRenderAvg = 0;
    m_trFrameAvg = -1;
    m_trDuration = 0;
    m_trRenderLast = 0;
    m_trWaitAvg = 0;
    m_tRenderStart = 0;
    m_cFramesDrawn = 0;
    m_cFramesDropped = 0;
    m_iTotAcc = 0;
    m_iSumSqAcc = 0;
    m_iSumSqFrameTime = 0;
    m_trFrame = 0;
    m_trLate = 0;
    m_iSumFrameTime = 0;
    m_nNormal = 0;
    m_trEarliness = 0;
    m_trTarget = -300000;
    m_trThrottle = 0;
    m_trRememberStampForPerf = 0;
    return 0;   // NOERROR
}