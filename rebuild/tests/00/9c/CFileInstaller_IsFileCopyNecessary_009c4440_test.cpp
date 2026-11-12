// Behaviour test for CFileInstaller::IsFileCopyNecessary (0x009C4440): a copy of the model's decision logic over fake files.
#include <cstdio>
#include <cstring>
struct FakeFile { bool exists; bool openable; bool sizeReadable; unsigned __int64 size; unsigned __int64 stamp; };
struct Installer { bool ignoreTimeStamp; bool dirtyDisc; bool exitThread; int closes; };
static void SetDirtyDiscError(Installer& in, bool error)
{
    in.dirtyDisc = error;
    if (in.exitThread || error) error = true;
    in.exitThread = error;
}
static bool IsFileCopyNecessary(Installer& in, const FakeFile& src, const FakeFile& dst, bool compressed)
{
    if (!src.exists || !dst.exists) return true;
    bool srcOpen = src.openable, dstOpen = dst.openable;
    if (!srcOpen) { SetDirtyDiscError(in, true); if (dstOpen) in.closes++; return false; }
    if (!dstOpen) { in.closes++; return true; }
    if (!compressed) {
        unsigned __int64 sourceSize = src.size, destSize = dst.size;
        if (!src.sizeReadable) { in.closes += 2; SetDirtyDiscError(in, true); return false; }
        if (!dst.sizeReadable) { in.closes += 2; return true; }
        if (std::memcmp(&sourceSize, &destSize, sizeof sourceSize) != 0) { in.closes += 2; return true; }
    }
    in.closes += 2;
    if (in.ignoreTimeStamp) return false;
    unsigned __int64 sourceTime = src.stamp, destTime = dst.stamp;
    if (destTime >= sourceTime && destTime - sourceTime > 30000000) return true;
    if (sourceTime > destTime && sourceTime - destTime > 30000000) return true;
    return false;
}
int main()
{
    FakeFile a = { true, true, true, 100, 1000000000ui64 };
    FakeFile b = a;
    Installer in = { false, false, false, 0 };
    int bad = 0;
    if (IsFileCopyNecessary(in, a, b, false)) bad |= 1;               // identical
    FakeFile missing = a; missing.exists = false;
    if (!IsFileCopyNecessary(in, a, missing, false)) bad |= 2;
    FakeFile bigger = a; bigger.size = 101;
    if (!IsFileCopyNecessary(in, a, bigger, false)) bad |= 4;          // sizes differ
    if (IsFileCopyNecessary(in, a, bigger, true)) bad |= 8;            // compressed: size ignored
    FakeFile newer = a; newer.stamp += 30000001;
    if (!IsFileCopyNecessary(in, a, newer, false)) bad |= 16;
    FakeFile older = a; older.stamp -= 30000001;
    if (!IsFileCopyNecessary(in, a, older, false)) bad |= 32;
    FakeFile close = a; close.stamp += 30000000;
    if (IsFileCopyNecessary(in, a, close, false)) bad |= 64;           // exactly 3 s: not necessary
    in.ignoreTimeStamp = true;
    if (IsFileCopyNecessary(in, a, newer, false)) bad |= 128;
    in.ignoreTimeStamp = false;
    FakeFile unreadable = a; unreadable.openable = false;
    if (IsFileCopyNecessary(in, unreadable, b, false) || !in.dirtyDisc || !in.exitThread) bad |= 256;
    FakeFile noDst = a; noDst.openable = false;
    if (!IsFileCopyNecessary(in, a, noDst, false)) bad |= 512;
    if (bad) { std::printf("009c4440_TEST FAIL\n"); return 1; }
    std::printf("009c4440_TEST PASS\n");
    return 0;
}