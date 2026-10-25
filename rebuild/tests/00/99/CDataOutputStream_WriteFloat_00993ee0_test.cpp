// Behaviour test for 0x00993EE0 CDataOutputStream::WriteFloat: the value's bytes reach the virtual Write unchanged.
#include <cstdio>
#include <cstring>
class CDataOutputStream {
public:
    virtual ~CDataOutputStream();
    virtual void SetPosition(unsigned long position) = 0;
    virtual unsigned long GetPosition() const = 0;
    virtual void Close() = 0;
    virtual void Write(const void* data, long size) = 0;
    void WriteFloat(float value);
};
CDataOutputStream::~CDataOutputStream() { }
class CTestStream : public CDataOutputStream {
public:
    unsigned char Buffer[16];
    long Size;
    long Calls;
    CTestStream() : Size(0), Calls(0) { std::memset(Buffer, 0xCC, sizeof(Buffer)); }
    virtual void SetPosition(unsigned long) { }
    virtual unsigned long GetPosition() const { return (unsigned long)Size; }
    virtual void Close() { }
    virtual void Write(const void* data, long size) { ++Calls; Size = size; std::memcpy(Buffer, data, size); }
};
int main()
{
    static const unsigned char expect[] = { 0x00,0x00,0xC0,0x3F };
    CTestStream s;
    s.WriteFloat(1.5f);
    if (s.Calls != 1 || s.Size != (long)sizeof(expect) || std::memcmp(s.Buffer, expect, sizeof(expect)) != 0 || s.Buffer[sizeof(expect)] != 0xCC) {
        std::printf("00993ee0_TEST FAIL calls=%ld size=%ld\n", s.Calls, s.Size);
        return 1;
    }
    std::printf("00993ee0_TEST PASS\n");
    return 0;
}
