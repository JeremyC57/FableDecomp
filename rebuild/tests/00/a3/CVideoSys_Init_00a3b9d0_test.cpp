// Behaviour test for CVideoSys::Init (0x00A3B9D0): the disabled flag, the .wmv/.asf RenderFile route versus the source
// filter route (Render the output pin, or FindPin "In" + Connect), HRESULT propagation and the final interface queries.
#include <stdio.h>
#include <string.h>

static int gCalls[16];
enum { C_COCREATE, C_ADDFILTER, C_RENDERFILE, C_ADDSOURCE, C_FINDOUT, C_RENDER, C_FINDIN, C_CONNECT, C_QI, C_CS, C_EVENT,
       C_UPDATE, C_CLEAR, C_PARSE, C_NEWRENDERER, C_ADDREF };
static int gFail = -1;   // which call fails (returns a negative HRESULT)
static int Hr(int which) { ++gCalls[which]; return gFail == which ? -5 : 0; }

static const char* Find(const char* s, const char* t) { return strstr(s, t); }

struct Renderer { void* PVideoSys; int refs; };
struct VideoSys {
    void* GBuilder; void* MControl; void* MPosition; void* MSeeking; void* MEvent; void* BasicAudio;
    Renderer* TexRenderer; int State; void* RenderEvent; void* TextBank; void* SubtitleBank;
    int Init(const char* filename, bool render_source_pin, bool disabled)
    {
        if (disabled)
            return 0;
        ++gCalls[C_UPDATE]; ++gCalls[C_CLEAR];
        SubtitleBank = TextBank; ++gCalls[C_PARSE];
        bool render_file = false;
        if (Find(filename, ".wmv") || Find(filename, ".asf"))
            render_file = true;
        int hr = Hr(C_COCREATE);
        if (hr < 0)
            return hr;
        GBuilder = (void*)this;
        if (!TexRenderer) {
            static Renderer r; r.PVideoSys = 0; r.refs = 0;
            ++gCalls[C_NEWRENDERER];
            TexRenderer = &r;
            TexRenderer->PVideoSys = this;
            ++TexRenderer->refs; ++gCalls[C_ADDREF];
        }
        hr = Hr(C_ADDFILTER);
        if (hr < 0)
            return hr;
        if (render_file) {
            hr = Hr(C_RENDERFILE);
            if (hr < 0)
                return hr;
        }
        else {
            hr = Hr(C_ADDSOURCE);
            if (hr < 0)
                return hr;
            hr = Hr(C_FINDOUT);
            if (hr < 0)
                return hr;
            if (render_source_pin) {
                Hr(C_RENDER);
            }
            else {
                hr = Hr(C_FINDIN);
                if (hr < 0)
                    return hr;
                hr = Hr(C_CONNECT);
                if (hr < 0)
                    return hr;
            }
        }
        for (int i = 0; i < 5; ++i) Hr(C_QI);
        State = 3;
        ++gCalls[C_CS];
        RenderEvent = (void*)1; ++gCalls[C_EVENT];
        return 0;
    }
};

static void Reset() { memset(gCalls, 0, sizeof(gCalls)); gFail = -1; }

int main()
{
    int ok = 1;
    VideoSys v; memset(&v, 0, sizeof(v));
    Reset(); ok &= v.Init("intro.wmv", false, true) == 0 && gCalls[C_COCREATE] == 0;
    Reset(); ok &= v.Init("intro.wmv", false, false) == 0;
    ok &= gCalls[C_RENDERFILE] == 1 && gCalls[C_ADDSOURCE] == 0 && gCalls[C_QI] == 5 && v.State == 3 && gCalls[C_NEWRENDERER] == 1;
    ok &= v.TexRenderer->PVideoSys == (void*)&v && v.TexRenderer->refs == 1;
    Reset(); ok &= v.Init("intro.bik", true, false) == 0;
    ok &= gCalls[C_RENDERFILE] == 0 && gCalls[C_RENDER] == 1 && gCalls[C_FINDIN] == 0 && gCalls[C_NEWRENDERER] == 0;
    Reset(); ok &= v.Init("intro.bik", false, false) == 0;
    ok &= gCalls[C_FINDIN] == 1 && gCalls[C_CONNECT] == 1;
    Reset(); gFail = C_CONNECT; ok &= v.Init("intro.asf.bik", false, false) == 0 && gCalls[C_RENDERFILE] == 1;
    Reset(); gFail = C_CONNECT; ok &= v.Init("x.bik", false, false) == -5 && gCalls[C_QI] == 0;
    Reset(); gFail = C_RENDER; ok &= v.Init("x.bik", true, false) == 0 && gCalls[C_QI] == 5;
    Reset(); gFail = C_COCREATE; ok &= v.Init("x.bik", true, false) == -5 && gCalls[C_ADDFILTER] == 0;
    if (!ok) { printf("00a3b9d0_TEST FAIL\n"); return 1; }
    printf("00a3b9d0_TEST PASS\n");
    return 0;
}