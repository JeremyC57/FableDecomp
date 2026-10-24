#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif
typedef int LONG32;   // the retail 4-byte long (the g++ host is LP64)
#define TEST_ADDR "008269a0"
#define CHECK(c) do { if (!(c)) { std::printf("%s_TEST FAIL (line %d)\n", TEST_ADDR, __LINE__); return 1; } } while (0)

class CASuspendableProcess;
class CAFile {
public:
    unsigned char data[64]; long pos; int seeks;
    void Read(void* buffer, LONG32 size, CASuspendableProcess*) { std::memcpy(buffer, data + pos, size); pos += size; }
    void SetPosition(long position) { pos = position; ++seeks; }
    long GetPosition() { return pos; }
    void ReadVar(int* value, CASuspendableProcess* p) { Read(value, 4, p); }
    void ReadVar(unsigned char* value, CASuspendableProcess* p) { Read(value, 1, p); }
};
struct SRecord {
    unsigned int ByteLength;
    unsigned char SerialiseVersionNumber;
    unsigned char Payload[7];
};
bool ReadFromFile(SRecord* self, CAFile& file, CASuspendableProcess* process)
{
    int length;
    file.ReadVar(&length, process);
    long position = file.GetPosition();
    unsigned char version;
    file.ReadVar(&version, process);
    file.SetPosition(position);
    self->ByteLength = length;
    file.Read(&self->SerialiseVersionNumber, length - 4, process);
    return true;
}
int main()
{
    CAFile f; f.pos = 0; f.seeks = 0;
    unsigned char bytes[12] = { 12, 0, 0, 0, 1, 'a', 'b', 'c', 'd', 'e', 'f', 'g' };
    std::memcpy(f.data, bytes, 12);
    SRecord r; std::memset(&r, 0, sizeof(r));
    CHECK(ReadFromFile(&r, f, 0));
    CHECK(r.ByteLength == 12 && r.SerialiseVersionNumber == 1 && r.Payload[0] == 'a' && r.Payload[6] == 'g' && f.pos == 12 && f.seeks == 1);
    std::printf("008269a0_TEST PASS\n");
    return 0;
}