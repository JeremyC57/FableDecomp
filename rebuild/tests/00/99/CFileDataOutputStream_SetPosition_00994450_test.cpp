// Behaviour test for CFileDataOutputStream::SetPosition 0x00994450: the stream's position change goes to its file (the file's own
// SetPosition virtual), with the position unchanged.
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
    CDataStream& base = s;
    base.SetPosition(1234);
    if (f.Pos != 1234) { std::printf("00994450_TEST FAIL\n"); return 1; }
    base.SetPosition(0);
    if (f.Pos != 0) { std::printf("00994450_TEST FAIL\n"); return 1; }
    std::printf("00994450_TEST PASS\n");
    return 0;
}