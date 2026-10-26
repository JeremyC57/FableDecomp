// Behaviour test for CAnimationData::CanUnload (0x00A25A70): a pending read decides alone; otherwise
// any shared (count > 1) animation blocks the unload.
#include <cstdio>

class CBankFileAsyncData { public: bool finished; unsigned char IsFinished() { return finished ? 1 : 0; } };
struct CPtr { void* obj; unsigned int* count; bool IsShared() const { return count && *count > 1; } };

static bool CanUnload(CBankFileAsyncData* file, const CPtr& shared, const CPtr* anims, int n)
{
    if (!file) {
        if (shared.IsShared()) return false;
        for (int i = 0; i != n; ++i)
            if (anims[i].IsShared()) return false;
        return true;
    }
    return file->IsFinished() ? true : false;
}

int main()
{
    unsigned int one = 1, two = 2;
    CPtr none = { 0, 0 }, uniq = { 0, &one }, shared = { 0, &two };
    CPtr anims[3] = { uniq, none, uniq };
    CBankFileAsyncData f; f.finished = false;
    if (CanUnload(&f, shared, anims, 3)) { std::printf("00a25a70_TEST FAIL\n"); return 1; }
    f.finished = true;
    if (!CanUnload(&f, shared, anims, 3)) { std::printf("00a25a70_TEST FAIL\n"); return 1; }
    if (!CanUnload(0, uniq, anims, 3)) { std::printf("00a25a70_TEST FAIL\n"); return 1; }
    if (CanUnload(0, shared, anims, 3)) { std::printf("00a25a70_TEST FAIL\n"); return 1; }
    anims[1] = shared;
    if (CanUnload(0, none, anims, 3)) { std::printf("00a25a70_TEST FAIL\n"); return 1; }
    std::printf("00a25a70_TEST PASS\n");
    return 0;
}