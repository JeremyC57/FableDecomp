// Behaviour test for 0x009F1870 CGameEvent::InitFromCompressedBuffer (round trip against the CompressIntoBuffer format).
#include <cstdio>
#include <cstring>
struct CGameEvent {
    int Type; signed char Player; unsigned char Data[32]; unsigned char EndPos; bool Valid; bool Replacement;
    void Reset() { EndPos = 0; Valid = true; }
    void AddData(const unsigned char* data, int size) { std::memcpy(&Data[EndPos], data, size); EndPos = (unsigned char)(EndPos + size); }
    int InitFromCompressedBuffer(const unsigned char* buffer)
    {
        bool replacement = false;
        const unsigned char* in = buffer;
        unsigned short type = (unsigned short)(in[0] | (in[1] << 8));
        in += 2;
        signed char player = (signed char)*in++;
        unsigned char size = *in++;
        if (type & 0x8000) { replacement = true; type &= 0x7fff; }
        Type = type; Player = player; Replacement = replacement;
        Reset();
        int length = size;
        AddData(in, length);
        in += length;
        return (int)(in - buffer);
    }
};
static int Fail() { std::printf("009f1870_TEST FAIL\n"); return 1; }
int main()
{
    unsigned char buf[16] = { 0x34, 0x92, 0x03, 0x05, 'h', 'e', 'l', 'l', 'o', 0xEE };
    CGameEvent e; e.EndPos = 7; e.Valid = false; e.Replacement = false;
    int used = e.InitFromCompressedBuffer(buf);
    if (used != 9 || e.Type != 0x1234 || e.Player != 3 || !e.Replacement || !e.Valid || e.EndPos != 5 || std::memcmp(e.Data, "hello", 5) != 0) return Fail();
    unsigned char buf2[4] = { 0x10, 0x00, 0xFF, 0x00 };
    used = e.InitFromCompressedBuffer(buf2);
    if (used != 4 || e.Type != 0x10 || e.Player != -1 || e.Replacement || e.EndPos != 0) return Fail();
    std::printf("009f1870_TEST PASS\n");
    return 0;
}