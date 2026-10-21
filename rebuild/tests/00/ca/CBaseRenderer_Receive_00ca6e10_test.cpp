// Behaviour test for CBaseRenderer::Receive (0x00CA6E10): a failed PrepareReceive returns its code except
// VFW_E_SAMPLE_REJECTED (0); paused, it prepares the render, then under both locks (unless stopped meanwhile -> 0)
// marks in-receive, hands the first sample over and signals completion; a failed render-time wait clears in-receive
// and returns 0; otherwise it renders the pending sample, clears it, sends end-of-stream and cancels the advise.
#include <cstdio>
#include <string>

static std::string g_log;
struct Renderer {
    int state, prepareHr, waitHr; bool stopDuringPause; int inReceive, locks;
    int Receive(int sample)
    {
        int hr = prepareHr; g_log += "P";
        if (hr < 0) return hr == (int)0x8004022B ? 0 : hr;
        if (state == 1) {
            g_log += "R"; inReceive = 0;
            ++locks; if (stopDuringPause) state = 0;
            if (state == 0) { --locks; return 0; }
            inReceive = 1; ++locks; g_log += "F"; locks -= 2;
            g_log += "S";
        }
        hr = waitHr; g_log += "W";
        if (hr < 0) { inReceive = 0; return 0; }
        g_log += "R"; inReceive = 0;
        if (state == 0) return 0;
        g_log += "DCEN";
        return 0;
    }
};

int main()
{
    int ok = 1;
    Renderer r = { 2, (int)0x80004005, 0, false, 0, 0 };
    ok &= r.Receive(1) == (int)0x80004005 && g_log == "P";
    r.prepareHr = (int)0x8004022B; g_log = "";
    ok &= r.Receive(1) == 0 && g_log == "P";
    r.prepareHr = 0; r.state = 1; g_log = "";
    ok &= r.Receive(1) == 0 && g_log == "PRFSWRDCEN" && r.locks == 0 && r.inReceive == 0;
    Renderer s = { 1, 0, 0, true, 0, 0 }; g_log = "";
    ok &= s.Receive(1) == 0 && g_log == "PR" && s.locks == 0;
    Renderer t = { 2, 0, -1, false, 1, 0 }; g_log = "";
    ok &= t.Receive(1) == 0 && g_log == "PW" && t.inReceive == 0;
    std::printf(ok ? "00ca6e10_TEST PASS\n" : "00ca6e10_TEST FAIL\n");
    return ok ? 0 : 1;
}