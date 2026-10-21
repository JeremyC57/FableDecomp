#include <cstdio>
#include <cstring>
#include <cstdlib>
// Behaviour model of the PK serialiser string transfer at 0x00C31B90: reading fetches a length and that many bytes
// into the string (empty length -> ""), failing when either transfer fails; writing sends the string's length
// and, when non-zero, its bytes.
struct Stream {
    bool reading;
    char store[64]; unsigned int storeLen; unsigned int pos;
    bool failSize, failData;
    bool TransferSize(unsigned int* size)
    {
        if (failSize) return false;
        if (reading) { std::memcpy(size, store + pos, 4); pos += 4; }
        else { std::memcpy(store + storeLen, size, 4); storeLen += 4; }
        return true;
    }
    bool Read(char* buffer, unsigned int size)
    {
        if (failData) return false;
        std::memcpy(buffer, store + pos, size); pos += size; return true;
    }
    unsigned char Write(const char* text, unsigned int size)
    {
        if (failData) return 0;
        std::memcpy(store + storeLen, text, size); storeLen += size; return 7;
    }
};
struct Str { char text[64]; void Assign(const char* t) { std::strcpy(text, t); } };
static bool Transfer(Stream& s, Str& data)
{
    if (s.reading) {
        unsigned int size;
        if (!s.TransferSize(&size)) return false;
        if (size == 0) { data.Assign(""); return true; }
        char* buffer = (char*)std::malloc(size + 1);
        if (!s.Read(buffer, size)) { std::free(buffer); return false; }
        buffer[size] = 0;
        data.Assign(buffer);
        std::free(buffer);
        return true;
    }
    unsigned int size = (unsigned int)std::strlen(data.text);
    if (!s.TransferSize(&size)) return false;
    if (size == 0) return true;
    return s.Write(data.text, size) ? true : false;
}
int main()
{
    Stream w; std::memset(&w, 0, sizeof(w)); w.reading = false;
    Str a; a.Assign("ps_1_1 tex t0");
    bool ok = Transfer(w, a) && w.storeLen == 4 + 13;
    Stream r = w; r.reading = true; r.pos = 0;
    Str b; b.Assign("junk");
    ok = ok && Transfer(r, b) && std::strcmp(b.text, "ps_1_1 tex t0") == 0;
    // empty string: only the length goes out, and reads back as ""
    Stream we; std::memset(&we, 0, sizeof(we)); Str e; e.Assign("");
    ok = ok && Transfer(we, e) && we.storeLen == 4;
    we.reading = true; Str e2; e2.Assign("x");
    ok = ok && Transfer(we, e2) && e2.text[0] == 0;
    // failures
    Stream rf = w; rf.reading = true; rf.pos = 0; rf.failData = true; Str c; c.Assign("keep");
    ok = ok && !Transfer(rf, c) && std::strcmp(c.text, "keep") == 0;
    Stream wf; std::memset(&wf, 0, sizeof(wf)); wf.failData = true;
    ok = ok && !Transfer(wf, a);
    wf.failSize = true;
    ok = ok && !Transfer(wf, a);
    if (!ok) { std::printf("00c31b90_TEST FAIL\n"); return 1; }
    std::printf("00c31b90_TEST PASS\n");
    return 0;
}