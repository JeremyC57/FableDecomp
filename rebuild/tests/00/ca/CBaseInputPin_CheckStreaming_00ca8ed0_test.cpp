#include <cstdio>
// CBaseInputPin::CheckStreaming: stopped -> VFW_E_WRONG_STATE; flushing -> S_FALSE; run-time error -> VFW_E_RUNTIME_ERROR; else S_OK.
struct Filter { int m_State; };
struct InPin {
    Filter* m_pFilter; bool m_bRunTimeError; unsigned char m_bFlushing;
    int CheckStreaming()
    {
        if (m_pFilter->m_State == 0) return (int)0x80040227;
        if (m_bFlushing) return 1;
        if (m_bRunTimeError) return (int)0x8004020B;
        return 0;
    }
};
int main()
{
    Filter f; InPin p; p.m_pFilter = &f; p.m_bRunTimeError = false; p.m_bFlushing = 0;
    f.m_State = 0; if (p.CheckStreaming() != (int)0x80040227) { std::printf("00ca8ed0_TEST FAIL\n"); return 1; }
    f.m_State = 2; if (p.CheckStreaming() != 0) { std::printf("00ca8ed0_TEST FAIL\n"); return 1; }
    p.m_bRunTimeError = true; if (p.CheckStreaming() != (int)0x8004020B) { std::printf("00ca8ed0_TEST FAIL\n"); return 1; }
    p.m_bFlushing = 1; if (p.CheckStreaming() != 1) { std::printf("00ca8ed0_TEST FAIL\n"); return 1; }
    std::printf("00ca8ed0_TEST PASS\n");
    return 0;
}