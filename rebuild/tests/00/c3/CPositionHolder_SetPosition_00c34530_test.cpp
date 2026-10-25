// Behaviour test for 00c34530: CPositionHolder::SetPosition -- stores x/y and reports a change
#include <cstdio>
struct CFloatPair {
    float m_x, m_y;
    float GetX() const { return m_x; }
    float GetY() const { return m_y; }
    void SetX(float x) { m_x = x; }
    void SetY(float y) { m_y = y; }
};
struct CPositionHolder {
    unsigned char _pad00[0x14];
    CFloatPair m_position;
    bool SetPosition(float x, float y)
    {
        if (m_position.GetY() == y && m_position.GetX() == x)
            return false;
        m_position.SetX(x);
        m_position.SetY(y);
        return true;
    }
};
int main()
{
    CPositionHolder h; h.m_position.m_x = 1.0f; h.m_position.m_y = 2.0f;
    int ok = 1;
    if (h.SetPosition(1.0f, 2.0f)) ok = 0;
    if (!h.SetPosition(3.0f, 2.0f)) ok = 0;
    if (h.m_position.m_x != 3.0f || h.m_position.m_y != 2.0f) ok = 0;
    if (!h.SetPosition(3.0f, 5.0f)) ok = 0;
    if (h.m_position.m_y != 5.0f) ok = 0;
    if (h.SetPosition(3.0f, 5.0f)) ok = 0;
    std::printf(ok ? "00c34530_TEST PASS\n" : "00c34530_TEST FAIL\n");
    return ok ? 0 : 1;
}