#include <cstdio>
class CChecksum { public: long Checksum1, Checksum2, Checksum3, Checksum4; };
class CInputManager { public:
    virtual void v00() { }
    unsigned char _pad04[0x38];
    CChecksum Checksum;
    unsigned char _pad4c[0x10];
    long TimeStamp;
    void UpdateChecksum(long timeStamp, const CChecksum& checksum) { Checksum = checksum; TimeStamp = timeStamp; }
};
int main()
{
    CInputManager m; CChecksum c; c.Checksum1 = 1; c.Checksum2 = 2; c.Checksum3 = 3; c.Checksum4 = 4;
    m.TimeStamp = 0; m.UpdateChecksum(77, c);
    if (m.Checksum.Checksum1 != 1 || m.Checksum.Checksum2 != 2 || m.Checksum.Checksum3 != 3 || m.Checksum.Checksum4 != 4 || m.TimeStamp != 77) { std::printf("009f4a90_TEST FAIL\n"); return 1; }
    std::printf("009f4a90_TEST PASS\n");
    return 0;
}