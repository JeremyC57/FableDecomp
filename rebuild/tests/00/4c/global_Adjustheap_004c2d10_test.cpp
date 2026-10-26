// behaviour test for 004c2d10: _Adjust_heap over 12-byte NGraphicAppearance::CMeshInfo with CSortPrimaryMeshesLast
// (pred(a, b) = !a.PrimaryMesh, reading only its first argument); hand-traced sift-down cases, including the
// lone-child bottom case and a push back up past the start hole.
#include <cstdio>

struct CMeshInfo {
    long Index;
    float SizeX;
    unsigned char PrimaryMesh : 1;
};

class CSortPrimaryMeshesLast { public:
    bool operator()(const CMeshInfo& a, const CMeshInfo& b) const { return !a.PrimaryMesh; }
};

static void PushHeap(CMeshInfo* first, long hole, long top, CMeshInfo value, CSortPrimaryMeshesLast pred)
{
    long parent = (hole - 1) / 2;
    while (top < hole && pred(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}

static void AdjustHeap(CMeshInfo* first, long hole, long bottom, CMeshInfo value, CSortPrimaryMeshesLast pred)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < bottom; idx = 2 * idx + 2) {
        if (pred(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx], hole = idx;
    }
    if (idx == bottom) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    PushHeap(first, hole, top, value, pred);
}

static void Fill(CMeshInfo* a, long n, const unsigned char* flags)
{
    long i = 0;
    for (i = 0; i < n; ++i) {
        a[i].Index = 10 + i;
        a[i].SizeX = (float)i * 0.5f;
        a[i].PrimaryMesh = flags[i];
    }
}

static int Check(const CMeshInfo* a, long n, const long* expect)
{
    long i = 0;
    for (i = 0; i < n; ++i) {
        if (a[i].Index != expect[i])
            return 0;
        if (expect[i] != 99 && a[i].SizeX != (float)(expect[i] - 10) * 0.5f)
            return 0;
    }
    return 1;
}

int main()
{
    static const unsigned char flagsA[5] = { 0, 0, 1, 0, 1 };
    static const unsigned char flagsB[3] = { 0, 0, 0 };
    static const long expect5[5] = { 12, 11, 99, 13, 14 };
    static const long expect4[4] = { 12, 11, 99, 13 };
    static const long expect3[3] = { 99, 11, 12 };
    CMeshInfo a[5];
    CMeshInfo value;
    CSortPrimaryMeshesLast pred;
    int ok = 1;
    value.Index = 99;
    value.SizeX = 9.0f;
    value.PrimaryMesh = 1;
    Fill(a, 5, flagsA);
    AdjustHeap(a, 0, 5, value, pred);
    if (!Check(a, 5, expect5))
        ok = 0;
    Fill(a, 4, flagsA);
    AdjustHeap(a, 0, 4, value, pred);
    if (!Check(a, 4, expect4))
        ok = 0;
    Fill(a, 3, flagsB);
    AdjustHeap(a, 0, 3, value, pred);
    if (!Check(a, 3, expect3))
        ok = 0;
    if (!ok) {
        std::printf("004c2d10_TEST FAIL\n");
        return 1;
    }
    std::printf("004c2d10_TEST PASS\n");
    return 0;
}