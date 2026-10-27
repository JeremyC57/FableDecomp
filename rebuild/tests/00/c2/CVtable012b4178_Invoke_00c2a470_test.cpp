#include <cstdio>

// Behaviour model of CVtable_012b4178::Invoke (0x00C2A470): the target's update with the held flag byte.
struct Target { int m_last; void Update(int flag) { m_last = flag; } };
struct Callback { unsigned char m_flag; void Invoke(Target* t) { t->Update(m_flag); } };

int main()
{

    Target t; t.m_last = -1; Callback c; c.m_flag = 0xFF;
    c.Invoke(&t); if (t.m_last != 255) { std::printf("00c2a470_TEST FAIL\n"); return 1; }
    c.m_flag = 0; c.Invoke(&t); if (t.m_last != 0) { std::printf("00c2a470_TEST FAIL\n"); return 1; }
    std::printf("00c2a470_TEST PASS\n");
    return 0;
}