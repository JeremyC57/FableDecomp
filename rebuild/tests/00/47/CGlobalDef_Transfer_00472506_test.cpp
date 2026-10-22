// behaviour test for CGlobalDef::Transfer (0x00472506): every member is handed to the typed transfer once, in order
#include <cstdio>
struct CCharString { void* m_pData; };
struct CEngineGraphic { unsigned char _body[0xe]; };
struct CConversationCameraMap { void* h[3]; };
struct CCharStringVector { void* a[3]; };
struct CDialogueLayerDefVector { void* a[3]; };
static const void* g_log[80]; static int g_type[80]; static int g_n = 0;
struct CPersistContext {
    void Transfer(long& v) { g_log[g_n] = &v; g_type[g_n++] = 1; }
    void Transfer(float& v) { g_log[g_n] = &v; g_type[g_n++] = 2; }
    void Transfer(CCharString& v) { g_log[g_n] = &v; g_type[g_n++] = 3; }
    void Transfer(CEngineGraphic& v) { g_log[g_n] = &v; g_type[g_n++] = 4; }
    void Transfer(CConversationCameraMap& v) { g_log[g_n] = &v; g_type[g_n++] = 5; }
    void Transfer(CCharStringVector& v) { g_log[g_n] = &v; g_type[g_n++] = 6; }
    void Transfer(CDialogueLayerDefVector& v) { g_log[g_n] = &v; g_type[g_n++] = 7; }
};
struct CGlobalDef {
    long a0, a1, a2, a3, a4, a5, a6; float f7; long a8, a9, a10, a11; float f12; long a13; float f14; long a15, a16, a17, a18, a19, a20;
    CCharString s21, s22, s23; long a24, a25, a26, a27; CCharString s28; long a29, a30, a31, a32, a33, a34; float f35; long a36; float f37; long a38;
    CEngineGraphic g39, g40; CConversationCameraMap m41; CCharStringVector v42; CDialogueLayerDefVector v43;
    long a44, a45, a46, a47, a48, a49; float f50, f51, f52; long a53; float f54; long a55; float f56, f57;
    void Transfer(CPersistContext& c)
    {
        c.Transfer(a0); c.Transfer(a1); c.Transfer(a2); c.Transfer(a3); c.Transfer(a4); c.Transfer(a5); c.Transfer(a6); c.Transfer(f7);
        c.Transfer(a8); c.Transfer(a9); c.Transfer(a10); c.Transfer(a11); c.Transfer(f12); c.Transfer(a13); c.Transfer(f14); c.Transfer(a15);
        c.Transfer(a16); c.Transfer(a17); c.Transfer(a18); c.Transfer(a19); c.Transfer(a20); c.Transfer(s21); c.Transfer(s22); c.Transfer(s23);
        c.Transfer(a24); c.Transfer(a25); c.Transfer(a26); c.Transfer(a27); c.Transfer(s28); c.Transfer(a29); c.Transfer(a30); c.Transfer(a31);
        c.Transfer(a32); c.Transfer(a33); c.Transfer(a34); c.Transfer(f35); c.Transfer(a36); c.Transfer(f37); c.Transfer(a38); c.Transfer(g39);
        c.Transfer(g40); c.Transfer(m41); c.Transfer(v42); c.Transfer(v43); c.Transfer(a44); c.Transfer(a45); c.Transfer(a46); c.Transfer(a47);
        c.Transfer(a48); c.Transfer(a49); c.Transfer(f50); c.Transfer(f51); c.Transfer(f52); c.Transfer(a53); c.Transfer(f54); c.Transfer(a55);
        c.Transfer(f56); c.Transfer(f57);
    }
};
int main()
{
    static CGlobalDef d; CPersistContext c; d.Transfer(c);
    // retail type sequence (1 long, 2 float, 3 string, 4 graphic, 5 map, 6 string vector, 7 dialogue layers)
    static const int want[] = {1,1,1,1,1,1,1,2,1,1,1,1,2,1,2,1,1,1,1,1,1,3,3,3,1,1,1,1,3,1,1,1,1,1,1,2,1,2,1,4,4,5,6,7,1,1,1,1,1,1,2,2,2,1,2,1,2,2};
    if (g_n != 58) { std::printf("00472506_TEST FAIL\n"); return 1; }
    for (int i = 0; i < g_n; ++i) {
        if (g_type[i] != want[i]) { std::printf("00472506_TEST FAIL\n"); return 1; }
        if (i > 0 && (const char*)g_log[i] <= (const char*)g_log[i - 1]) { std::printf("00472506_TEST FAIL\n"); return 1; }
    }
    std::printf("00472506_TEST PASS\n");
    return 0;
}