#include <cstdio>
// CBasePin::QueryAccept: E_POINTER for null; CheckMediaType failures map to S_FALSE; successes pass through.
struct MT { int verdict; };
struct Pin {
    int CheckMediaType(const MT* mt) { return mt->verdict; }
    int QueryAccept(const MT* pmt)
    {
        if (pmt == 0) return (int)0x80004003;
        int hr = CheckMediaType(pmt);
        if (hr < 0) return 1;
        return hr;
    }
};
int main()
{
    Pin p;
    MT ok = { 0 }, no = { (int)0x80070057 }, sf = { 1 };
    if (p.QueryAccept(0) != (int)0x80004003 || p.QueryAccept(&ok) != 0 || p.QueryAccept(&no) != 1 || p.QueryAccept(&sf) != 1) {
        std::printf("00ca84c0_TEST FAIL\n"); return 1;
    }
    std::printf("00ca84c0_TEST PASS\n");
    return 0;
}