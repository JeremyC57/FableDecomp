// Behaviour test for 0x00A00950 CCRC::Calc: table-driven reflected CRC-32 (the standard 0xEDB88320 table at 0x0129A168) with
// a zero seed and no final inversion; an empty buffer gives 0.
#include <cstdio>

class CCRC {
public:
    static unsigned long Table[256];
    static void BuildTable()
    {
        for (unsigned long i = 0; i < 256; ++i) {
            unsigned long c = i;
            for (int k = 0; k < 8; ++k)
                c = (c & 1) ? ((c >> 1) ^ 0xEDB88320UL) : (c >> 1);
            Table[i] = c;
        }
    }
    static unsigned long Calc(const void* data, unsigned long length)
    {
        unsigned long crc = 0;
        if (length == 0)
            return 0;
        const unsigned char* bytes = (const unsigned char*)data;
        for (unsigned long i = 0; i < length; ++i)
            crc = Table[bytes[i] ^ (crc & 0xFF)] ^ (crc >> 8);
        return crc;
    }
};
unsigned long CCRC::Table[256];

int main()
{
    CCRC::BuildTable();
    const char text[] = "123456789";
    // Bit-by-bit reference of the same zero-seed, non-inverted reflected CRC-32.
    unsigned long ref = 0;
    for (int i = 0; i < 9; ++i) {
        ref ^= (unsigned char)text[i];
        for (int k = 0; k < 8; ++k)
            ref = (ref & 1) ? ((ref >> 1) ^ 0xEDB88320UL) : (ref >> 1);
    }
    unsigned long crc = CCRC::Calc(text, 9);
    unsigned char zero = 0;
    bool ok = crc == ref && crc != 0 && CCRC::Calc(text, 0) == 0 && CCRC::Calc(&zero, 1) == 0
           && CCRC::Calc("\x01", 1) == CCRC::Table[1] && (crc & 0xFFFFFFFFUL) == crc;
    if (!ok) { std::printf("00a00950_TEST FAIL\n"); return 1; }
    std::printf("00a00950_TEST PASS\n");
    return 0;
}