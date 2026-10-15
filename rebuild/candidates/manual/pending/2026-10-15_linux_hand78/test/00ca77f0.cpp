#include <cstdio>
typedef unsigned short WCHAR_T;
static int g_addRefs = 0;
static WCHAR_T* CopyN(WCHAR_T* d, const WCHAR_T* s, int n) { int i = 0; for (; i < n - 1 && s[i]; ++i) d[i] = s[i]; d[i] = 0; return d; }
struct IFilterGraphLike { virtual unsigned long AddRef() { return ++g_addRefs; } virtual ~IFilterGraphLike() {} };
struct FILTER_INFO_Local { WCHAR_T achName[128]; IFilterGraphLike* pGraph; };
struct CBaseFilter {
    WCHAR_T* m_pName; IFilterGraphLike* m_pGraph;
    int QueryFilterInfo(FILTER_INFO_Local* info)
    {
        if (info == 0) return (int)0x80004003L;
        if (m_pName) CopyN(info->achName, m_pName, 128); else info->achName[0] = 0;
        info->pGraph = m_pGraph;
        if (m_pGraph) m_pGraph->AddRef();
        return 0;
    }
};
int main()
{
    WCHAR_T name[] = { 'T', 'e', 'x', 0 };
    IFilterGraphLike graph;
    CBaseFilter f; f.m_pName = name; f.m_pGraph = &graph;
    FILTER_INFO_Local info; info.achName[0] = 'x'; info.pGraph = 0;
    if (f.QueryFilterInfo(0) != (int)0x80004003L) { std::printf("00ca77f0_TEST FAIL\n"); return 1; }
    if (f.QueryFilterInfo(&info) != 0 || info.achName[0] != 'T' || info.achName[2] != 'x' || info.achName[3] != 0 || info.pGraph != &graph || g_addRefs != 1) { std::printf("00ca77f0_TEST FAIL\n"); return 1; }
    f.m_pName = 0; f.m_pGraph = 0;
    if (f.QueryFilterInfo(&info) != 0 || info.achName[0] != 0 || info.pGraph != 0 || g_addRefs != 1) { std::printf("00ca77f0_TEST FAIL\n"); return 1; }
    std::printf("00ca77f0_TEST PASS\n");
    return 0;
}
