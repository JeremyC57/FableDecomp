// Test for 0x009BC7D0 CXMVPlaybackInfo::CXMVPlaybackInfo: only LazyInstallerNeedsUnpause is initialised (false); the rest is left as found.
#include <stdio.h>
#include <string.h>
#include <new>
class CXMVPlaybackInfo {
public:
    CXMVPlaybackInfo() : LazyInstallerNeedsUnpause(false) { }
    void* PDecoder; void* PDraw; void* PShow; bool SurfaceChanged; unsigned int Flags; bool LazyInstallerNeedsUnpause;
};
int main()
{
    static unsigned char buf[sizeof(CXMVPlaybackInfo)];
    memset(buf, 0xAB, sizeof(buf));
    CXMVPlaybackInfo* p = new (buf) CXMVPlaybackInfo;
    bool ok = p->LazyInstallerNeedsUnpause == false && p->Flags == 0xABABABABu;
    puts(ok ? "009bc7d0_TEST PASS" : "009bc7d0_TEST FAIL");
    return ok ? 0 : 1;
}