#include <stdio.h>
static char g_log[32]; static long g_arg[32]; static int g_n;
struct CAFile {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {}
    virtual void Write(const void* data, long size, long flags) { g_log[g_n] = 'W'; g_arg[g_n++] = size; g_log[g_n] = (char)('0' + flags); g_arg[g_n++] = (long)data; }
    virtual void Seek(long position) { g_log[g_n] = 'S'; g_arg[g_n++] = position; }
};
static int g_freed;
struct CBufferedFileDataOutputStream {
    virtual void v00() {} void* m_name; CAFile* File; unsigned char* Buffer; long BufferSize; long BufferDataSize; long DataStartFilePtr; long FileSize; long FilePos;
    virtual void v01() {} virtual void v02() {}
    virtual void Close();
};
void CBufferedFileDataOutputStream::Close()
{
    if (BufferDataSize > 0) { File->Seek(DataStartFilePtr); File->Write(Buffer, BufferDataSize, 0); File->Seek(FileSize); BufferDataSize = 0; }
    DataStartFilePtr = FilePos;
    delete[] Buffer;
    Buffer = 0;
}
int main()
{
    CAFile f; CBufferedFileDataOutputStream s; s.File = &f; s.Buffer = new unsigned char[16]; unsigned char* buf = s.Buffer; s.BufferSize = 16; s.BufferDataSize = 7; s.DataStartFilePtr = 100; s.FileSize = 200; s.FilePos = 150;
    s.Close();
    bool ok = g_n == 4 && g_log[0] == 'S' && g_arg[0] == 100 && g_log[1] == 'W' && g_arg[1] == 7 && g_arg[2] == (long)buf && g_log[3] == 'S' && g_arg[3] == 200 && s.BufferDataSize == 0 && s.DataStartFilePtr == 150 && s.Buffer == 0;
    s.Buffer = 0; s.FilePos = 175; s.Close();
    ok = ok && g_n == 4 && s.DataStartFilePtr == 175 && s.Buffer == 0;
    if (!ok) { printf("00994810_TEST FAIL\n"); return 1; }
    printf("00994810_TEST PASS\n"); return 0;
}
