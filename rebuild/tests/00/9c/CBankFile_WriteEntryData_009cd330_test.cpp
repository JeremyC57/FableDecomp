// Behaviour test for CBankFile::WriteEntryData (0x009CD330): reopen once (position kept), seek to
// the entry, copy the source bytes, then append the zero padding.
#include <cstdio>
#include <cstring>

struct CDiskFile {
    unsigned char data[64]; unsigned int pos; int opens; bool writable;
    unsigned int GetPosition() { return pos; }
    void Close() {}
    void Open() { ++opens; writable = true; }
    void Seek(unsigned int p) { pos = p; }
    void Write(const void* d, unsigned int n) { std::memcpy(data + pos, d, n); pos += n; }
};
class CDataInputStream { public:
    const unsigned char* src;
    void CopyTo(CDiskFile* out, unsigned int size) { out->Write(src, size); src += size; }
};
class CBankFile { public:
    CDiskFile* BankFile_Data; bool WrittenToFile;
    void MakeWritable()
    {
        if (!WrittenToFile) {
            unsigned int position = BankFile_Data->GetPosition();
            BankFile_Data->Close(); BankFile_Data->Open(); BankFile_Data->Seek(position);
            WrittenToFile = true;
        }
    }
    void WriteZeros(unsigned int count)
    {
        MakeWritable();
        if (count) { unsigned char zeros[32]; std::memset(zeros, 0, sizeof(zeros)); BankFile_Data->Write(zeros, count); }
    }
    void WriteEntryData(unsigned int position, CDataInputStream& source, unsigned int size, unsigned int padding)
    {
        MakeWritable();
        BankFile_Data->Seek(position);
        source.CopyTo(BankFile_Data, size);
        WriteZeros(padding);
    }
};

int main()
{
    CDiskFile f; std::memset(f.data, 0xEE, sizeof(f.data)); f.pos = 7; f.opens = 0; f.writable = false;
    CBankFile b = { &f, false };
    static const unsigned char payload[4] = { 1, 2, 3, 4 };
    CDataInputStream in = { payload };
    b.WriteEntryData(8, in, 4, 3);
    if (f.opens != 1 || !b.WrittenToFile || f.data[8] != 1 || f.data[11] != 4 || f.data[12] != 0 || f.data[14] != 0 || f.data[15] != 0xEE || f.pos != 15) {
        std::printf("009cd330_TEST FAIL\n"); return 1;
    }
    b.WriteEntryData(20, in, 0, 0);
    if (f.opens != 1 || f.pos != 20) { std::printf("009cd330_TEST FAIL\n"); return 1; }
    std::printf("009cd330_TEST PASS\n");
    return 0;
}