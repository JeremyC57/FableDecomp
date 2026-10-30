// Behaviour test for CBaseVideoRenderer::SendQuality (0x00CA56E0): message type from the averages, proportion rules
// (1000 default; late -> 1000 - late/10000 floored at 500; waiting a lot -> up to 2000), lateness plus half the render
// average, the sink fetched lazily from the upstream pin, S_FALSE (1) when there is no sink.
#include <cstdio>
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif

struct Quality { int Type; long Proportion; i64 Late; i64 TimeStamp; };
struct Sink { int calls; Quality last; long Notify(const Quality& q) { ++calls; last = q; return 0; } };
struct Pin { Sink* sink; };

struct Renderer {
    int m_trRenderAvg, m_trWaitAvg, m_trFrameAvg;
    Sink* m_pQSink; Pin* m_pConnected;
    long SendQuality(i64 trLate, i64 trRealStream)
    {
        Quality q;
        q.TimeStamp = trRealStream;
        if (m_trFrameAvg < 0) q.Type = 0;
        else if (m_trFrameAvg > 2 * m_trRenderAvg) q.Type = 0;
        else q.Type = 1;
        q.Proportion = 1000;
        if (m_trFrameAvg < 0) {
        } else if (trLate > 0) {
            q.Proportion = 1000 - (int)(trLate / 10000);
            if (q.Proportion < 500) q.Proportion = 500;
        } else if (m_trWaitAvg > 20000 && trLate < -20000) {
            if (m_trWaitAvg >= m_trFrameAvg) q.Proportion = 2000;
            else if (m_trFrameAvg + 20000 > m_trWaitAvg)
                q.Proportion = 1000 * (m_trFrameAvg / (m_trFrameAvg + 20000 - m_trWaitAvg));
            else q.Proportion = 2000;
            if (q.Proportion > 2000) q.Proportion = 2000;
        }
        q.Late = trLate + m_trRenderAvg / 2;
        if (m_pQSink == 0 && m_pConnected->sink) m_pQSink = m_pConnected->sink;
        if (m_pQSink) return m_pQSink->Notify(q);
        return 1;
    }
};

int main()
{
    int ok = 1;
    Sink sink; sink.calls = 0;
    Pin pin; pin.sink = 0;
    Renderer r; r.m_trRenderAvg = 1000; r.m_trWaitAvg = 0; r.m_trFrameAvg = 5000; r.m_pQSink = 0; r.m_pConnected = &pin;
    ok &= r.SendQuality(0, 7) == 1;                               // no sink upstream
    pin.sink = &sink;
    ok &= r.SendQuality(2000000, 9) == 0 && sink.calls == 1 && r.m_pQSink == &sink;
    ok &= sink.last.Type == 0 && sink.last.Proportion == 800 && sink.last.Late == 2000500 && sink.last.TimeStamp == 9;
    r.SendQuality(9000000, 0);
    ok &= sink.last.Proportion == 500;
    r.m_trRenderAvg = 4000; r.m_trWaitAvg = 30000; r.m_trFrameAvg = 400000;
    r.SendQuality(-30000, 0);
    ok &= sink.last.Type == 0 && sink.last.Proportion == 1000 * (400000 / (400000 + 20000 - 30000));
    r.m_trFrameAvg = 25000;
    r.SendQuality(-30000, 0);
    ok &= sink.last.Proportion == 2000;
    r.m_trFrameAvg = 6000;                                        // mainly bltting: Flood
    r.SendQuality(0, 0);
    ok &= sink.last.Type == 1 && sink.last.Proportion == 1000;
    r.m_trFrameAvg = -1;
    r.SendQuality(5000000, 0);
    ok &= sink.last.Proportion == 1000 && sink.last.Type == 0;
    if (!ok) { std::printf("00ca56e0_TEST FAIL\n"); return 1; }
    std::printf("00ca56e0_TEST PASS\n");
    return 0;
}