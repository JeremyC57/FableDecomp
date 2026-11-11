// Behaviour test for 0x00C400D0 LookupDirectionFilterPair(long table, float azimuth, float elevation, first, second)
#include <stdio.h>

struct SDirectionFilter { short m_coefficients[25]; unsigned short m_delay; };
struct CStaticRecord_0143bc18 { long m_count; const SDirectionFilter* m_pFirst; const SDirectionFilter* m_pLast; };
static SDirectionFilter g_tableFirst[400];
static SDirectionFilter g_tableLast[400];
static CStaticRecord_0143bc18 g_record_0143bc18 = { 1, g_tableFirst, g_tableLast };
static float g_lastAzimuth;
static long LookupAzimuthElevation(float azimuth, float elevation)   // stub: records the (folded) azimuth
{
    g_lastAzimuth = azimuth;
    return (long)(azimuth + 180.0f) + (long)elevation;
}

static void LookupDirectionFilterPair(long table, float azimuth, float elevation, const SDirectionFilter** first,
                                      const SDirectionFilter** second)   // the row's body
{
    const SDirectionFilter* filters;
    switch (table) {
    case 1:
        filters = g_record_0143bc18.m_pLast;
        break;
    case 8:
    case 64:
        if (azimuth > 90.0f)
            azimuth = 180.0f - azimuth;
        else if (azimuth < -90.0f)
            azimuth = -180.0f - azimuth;
    case 4:
        filters = g_record_0143bc18.m_pFirst;
        break;
    default:
        return;
    }
    long index = LookupAzimuthElevation(azimuth, elevation);
    if (azimuth >= 0.0f) {
        *first = &filters[index];
        *second = &filters[index + 1];
    } else {
        *second = &filters[index];
        *first = &filters[index + 1];
    }
}

int main()
{
    int ok = 1;
    const SDirectionFilter* a = 0;
    const SDirectionFilter* b = 0;
    LookupDirectionFilterPair(1, 10.0f, 0.0f, &a, &b);
    ok &= a == &g_tableLast[190] && b == &g_tableLast[191];
    LookupDirectionFilterPair(4, 120.0f, 0.0f, &a, &b);              // mode 4: no folding
    ok &= g_lastAzimuth == 120.0f && a == &g_tableFirst[300] && b == &g_tableFirst[301];
    LookupDirectionFilterPair(8, 120.0f, 0.0f, &a, &b);              // folded to 60
    ok &= g_lastAzimuth == 60.0f && a == &g_tableFirst[240] && b == &g_tableFirst[241];
    LookupDirectionFilterPair(64, -150.0f, 2.0f, &a, &b);            // folded to -30: negative, swapped
    ok &= g_lastAzimuth == -30.0f && b == &g_tableFirst[152] && a == &g_tableFirst[153];
    a = b = 0;
    LookupDirectionFilterPair(2, 10.0f, 0.0f, &a, &b);               // unknown mode: untouched
    ok &= a == 0 && b == 0;
    printf(ok ? "00c400d0_TEST PASS\n" : "00c400d0_TEST FAIL\n");
    return ok ? 0 : 1;
}