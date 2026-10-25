#include <stdio.h>
// Behaviour test for 0x00A3A7E0 CCompressorZlib::HasStreamInput: links against the source object.
struct z_stream_s {
    const unsigned char* next_in;
    unsigned int         avail_in;
};
class CCompressorZlib {
public:
    unsigned char _pad00[0x20];
    z_stream_s    Stream;
    bool HasStreamInput() const;
};
int main()
{
    if ((char*)&((CCompressorZlib*)0)->Stream.next_in - (char*)0 != 0x20) { printf("00a3a7e0_TEST FAIL layout\n"); return 1; }
    static const unsigned char input[4] = { 1, 2, 3, 4 };
    CCompressorZlib z; z.Stream.next_in = 0; z.Stream.avail_in = 4;
    if (z.HasStreamInput()) { printf("00a3a7e0_TEST FAIL null\n"); return 1; }
    z.Stream.next_in = input; z.Stream.avail_in = 0;
    if (!z.HasStreamInput()) { printf("00a3a7e0_TEST FAIL set\n"); return 1; }
    printf("00a3a7e0_TEST PASS\n");
    return 0;
}
