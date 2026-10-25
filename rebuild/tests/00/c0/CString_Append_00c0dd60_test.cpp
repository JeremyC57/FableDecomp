// Behaviour test (0x00c0dd60): CString::Append(double) appends the value printed with "%.2f"
#include <cstdio>
#include <cstring>
class CString {
public:
    char m_text[128];
    CString() { m_text[0] = 0; }
    void Append(const char* text) { std::strcat(m_text, text); }
    void Append(double value)
    {
        char buffer[64];
        std::sprintf(buffer, "%.2f", value);
        Append(buffer);
    }
};
int main()
{
    CString s;
    s.Append("x=");
    s.Append(3.14159);
    s.Append(";y=");
    s.Append(-2.0);
    if (std::strcmp(s.m_text, "x=3.14;y=-2.00") != 0) { std::printf("00c0dd60_TEST FAIL\n"); return 1; }
    std::printf("00c0dd60_TEST PASS\n");
    return 0;
}