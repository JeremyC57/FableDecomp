#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_from = -1; static const char* g_text = 0;
struct C2DVector_0048da62 { float X, Y; };
class CCharString { public: void* m_pData; const char* GetCharacters(long from) const { g_from = from; return (const char*)m_pData; } };
class CFontBank { public: virtual void v00() { } virtual C2DVector_0048da62 GetStringSize(const char* t) { g_text = t; C2DVector_0048da62 v; v.X = 4.0f * (float)std::strlen(t); v.Y = 1.0f; return v; }
    float GetStringWidth(const CCharString& text) { return GetStringSize(text.GetCharacters(0)).X; } };
int main() {
    CFontBank b; CCharString s; s.m_pData = (void*)"abc"; if (b.GetStringWidth(s) != 12.0f || g_from != 0 || std::strcmp(g_text, "abc")) { std::printf("0048da62_TEST FAIL\n"); return 1; }
    std::printf("0048da62_TEST PASS\n"); return 0;
}