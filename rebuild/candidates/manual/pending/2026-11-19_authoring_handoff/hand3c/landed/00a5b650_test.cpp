// Behaviour test for CCachedFile::Copy (0x00A5B650): a copy of the row's logic on stand-alone types (a counted disk-file pointer with
// a clone hook, a byte buffer), checking fields, clone-or-release of the disk file and the buffer replacement.
#include <stdio.h>
#include <vector>
#include <string>
struct File { int clones; File* Clone() { ++clones; return this; } };
struct Counted {
    File* Data; long* Info; int releases, resets;
    bool IsNull() const { return Data == 0; }
    void ReleaseData() { if (Info) { --*Info; ++releases; } Data = 0; Info = 0; }
    void Reset(File* f) { ReleaseData(); ++resets; Data = f; }
};
struct Cached {
    long Position, BufferStartPosition; std::vector<unsigned char> DataBuffer; std::wstring Pathname; Counted PDiskFile; bool Opened, BufferValid;
    void Copy(const Cached& rhs)
    {
        Position = rhs.Position; Opened = rhs.Opened; Pathname = rhs.Pathname;
        BufferStartPosition = rhs.BufferStartPosition; BufferValid = rhs.BufferValid;
        if (!rhs.PDiskFile.IsNull()) PDiskFile.Reset(rhs.PDiskFile.Data->Clone());
        else PDiskFile.ReleaseData();
        DataBuffer.clear();
        for (size_t i = 0; i < rhs.DataBuffer.size(); ++i) DataBuffer.push_back(rhs.DataBuffer[i]);
    }
};
int main()
{
    int fails = 0;
    File disk = { 0 }; long ref = 2;
    Cached a = { 7, 64, std::vector<unsigned char>(3, 9), L"data\\x.big", { &disk, 0, 0, 0 }, true, true };
    Cached b = { 0, 0, std::vector<unsigned char>(5, 1), L"", { 0, &ref, 0, 0 }, false, false };
    b.Copy(a);
    if (b.Position != 7 || b.BufferStartPosition != 64 || !b.Opened || !b.BufferValid || b.Pathname != L"data\\x.big") ++fails;
    if (b.DataBuffer.size() != 3 || b.DataBuffer[2] != 9) ++fails;
    if (b.PDiskFile.Data != &disk || disk.clones != 1 || ref != 1 || b.PDiskFile.resets != 1) ++fails;
    Cached c = { 1, 2, std::vector<unsigned char>(), L"y", { 0, 0, 0, 0 }, false, false };
    b.Copy(c);                                               // no disk file: ours is released, buffer emptied
    if (!b.PDiskFile.IsNull() || b.DataBuffer.size() != 0 || disk.clones != 1 || b.Position != 1) ++fails;
    printf(fails ? "00a5b650_TEST FAIL\n" : "00a5b650_TEST PASS\n");
    return fails ? 1 : 0;
}
