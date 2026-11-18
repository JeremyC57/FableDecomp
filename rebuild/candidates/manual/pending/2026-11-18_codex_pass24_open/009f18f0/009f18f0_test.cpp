#include <cassert>
#include <cstdio>
#include <cstring>

#ifndef _MSC_VER
typedef long long __int64;
#endif

class CGameEventDataPackage
{
public:
    unsigned char Data[32];
    unsigned long Length;
    unsigned long StartPos;

    void GetData(long no_bytes, void* area);

};

// The selected implementation is reproduced verbatim for this standalone fixture.
void CGameEventDataPackage::GetData(long no_bytes, void* area)
{
    unsigned long read_pos = StartPos;
    unsigned char* byte_pointer = (unsigned char*)area;
    long bytes_to_read;
    if (read_pos + no_bytes > sizeof(Data))
    {
        bytes_to_read = sizeof(Data) - read_pos;
        __int64 overflow_check = read_pos + bytes_to_read;
        if (overflow_check < 0 || overflow_check > 0x7fffffff)
            return;
        memcpy(byte_pointer, &Data[read_pos], bytes_to_read);
        memcpy(byte_pointer + bytes_to_read, Data, no_bytes - bytes_to_read);
        StartPos = no_bytes - bytes_to_read;
    }
    else
    {
        memcpy(byte_pointer, &Data[read_pos], no_bytes);
        StartPos = (read_pos + no_bytes) % sizeof(Data);
    }
    Length -= no_bytes;
}

int main()
{
    for (unsigned start = 0; start < 32; ++start) {
        for (unsigned count = 0; count <= 32; ++count) {
            CGameEventDataPackage packet;
            packet.Length = 32;
            packet.StartPos = start;
            for (unsigned i = 0; i < 32; ++i)
                packet.Data[i] = static_cast<unsigned char>(i + 10);
            unsigned char out[34];
            std::memset(out, 0xfe, sizeof out);
            packet.GetData(count, out + 1);
            assert(out[0] == 0xfe && out[count + 1] == 0xfe);
            assert(packet.StartPos == (start + count) % 32);
            assert(packet.Length == 32 - count);
            for (unsigned i = 0; i < count; ++i)
                assert(out[i + 1] == (start + i) % 32 + 10);
            for (unsigned i = 0; i < 32; ++i)
                assert(packet.Data[i] == i + 10);
        }
    }
    std::puts("009f18f0_TEST PASS");
    return 0;
}
