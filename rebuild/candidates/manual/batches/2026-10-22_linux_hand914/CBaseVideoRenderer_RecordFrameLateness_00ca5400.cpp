// CBaseVideoRenderer::RecordFrameLateness(int, int) -- DirectShow base-class quality statistics: accumulates the clamped lateness and inter-frame time (ms) and counts the frame
// Leaf: RecordFrameLateness
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA5400, 262 bytes, `ret 8`. CTextureRenderer vtable 0x0129D08C slot 45 (the
// inherited strmbase CBaseVideoRenderer method; CTextureRenderer is the DirectShow movie texture renderer). No callees.
// Shape: the strmbase renbase.cpp body (MSR_INTEGER compiled out): `/10000` as the 0x68DB8BAD reciprocal with `sar 0Ch`;
// out-of-range lateness becomes 0 for the first frames, else `tLate > 0 ? 1000 : -1000` (the setle/dec/and/add form);
// the LONGLONG accumulators take a sign-extended 32-bit value (the squares are 32-bit products); FRAMES_TO_IGNORE is 1.
// Layout (retail, this TU's CBaseVideoRenderer): m_cFramesDrawn +0x124, m_iTotAcc +0x128, m_iSumSqAcc +0x130,
// m_iSumSqFrameTime +0x140, m_iSumFrameTime +0x148; the members in between are not touched here.
#define FRAMES_TO_IGNORE 1

class CBaseVideoRenderer {
public:
    void RecordFrameLateness(int trLate, int trFrame);   // virtual in strmbase (slot 45 here)
private:
    void*   __vftable;                 // +0x000 (a plain pointer: a real vptr would pad to 8 before the __int64 members)
    unsigned char _pad004[0x124 - 4];
    int     m_cFramesDrawn;            // +0x124
    __int64 m_iTotAcc;                 // +0x128
    __int64 m_iSumSqAcc;               // +0x130
    unsigned char _pad138[0x140 - 0x138];
    __int64 m_iSumSqFrameTime;         // +0x140
    __int64 m_iSumFrameTime;           // +0x148
};

void CBaseVideoRenderer::RecordFrameLateness(int trLate, int trFrame)
{
    // Record how timely we are.
    int tLate = trLate / 10000;

    // Ignore frames more than a second off (they invalidate the statistics).
    if (tLate > 1000 || tLate < -1000) {
        if (m_cFramesDrawn <= FRAMES_TO_IGNORE) {
            tLate = 0;
        } else if (tLate > 0) {
            tLate = 1000;
        } else {
            tLate = -1000;
        }
    }
    // The very first frame often has an invalid time.
    if (m_cFramesDrawn > FRAMES_TO_IGNORE) {
        m_iTotAcc += tLate;
        m_iSumSqAcc += (tLate * tLate);
    }

    // Inter-frame time makes no sense for the first two frames.
    if (m_cFramesDrawn > FRAMES_TO_IGNORE + 1) {
        int tInterFrame = trFrame / 10000;

        // Clamp the time to 1 second.
        if (tInterFrame > 1000 || tInterFrame < 0) tInterFrame = 1000;
        m_iSumSqFrameTime += tInterFrame * tInterFrame;
        m_iSumFrameTime += tInterFrame;
    }
    ++m_cFramesDrawn;
}
