#include <cstdio>
static int g_baseBuilt = 0;
struct CBaseIntelligentPointer {
    virtual void v00() { }
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_baseBuilt; }
};
struct CIntelligentPointer_CThing : CBaseIntelligentPointer {
    virtual void v00() { }
    CIntelligentPointer_CThing() : CBaseIntelligentPointer() { }
};
struct C3DVector { float X, Y, Z; C3DVector() { } };
struct CTargetPoint {
    CIntelligentPointer_CThing PThing;
    long Index;
    C3DVector TargetingPosition;
    long FrameRejected;
    CTargetPoint() : PThing(), Index(-1), FrameRejected(-1) { }
};

int main()
{
    CTargetPoint p;
    if (g_baseBuilt != 1 || p.Index != -1 || p.FrameRejected != -1 || p.PThing.m_pItem != 0) { std::printf("004d7a76_TEST FAIL\n"); return 1; }
    CTargetPoint q;
    if (g_baseBuilt != 2 || q.Index != -1) { std::printf("004d7a76_TEST FAIL\n"); return 1; }
    std::printf("004d7a76_TEST PASS\n");
    return 0;
}
