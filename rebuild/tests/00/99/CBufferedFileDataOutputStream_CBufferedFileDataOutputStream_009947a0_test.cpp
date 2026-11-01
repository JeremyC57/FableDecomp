// Behaviour test for CBufferedFileDataOutputStream::~CBufferedFileDataOutputStream (0x009947A0): when bytes are buffered, seek to
// the buffer's file position, write them, seek back to the end and empty the buffer; restart at FilePos; free the buffer.
#include <cstdio>
#include <cstring>
struct CFileModel {
    long Pos; long Written; long WriteAt; long Seeks; unsigned char Data[16];
    void SetPosition(long p) { Pos = p; ++Seeks; }
    void Write(const void* data, long size, void* process) { if (process) return; std::memcpy(Data, data, size); Written = size; WriteAt = Pos; }
};
static int g_freed = 0;
struct CStreamModel {
    CFileModel* File; unsigned char* Buffer; long BufferSize; long BufferDataSize; long DataStartFilePtr; long FileSize; long FilePos;
    void Flush()
    {
        if (BufferDataSize > 0) {
            File->SetPosition(DataStartFilePtr);
            File->Write(Buffer, BufferDataSize, 0);
            File->SetPosition(FileSize);
            BufferDataSize = 0;
        }
        DataStartFilePtr = FilePos;
    }
    ~CStreamModel() { Flush(); if (Buffer) ++g_freed; delete[] Buffer; }
};
int main()
{
    CFileModel f;
    std::memset(&f, 0, sizeof(f));
    {
        CStreamModel s;
        s.File = &f; s.Buffer = new unsigned char[8]; s.BufferSize = 8; s.BufferDataSize = 3;
        s.Buffer[0] = 'a'; s.Buffer[1] = 'b'; s.Buffer[2] = 'c';
        s.DataStartFilePtr = 5; s.FileSize = 20; s.FilePos = 9;
    }
    if (f.Written != 3 || f.WriteAt != 5 || f.Pos != 20 || f.Seeks != 2 || f.Data[2] != 'c' || g_freed != 1) {
        std::printf("009947a0_TEST FAIL\n"); return 1;
    }
    {
        CStreamModel s;
        s.File = &f; s.Buffer = new unsigned char[8]; s.BufferSize = 8; s.BufferDataSize = 0;
        s.DataStartFilePtr = 0; s.FileSize = 0; s.FilePos = 0;
    }
    if (f.Seeks != 2 || g_freed != 2) { std::printf("009947a0_TEST FAIL\n"); return 1; }
    std::printf("009947a0_TEST PASS\n");
    return 0;
}