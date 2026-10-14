#include <stdio.h>
static const void* g_data; static unsigned long g_size; static long g_flags; static int g_calls;
struct CAFile {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {}
    virtual void Write(const void* data, unsigned long size, long flags) { g_data = data; g_size = size; g_flags = flags; ++g_calls; }
};
struct CBinaryFileDataWriter { virtual bool Write(const void* data, unsigned long size); CAFile* File; };
bool CBinaryFileDataWriter::Write(const void* data, unsigned long size) { File->Write(data, size, 0); return true; }
int main()
{
    CAFile f; CBinaryFileDataWriter w; w.File = &f; char buf[4];
    bool r = w.Write(buf, 4);
    bool ok = r && g_calls == 1 && g_data == buf && g_size == 4 && g_flags == 0;
    if (!ok) { printf("00418ef7_TEST FAIL\n"); return 1; }
    printf("00418ef7_TEST PASS\n"); return 0;
}
