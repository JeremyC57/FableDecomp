// Behaviour test for 0x00BF8430 (CLandscapeBackgroundSubNodeSplitBranch::ProcessBackgroundLoading): status merge of two branch loads.
#include <cstdio>
enum LS_LOADING_STATUS { LS_LOADED = 1, LS_LOADING = 2, LS_FAILED = 3 };
struct Node {
    LS_LOADING_STATUS status; int calls;
    LS_LOADING_STATUS Process(double, bool, void*, bool, bool) { ++calls; return status; }
};
struct Split {
    Node LeftBranch, RightBranch;
    LS_LOADING_STATUS Process(double t, bool f, void* b, bool i, bool l)
    {
        LS_LOADING_STATUS left = LeftBranch.Process(t, f, b, i, l);
        if (left != LS_FAILED)
        {
            LS_LOADING_STATUS right = RightBranch.Process(t, f, b, i, l);
            if (right != LS_FAILED)
            {
                if (left == LS_LOADING || right == LS_LOADING)
                    return LS_LOADING;
                return LS_LOADED;
            }
        }
        return LS_FAILED;
    }
};
static int Run(int a, int b, int* rc)
{
    Split s; s.LeftBranch.status = (LS_LOADING_STATUS)a; s.RightBranch.status = (LS_LOADING_STATUS)b;
    s.LeftBranch.calls = 0; s.RightBranch.calls = 0;
    int r = s.Process(1.0, false, 0, false, false);
    *rc = s.RightBranch.calls;
    return r;
}
int main()
{
    int rc;
    int ok = 1;
    ok &= Run(3, 1, &rc) == 3 && rc == 0;
    ok &= Run(1, 3, &rc) == 3 && rc == 1;
    ok &= Run(2, 3, &rc) == 3;
    ok &= Run(2, 1, &rc) == 2;
    ok &= Run(1, 2, &rc) == 2;
    ok &= Run(1, 1, &rc) == 1;
    if (!ok) { std::printf("00bf8430_TEST FAIL\n"); return 1; }
    std::printf("00bf8430_TEST PASS\n");
    return 0;
}
