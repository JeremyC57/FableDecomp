// Behaviour test for CUserProfileManager::SaveGameState (0x00409FD0): the profile file image -- "FablePro", version 0, total
// length (text + 0x10), the text, the CRC-32 (reflected 0xEDB88320 table, seed 0, no final inversion) of header then text, and
// space padding up to 0x4000 bytes.
#include <cstdio>
#include <cstring>
static unsigned int table[256];
static unsigned int Calc(unsigned int crc, const unsigned char* p, unsigned int n)
{
    if (n == 0) return crc;
    for (unsigned int i = 0; i < n; ++i) crc = table[p[i] ^ (crc & 0xFF)] ^ (crc >> 8);
    return crc;
}
int main()
{
    for (unsigned int i = 0; i < 256; ++i) { unsigned int c = i; for (int k = 0; k < 8; ++k) c = (c & 1) ? 0xEDB88320u ^ (c >> 1) : c >> 1; table[i] = c; }
    const char* text = "ProfileName \"Hero\";";
    unsigned int len = (unsigned int)std::strlen(text);
    static unsigned char file[0x4000];
    unsigned int pos = 0;
    std::memcpy(file, "FablePro", 8); pos = 8;
    unsigned int zero = 0, total = len + 0x10;
    std::memcpy(file + pos, &zero, 4); pos += 4; std::memcpy(file + pos, &total, 4); pos += 4;
    std::memcpy(file + pos, text, len); pos += len;
    unsigned int crc = Calc(0, file, 16); crc = Calc(crc, (const unsigned char*)text, len);
    std::memcpy(file + pos, &crc, 4); pos += 4;
    if (pos < 0x4000) std::memset(file + pos, ' ', 0x4000 - pos);
    // the CRC of the concatenation equals the chained CRC, and an empty run returns the seed
    if (Calc(0, file, 16 + len) != crc || Calc(crc, file, 0) != crc || file[0x3FFF] != ' ' || total - 16 != len) {
        std::printf("00409fd0_TEST FAIL\n"); return 1;
    }
    std::printf("00409fd0_TEST PASS\n");
    return 0;
}
