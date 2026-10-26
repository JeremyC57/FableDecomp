#include <cstdio>
struct CTurnState { void* m_pFirst; void* m_pLast; unsigned char m_kind; unsigned char m_flags; };
struct CTCTurnSource { CTurnState* m_pState; CTurnState* GetState() { return m_pState; } };
class CThing { public:
    unsigned long m_presentBits[9]; CTCTurnSource* m_comp;
    template <class T> bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) { *out = (T*)m_comp; return true; }
        return false;
    }
};
class CTCLook { public:
    CThing* m_pThing;
    bool IsCurrentlyTurningViaAnimation()
    {
        CTCTurnSource* source = 0;
        m_pThing->GetInterface(90, &source);
        CTurnState* state = source->GetState();
        if (state->m_kind == 7 && state->m_pFirst != state->m_pLast && !(state->m_flags & 2)) return true;
        return false;
    }
};
int main()
{
    int a, b;
    CTurnState st; st.m_pFirst = &a; st.m_pLast = &b; st.m_kind = 7; st.m_flags = 0;
    CTCTurnSource src; src.m_pState = &st;
    CThing t; for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0; t.m_presentBits[2] = 0x4000000; t.m_comp = &src;
    CTCLook l; l.m_pThing = &t;
    if (!l.IsCurrentlyTurningViaAnimation()) { std::printf("00753b40_TEST FAIL\n"); return 1; }
    st.m_flags = 2;
    if (l.IsCurrentlyTurningViaAnimation()) { std::printf("00753b40_TEST FAIL\n"); return 1; }
    st.m_flags = 0; st.m_pLast = &a;
    if (l.IsCurrentlyTurningViaAnimation()) { std::printf("00753b40_TEST FAIL\n"); return 1; }
    st.m_pLast = &b; st.m_kind = 6;
    if (l.IsCurrentlyTurningViaAnimation()) { std::printf("00753b40_TEST FAIL\n"); return 1; }
    std::printf("00753b40_TEST PASS\n");
    return 0;
}