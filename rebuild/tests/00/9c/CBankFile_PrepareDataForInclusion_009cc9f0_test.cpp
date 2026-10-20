#include <cstdio>
#include <cstdlib>
#include <cstring>
struct File {
    const char* name; unsigned long Length; bool opened; int closes;
    File() : name(0), Length(0), opened(false), closes(0) { }
    void Open(const char* n) { name = n; Length = (unsigned long)std::strlen(n); opened = true; }
    void Read(void* buf, long size) { std::memcpy(buf, name, size); }
    void Close() { ++closes; opened = false; }
};
struct Writer { unsigned char got[32]; unsigned long size; void Write(const void* d, unsigned long s) { std::memcpy(got, d, s); size = s; } };
static void Prepare(const char* source, Writer* writer, int* closes)
{
    File file;
    file.Open(source);
    unsigned long size = file.Length;
    unsigned char* buffer = size ? (unsigned char*)std::malloc(size) : 0;
    std::memset(buffer, 0, size);
    unsigned char dummy;
    file.Read(size ? buffer : &dummy, size);
    writer->Write(size ? buffer : &dummy, size);
    file.Close();
    std::free(buffer);
    *closes = file.closes;
}
int main()
{
    Writer w; int closes = 0;
    Prepare("bankentry.dat", &w, &closes);
    if (w.size != 13 || std::memcmp(w.got, "bankentry.dat", 13) || closes != 1) { std::printf("009cc9f0_TEST FAIL\n"); return 1; }
    std::printf("009cc9f0_TEST PASS\n");
    return 0;
}