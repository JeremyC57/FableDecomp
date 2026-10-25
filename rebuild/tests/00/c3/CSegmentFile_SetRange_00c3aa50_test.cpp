#include <cstdio>
struct CSegmentFile {
    unsigned long size, first, count, cursor, badSeek;
    unsigned long GetSize() const { return size; }
    void ClearRange() { first = count = cursor = 0; }
    bool Seek(unsigned long offset) { return offset != badSeek; }
    long SetRange(unsigned long f, unsigned long c)
    {
        if (f > GetSize()) { ClearRange(); return -1; }
        if (f + c > GetSize()) { ClearRange(); return -1; }
        if (!Seek(f)) { ClearRange(); return -1; }
        count = c; first = f; cursor = 0; return 0;
    }
};
static bool Run()
{
    CSegmentFile r; r.size = 10; r.badSeek = 7; r.first = 1; r.count = 1; r.cursor = 5;
    if (r.SetRange(2, 3) != 0 || r.first != 2 || r.count != 3 || r.cursor != 0) return false;
    if (r.SetRange(11, 0) != -1 || r.first != 0 || r.count != 0) return false;
    if (r.SetRange(5, 6) != -1) return false;
    return r.SetRange(7, 1) == -1 && r.SetRange(9, 1) == 0;
}
int main()
{
    if (!Run()) { std::printf("00c3aa50_TEST FAIL\n"); return 1; }
    std::printf("00c3aa50_TEST PASS\n");
    return 0;
}
