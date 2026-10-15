#include <cstdio>

struct C3DVector { float X, Y, Z; };
struct CCharString { const char* m_pData; };
static const CCharString g_FocalPointHelperName = { "focal" };
static const CCharString* g_name = 0; static long g_index = -1; static unsigned long g_flags = 0; static float g_time = 0.0f;
struct CTCGraphicAppearance {
    unsigned char m_found;
    unsigned char GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector* position, float time)
    {
        g_name = &name; g_index = index; g_flags = flags; g_time = time;
        if (m_found) { position->X = 1.0f; position->Y = 2.0f; position->Z = 3.0f; }
        return m_found;
    }
    bool GetFocalPosIfLoaded(C3DVector* position, float time)
    {
        return GetHelperPoint(g_FocalPointHelperName, 0, 2, position, time) ? true : false;
    }
};

int main()
{
    CTCGraphicAppearance a; a.m_found = 7; C3DVector p = { 0, 0, 0 };
    bool r = a.GetFocalPosIfLoaded(&p, 0.5f);
    if (!r || g_name != &g_FocalPointHelperName || g_index != 0 || g_flags != 2 || g_time != 0.5f || p.Y != 2.0f)
        { std::printf("004c11d0_TEST FAIL\n"); return 1; }
    a.m_found = 0;
    if (a.GetFocalPosIfLoaded(&p, 1.0f)) { std::printf("004c11d0_TEST FAIL\n"); return 1; }
    std::printf("004c11d0_TEST PASS\n");
    return 0;
}
