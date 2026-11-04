// Behaviour test for CFileDataOutputStream::GetPosition 0x00994460: the stream reports its file's position (the file's own GetPosition
// virtual), whatever moved the file.
#include <cstdio>
struct CAFile { virtual ~CAFile() {} virtual void SetPosition(long p) = 0; virtual long GetPosition() const = 0; };
struct CMemFile : CAFile { long Pos; CMemFile() : Pos(0) {} void SetPosition(long p) { Pos = p; } long GetPosition() const { return Pos; } };
struct CDataStream { virtual ~CDataStream() {} virtual void SetPosition(unsigned long) = 0; virtual unsigned long GetPosition() const = 0; };
struct CFileDataOutputStream : CDataStream {
    void* NewLine; CAFile* File;
    void SetPosition(unsigned long position) { File->SetPosition(position); }
    unsigned long GetPosition() const { return File->GetPosition(); }
};
int main()
{
    CMemFile f; CFileDataOutputStream s; s.File = &f;
    const CDataStream& base = s;
    f.Pos = 77;
    if (base.GetPosition() != 77u) { std::printf("00994460_TEST FAIL\n"); return 1; }
    f.SetPosition(5);
    if (base.GetPosition() != 5u) { std::printf("00994460_TEST FAIL\n"); return 1; }
    std::printf("00994460_TEST PASS\n");
    return 0;
}