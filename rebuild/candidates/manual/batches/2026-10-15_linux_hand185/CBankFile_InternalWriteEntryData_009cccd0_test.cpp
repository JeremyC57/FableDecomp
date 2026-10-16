// Behaviour test for 009cccd0: CBankFile::InternalWriteEntryData -- reopen once, seek to offset+start, write, pad + clear pending at the end.
#include <cstdio>
struct TFile {
    unsigned long pos; int opens, closes, writes; unsigned long lastSize;
    TFile() : pos(100), opens(0), closes(0), writes(0), lastSize(0) {}
    void Open() { ++opens; } void Close() { ++closes; } void Seek(unsigned long p) { pos = p; }
    unsigned long GetPosition() { return pos; } void Write(const void*, unsigned long s) { ++writes; lastSize = s; pos += s; }
};
struct TUpdate { bool pending; };
struct TBank {
    TFile* file; bool WrittenToFile; unsigned long Alignment; unsigned long sizes[4], offsets[4]; TUpdate* UpdateData[4];
    unsigned long padded; int pads;
    void WritePadding(unsigned long n) { padded = n; ++pads; }
    void InternalWriteEntryData(unsigned long index, unsigned long start, const void* data, unsigned long size)
    {
        unsigned long entrySize = sizes[index], entryOffset = offsets[index];
        if (!WrittenToFile) { unsigned long p = file->GetPosition(); file->Close(); file->Open(); file->Seek(p); WrittenToFile = true; }
        file->Seek(entryOffset + start);
        file->Write(data, size);
        if (start + size == entrySize) { WritePadding((entrySize + Alignment - 1) / Alignment * Alignment - entrySize); UpdateData[index]->pending = false; }
    }
};
int main()
{
    TFile f; TUpdate u; u.pending = true;
    TBank b; b.file = &f; b.WrittenToFile = false; b.Alignment = 16; b.sizes[1] = 20; b.offsets[1] = 64; b.UpdateData[1] = &u; b.pads = 0; b.padded = 0;
    char buf[20];
    b.InternalWriteEntryData(1, 0, buf, 10);
    if (f.opens != 1 || f.closes != 1 || f.pos != 74 || b.pads != 0 || !u.pending) { std::printf("009cccd0_TEST FAIL\n"); return 1; }
    b.InternalWriteEntryData(1, 10, buf, 10);
    if (f.opens != 1 || f.pos != 84 || b.pads != 1 || b.padded != 12 || u.pending) { std::printf("009cccd0_TEST FAIL\n"); return 1; }
    std::printf("009cccd0_TEST PASS\n");
    return 0;
}
