// Test: CCharString::Insert(position, c) -- builds the one-character string {c, 0} and inserts it at position after taking a private copy
#include <cstdio>
#include <cstring>
struct CCharString {
    char data[32]; int copies;
    void InsertText(unsigned long position, const char* text)
    {
        size_t n = std::strlen(text), len = std::strlen(data);
        std::memmove(data + position + n, data + position, len - position + 1);
        std::memcpy(data + position, text, n);
    }
    void Insert(long position, char c)
    {
        ++copies;                                       // MakeOwnCopyOfDataOrEmptyString()
        char text[2]; text[0] = c; text[1] = 0;
        InsertText(position, text);
    }
};
int main()
{
    CCharString s; std::strcpy(s.data, "ace"); s.copies = 0;
    s.Insert(1, 'b'); s.Insert(3, 'd'); s.Insert(5, 'f');
    if (std::strcmp(s.data, "abcdef") || s.copies != 3) { std::printf("0099f3d0_TEST FAIL\n"); return 1; }
    std::printf("0099f3d0_TEST PASS\n");
    return 0;
}
