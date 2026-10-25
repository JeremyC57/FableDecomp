// Behaviour test for 00c88870: COggVorbisStream scalar deleting destructor -- destroy, then free
#include <cstdio>
static int g_dtor = 0, g_base = 0;
class IAudioStream { public: virtual ~IAudioStream() { ++g_base; } };
class COggVorbisStream : public IAudioStream { public: virtual ~COggVorbisStream() { ++g_dtor; } unsigned char _body[0x33c]; };
int main()
{
    IAudioStream* p = new COggVorbisStream;
    delete p;
    int ok = g_dtor == 1 && g_base == 1;
    std::printf(ok ? "00c88870_TEST PASS\n" : "00c88870_TEST FAIL\n");
    return ok ? 0 : 1;
}
