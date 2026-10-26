#include <cstdio>
#include <cstring>
static const bool g_is32 = sizeof(void*) == 4 && sizeof(long) == 4;   // the layout checks hold for the 32-bit VC7.1 build
static char g_seq[32] = "";
class CCharString { public: void* storage_; ~CCharString() { std::strcat(g_seq, "s"); } };
class CThingGameObject { public: virtual void v00() { } unsigned char _pad04[0xcc]; ~CThingGameObject() { std::strcat(g_seq, "G"); } };
class CThingTrackNode : public CThingGameObject { public: virtual void v00() { } long m_trackWordD0; long m_trackWordD4; unsigned char NextNodeUID[8]; unsigned char PreviousNodeUID[8]; CCharString TrackName; bool IsStart; bool IsEnd; ~CThingTrackNode() { } };
int main()
{
    { CThingTrackNode n; if ((g_is32 && (char*)&n.TrackName - (char*)&n != 0xe8)) { std::printf("00836d00_TEST FAIL\n"); return 1; } }
    if (std::strcmp(g_seq, "sG")) { std::printf("00836d00_TEST FAIL\n"); return 1; }
    std::printf("00836d00_TEST PASS\n");
    return 0;
}