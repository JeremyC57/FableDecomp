#include <cstdio>
struct CThing { int id; };
struct CCharString { const char* m_pData; CCharString() : m_pData("") { } };
struct CTCBase {
    CThing* m_pParent; bool m_flag8, m_flag9;
    CTCBase(CThing* parent) : m_pParent(parent), m_flag8(false), m_flag9(false) { }
    virtual ~CTCBase() { }
};
struct CTCRotate : CTCBase {
    float m_axis[3]; float m_speed; long m_unknown1c; long m_framesElapsed; long m_framesTotal;
    bool m_finished; CCharString m_name; long m_index; bool m_active;
    CTCRotate(CThing* parent)
        : CTCBase(parent), m_speed(0.0f), m_framesElapsed(0), m_framesTotal(0), m_finished(false), m_name(), m_index(-1), m_active(false) { }
};
int main()
{
    CThing t;
    CTCRotate r(&t);
    if (r.m_pParent != &t || r.m_speed != 0.0f || r.m_framesElapsed != 0 || r.m_framesTotal != 0 || r.m_finished || r.m_name.m_pData[0] != 0 || r.m_index != -1 || r.m_active) { std::printf("004d2b2a_TEST FAIL\n"); return 1; }
    std::printf("004d2b2a_TEST PASS\n");
    return 0;
}
