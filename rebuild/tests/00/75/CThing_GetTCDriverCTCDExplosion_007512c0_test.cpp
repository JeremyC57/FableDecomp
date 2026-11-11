// Behaviour test for CThing::GetTCDriver<CTCDExplosion> @ 0x007512C0: true (and the driver pointer) only with interface bit 65 set.
#include <cstdio>

class CTCDriverBase;
class CTCDExplosion;

class CThing { public:
    void*          __vftable;
    unsigned char  _pad04[0x20 - 0x04];
    unsigned long  UsingComponents[9];
    unsigned char  _pad44[0x5c - 0x44];
    CTCDriverBase* PTCDriver;
    template <class T> bool GetTCDriver(T** out);
};

int main()
{
    CThing t;
    for (int i = 0; i < (int)sizeof(t); ++i) ((char*)&t)[i] = 0;
    int mk = 12345;
    t.PTCDriver = (CTCDriverBase*)&mk;

    CTCDExplosion* out = 0;

    // bit 65 clear -> false, out untouched
    if (t.GetTCDriver(&out)) { printf("FAIL1\n"); return 0; }
    if (out != 0) { printf("FAIL2\n"); return 0; }

    // bit 65 set -> true, out = PTCDriver
    t.UsingComponents[2] = 2;
    if (!t.GetTCDriver(&out)) { printf("FAIL3\n"); return 0; }
    if ((void*)out != (void*)&mk) { printf("FAIL4\n"); return 0; }

    // every other bit of the word set -> false
    t.UsingComponents[2] = ~2UL;
    out = 0;
    if (t.GetTCDriver(&out)) { printf("FAIL5\n"); return 0; }

    printf("007512c0_TEST PASS\n");
    return 0;
}
