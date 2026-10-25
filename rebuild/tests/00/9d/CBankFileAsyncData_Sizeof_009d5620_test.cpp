// Link test for 0x009D5620: the source object's CBankFileAsyncData::Sizeof returns the internal buffer size plus 0x48.
#include <cstdio>
class CBankFileAsyncData {
public:
    unsigned char _pad00[0x24];
    unsigned long InternalAllocatedBufferSize;   // +0x24
    unsigned char _pad28[0x20];
    unsigned long Sizeof() const;                // from the source object
};
int main()
{
    CBankFileAsyncData d;
    d.InternalAllocatedBufferSize = 0;
    unsigned long a = d.Sizeof();
    d.InternalAllocatedBufferSize = 0x1000;
    unsigned long b = d.Sizeof();
    if (sizeof(CBankFileAsyncData) != 0x48 || a != 0x48 || b != 0x1048) { std::printf("009d5620_TEST FAIL\n"); return 1; }
    std::printf("009d5620_TEST PASS\n");
    return 0;
}
