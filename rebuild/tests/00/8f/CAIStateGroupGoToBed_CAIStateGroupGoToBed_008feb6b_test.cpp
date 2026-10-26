#include <cstdio>
#include <cstddef>
// Behaviour model of CAIStateGroup_GoToBed::CAIStateGroup_GoToBed @ 0x008FEB6B: the base and member
// constructors run in declaration order and the member offsets match the retail layout.
static int g_order[4]; static int g_n = 0;
class CAIStateGroupBase { public: virtual void v00() {} CAIStateGroupBase() { g_order[g_n++] = 1; } };
struct CBaseIntelligentPointer { virtual void v00() {} void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { g_order[g_n++] = 2; } };
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: virtual void v00() {} CIntelligentPointer() : CBaseIntelligentPointer() {} };
class CCharString { public: void* m_pData; CCharString() : m_pData(0) { g_order[g_n++] = 3; } };
class CThing;
struct CAIStateGroup_GoToBed : CAIStateGroupBase {
    unsigned char _base[0x10];
    CIntelligentPointer<CThing> PBed;
    long m_sleepTimeOffset, m_bedTime, m_wakeTime, m_pad28, m_state;
    CCharString m_bedMarkerName;
    virtual void v00() {}
    CAIStateGroup_GoToBed() : CAIStateGroupBase(), PBed(), m_bedMarkerName() {}
};
int main()
{
    CAIStateGroup_GoToBed g;
    bool ok = g_n == 3 && g_order[0] == 1 && g_order[1] == 2 && g_order[2] == 3
        && g.PBed.m_pItem == 0 && g.m_bedMarkerName.m_pData == 0;
    if (sizeof(void*) == 4) ok = ok && offsetof(CAIStateGroup_GoToBed, PBed) == 0x14 && offsetof(CAIStateGroup_GoToBed, m_bedMarkerName) == 0x30;
    if (!ok) { std::printf("008feb6b_TEST FAIL\n"); return 1; }
    std::printf("008feb6b_TEST PASS\n");
    return 0;
}