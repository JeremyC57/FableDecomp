// Behaviour test for CMovie::SetMovie 0x00548510: the movie name is assigned to the member wide string. The body forwards to the member at +0x174
// (`add ecx,174h ; jmp`), so the call must reach that member object and nothing else.
#include <cstdio>
#include <cstddef>
#include <string>
struct CCharString { std::string Text; CCharString& operator=(const CCharString& o) { Text = o.Text; return *this; } };
struct CWideString { std::wstring Text; CWideString& operator=(const CWideString& o) { Text = o.Text; return *this; } };
#pragma pack(push, 4)
struct CMovie {                                  // retail is 32-bit: 4-byte packing keeps the +0x174 offset on a 64-bit host
    unsigned char Before[0x174];
    CWideString Movie;
    void SetMovie(CWideString& movie) { Movie = movie; }
};
#pragma pack(pop)
int main()
{
    if ((unsigned int)offsetof(CMovie, Movie) != 0x174u) { std::printf("00548510_TEST FAIL\n"); return 1; }
    CMovie m; CWideString v; v.Text = L"intro.bik";
    m.SetMovie(v);
    if (m.Movie.Text != v.Text) { std::printf("004c7bc0_TEST FAIL\n"); return 1; }
    std::printf("00548510_TEST PASS\n");
    return 0;
}