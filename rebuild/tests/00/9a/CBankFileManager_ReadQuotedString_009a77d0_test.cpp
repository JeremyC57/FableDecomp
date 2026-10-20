#include <cstdio>
#include <cstring>
struct Str { char buf[64]; int len; Str() : len(0) { buf[0] = 0; } void add(char c) { buf[len++] = c; buf[len] = 0; } };
struct Stream {
    const char* data; unsigned long pos, size;
    char ReadChar() { char c = 0; if (pos < size) c = data[pos++]; return c; }
    Str ReadUntilCharacter(char t, bool* found)
    {
        Str r;
        for (;;) {
            if (pos == size) { *found = false; break; }
            char c = ReadChar();
            if (c == t) { --pos; *found = true; break; }
            r.add(c);
        }
        return r;
    }
};
static bool ReadQuotedString(Str* out, Stream* stream)
{
    bool found;
    stream->ReadUntilCharacter('"', &found);
    if (found) {
        stream->ReadChar();
        *out = stream->ReadUntilCharacter('"', &found);
        if (found) stream->ReadChar();
        return true;
    }
    return false;
}
int main()
{
    Stream s = { "key = \"bank.big\" rest", 0, 21 };
    Str out;
    if (!ReadQuotedString(&out, &s) || std::strcmp(out.buf, "bank.big") || s.pos != 16) { std::printf("009a77d0_TEST FAIL\n"); return 1; }
    Stream t = { "noquote", 0, 7 };
    if (ReadQuotedString(&out, &t) || std::strcmp(out.buf, "bank.big")) { std::printf("009a77d0_TEST FAIL\n"); return 1; }
    std::printf("009a77d0_TEST PASS\n");
    return 0;
}