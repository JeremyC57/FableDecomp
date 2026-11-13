// Behaviour test for CRenderStateManager::CDeviceState::CDeviceState() (0x00A079B0): model copy of CStateInfo / CDeviceState
// (231 x 0x14-byte state slots). Fill the storage with garbage, placement-construct, then check every slot has the five
// ctor-initialised fields cleared and the untouched union / TextureStage bytes left as they were.
#include <cstdio>
#include <cstring>
#include <new>

struct CStateInfo {
    CStateInfo() : CurrentState(0), DesiredState(0), BookmarkMask(0), DirtyListFlag(false), StateType(0) {}
    unsigned long CurrentState;
    unsigned long DesiredState;
    unsigned long BookmarkMask;
    unsigned long Param;
    bool          DirtyListFlag;
    unsigned char StateType;
    unsigned char TextureStage;
};

struct CDeviceState {
    CDeviceState() {}
    CStateInfo Scalars[55];
    CStateInfo Stages[22][8];
};

int main()
{
    if (sizeof(CStateInfo) != 0x14 || sizeof(CDeviceState) != 0x120C) { std::printf("00a079b0_TEST FAIL\n"); return 1; }
    static unsigned char buf[sizeof(CDeviceState)];
    std::memset(buf, 0xAB, sizeof(buf));
    CDeviceState* s = new (buf) CDeviceState();
    const CStateInfo* e = reinterpret_cast<const CStateInfo*>(s);
    for (int i = 0; i < 231; ++i) {
        if (e[i].CurrentState != 0 || e[i].DesiredState != 0 || e[i].BookmarkMask != 0 || e[i].DirtyListFlag ||
            e[i].StateType != 0 || e[i].Param != 0xABABABABu || e[i].TextureStage != 0xAB) {
            std::printf("00a079b0_TEST FAIL\n"); return 1;
        }
    }
    if (&s->Stages[0][0] != e + 55 || &s->Stages[21][7] != e + 230) { std::printf("00a079b0_TEST FAIL\n"); return 1; }
    std::printf("00a079b0_TEST PASS\n");
    return 0;
}