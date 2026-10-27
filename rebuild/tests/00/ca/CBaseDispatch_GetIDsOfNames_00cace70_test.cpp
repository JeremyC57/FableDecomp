// Behaviour test for 0x00CACE70 (CBaseDispatch::GetIDsOfNames): GetTypeInfo then ITypeInfo::GetIDsOfNames and Release.
// Checks: a failing GetTypeInfo HRESULT is returned untouched with no type-info calls; on success the names, count and
// out array are forwarded, the type info is released once, and the forwarded HRESULT (even a failure) is returned.
#include <cstdio>
typedef int HRESULT;
typedef unsigned short OLECHAR;
typedef unsigned int UINT;
typedef int LCID;
typedef int DISPID;
struct IID { int id; };
static int g_releases = 0, g_calls = 0;
static HRESULT g_lookupResult = 0;
struct ITypeInfo {
    HRESULT GetIDsOfNames(OLECHAR** names, UINT count, DISPID* ids)
    { ++g_calls; for (UINT i = 0; i < count; ++i) ids[i] = 100 + (int)names[i][0]; return g_lookupResult; }
    void Release() { ++g_releases; }
};
static ITypeInfo g_ti;
class CBaseDispatch {
public:
    HRESULT m_typeInfoResult;
    int m_lastLcid;
    const IID* m_lastIid;
    HRESULT GetTypeInfo(const IID& riid, UINT itinfo, LCID lcid, ITypeInfo** pptinfo)
    {
        m_lastIid = &riid; m_lastLcid = lcid;
        if (itinfo != 0) return (HRESULT)0x8002000B;
        if (m_typeInfoResult < 0) return m_typeInfoResult;
        *pptinfo = &g_ti;
        return 0;
    }
    HRESULT GetIDsOfNames(const IID& riid, OLECHAR** rgszNames, UINT cNames, LCID lcid, DISPID* rgdispid);
};
HRESULT CBaseDispatch::GetIDsOfNames(const IID& riid, OLECHAR** rgszNames, UINT cNames, LCID lcid, DISPID* rgdispid)
{
    ITypeInfo* pti;
    HRESULT hr = GetTypeInfo(riid, 0, lcid, &pti);
    if (hr >= 0) {
        hr = pti->GetIDsOfNames(rgszNames, cNames, rgdispid);
        pti->Release();
    }
    return hr;
}
static int fail(int line) { std::printf("00cace70_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)
int main()
{
    CBaseDispatch d;
    IID iid = { 1 };
    static OLECHAR n1[] = { 'a', 0 }, n2[] = { 'b', 0 };
    OLECHAR* names[2] = { n1, n2 };
    DISPID ids[2] = { 0, 0 };
    d.m_typeInfoResult = (HRESULT)0x80004005;
    CHECK(d.GetIDsOfNames(iid, names, 2, 7, ids) == (HRESULT)0x80004005 && g_calls == 0 && g_releases == 0);
    CHECK(d.m_lastIid == &iid && d.m_lastLcid == 7);
    d.m_typeInfoResult = 0;
    CHECK(d.GetIDsOfNames(iid, names, 2, 9, ids) == 0 && g_calls == 1 && g_releases == 1);
    CHECK(ids[0] == 100 + 'a' && ids[1] == 100 + 'b' && d.m_lastLcid == 9);
    g_lookupResult = (HRESULT)0x80020006;
    CHECK(d.GetIDsOfNames(iid, names, 1, 9, ids) == (HRESULT)0x80020006 && g_calls == 2 && g_releases == 2);
    std::printf("00cace70_TEST PASS\n");
    return 0;
}