// CBankFile::GetRandomValidationValue() -- a pseudo-random bank validation word: CRC32 of the current system time (SYSTEMTIME, 16 bytes), retried until the value is above 1
// Leaf: GetRandomValidationValue
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CC0B0, length 163 bytes, static (__cdecl, plain `ret`).
// Callees: GetSystemTime (kernel32 import 0x0143FE18, held in esi). Data: the CRC32 table @ 0x0129A168 (0x77073096, ...).
// Shape: the byte loop is unrolled four times by the optimiser; each step is `table[p[i] ^ (crc & 0xff)] ^ (crc >> 8)`
// starting from 0; the retry test is unsigned (`jbe`).
struct SYSTEMTIME_ {
    unsigned short wYear, wMonth, wDayOfWeek, wDay, wHour, wMinute, wSecond, wMilliseconds;
};
extern "C" __declspec(dllimport) void __stdcall GetSystemTime(SYSTEMTIME_* time);   // import 0x0143FE18
extern const unsigned long g_crc32Table[256];      // 0x0129A168

class CBankFile {
public:
    static unsigned long GetRandomValidationValue();
};

unsigned long CBankFile::GetRandomValidationValue()
{
    unsigned long value;
    do {
        SYSTEMTIME_ time;
        GetSystemTime(&time);
        value = 0;
        const unsigned char* bytes = (const unsigned char*)&time;
        for (unsigned long i = 0; i < sizeof(time); ++i)
            value = g_crc32Table[bytes[i] ^ (value & 0xff)] ^ (value >> 8);
    } while (value <= 1);
    return value;
}
