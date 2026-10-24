#include <cstdio>

// Model of 0x004014A0 CCRC::Calc: table-driven reflected CRC-32 folded into `seed` (no pre/post inversion);
// checked against a bitwise reference with the same polynomial table.
static unsigned int g_CRCTable[256];

static void BuildTable()
{
    for (unsigned int n = 0; n < 256; ++n) {
        unsigned int c = n;
        for (int k = 0; k < 8; ++k)
            c = (c & 1) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
        g_CRCTable[n] = c;
    }
}

struct CCRC {
    static unsigned int Calc(unsigned int seed, const void* data, unsigned int byteCount)
    {
        if (byteCount != 0) {
            const unsigned char* bytes = static_cast<const unsigned char*>(data);
            for (unsigned int index = 0; index < byteCount; ++index)
                seed = g_CRCTable[bytes[index] ^ (seed & 0xFF)] ^ (seed >> 8);
        }
        return seed;
    }
};

static unsigned int Reference(unsigned int crc, const unsigned char* p, unsigned int n)
{
    for (unsigned int i = 0; i < n; ++i) {
        crc ^= p[i];
        for (int k = 0; k < 8; ++k)
            crc = (crc & 1) ? (0xEDB88320u ^ (crc >> 1)) : (crc >> 1);
    }
    return crc;
}

int main()
{
    BuildTable();
    const unsigned char text[] = "123456789";
    if (CCRC::Calc(0xFFFFFFFFu, text, 9) != (0xCBF43926u ^ 0xFFFFFFFFu) ||   // standard CRC-32 before the final xor
        CCRC::Calc(0, text, 9) != Reference(0, text, 9) ||
        CCRC::Calc(0x1234, text, 0) != 0x1234 ||
        CCRC::Calc(CCRC::Calc(7, text, 4), text + 4, 5) != CCRC::Calc(7, text, 9)) {
        std::printf("004014a0_TEST FAIL\n");
        return 1;
    }
    std::printf("004014a0_TEST PASS\n");
    return 0;
}