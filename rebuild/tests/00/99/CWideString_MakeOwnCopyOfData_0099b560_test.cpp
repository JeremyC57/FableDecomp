#include <cstdio>
#define FAIL { std::printf("0099b560_TEST FAIL\n"); return 1; }
struct CWideStringData {
    unsigned short* _M_start; unsigned short* _M_finish; unsigned short* _M_end_of_storage; long NoRefs;
    CWideStringData(const unsigned short* s) : _M_start(0), _M_finish(0), _M_end_of_storage(0), NoRefs(1)
    {
        unsigned long n = 0; while (s[n] != 0) ++n;
        _M_start = new unsigned short[n + 1]; for (unsigned long i = 0; i <= n; ++i) _M_start[i] = s[i];
        _M_finish = _M_start + n; _M_end_of_storage = _M_finish + 1;
    }
};
struct CWideString {
    CWideStringData* PStringData;
    void UnassignString() { if (PStringData) { if (--PStringData->NoRefs <= 0) { delete[] PStringData->_M_start; delete PStringData; } PStringData = 0; } }
    void MakeOwnCopyOfData()
    {
        CWideStringData* data = PStringData;
        if (data != 0 && data->NoRefs > 1) {
            const unsigned short* text = data->_M_start;
            CWideStringData* copy = new CWideStringData(text);
            UnassignString();
            PStringData = copy;
        }
    }
};
int main()
{
    unsigned short text[] = { 'x', 'y', 0 };
    CWideStringData* shared = new CWideStringData(text); shared->NoRefs = 2;
    CWideString a, b; a.PStringData = shared; b.PStringData = shared;
    a.MakeOwnCopyOfData();
    if (a.PStringData == shared || a.PStringData->NoRefs != 1 || shared->NoRefs != 1 || a.PStringData->_M_start[1] != 'y') FAIL;
    CWideStringData* own = a.PStringData;
    a.MakeOwnCopyOfData();
    if (a.PStringData != own) FAIL;
    CWideString e; e.PStringData = 0; e.MakeOwnCopyOfData();
    if (e.PStringData != 0) FAIL;
    std::printf("0099b560_TEST PASS\n");
    return 0;
}