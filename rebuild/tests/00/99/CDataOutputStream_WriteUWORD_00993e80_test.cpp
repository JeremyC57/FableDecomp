// Behaviour test for 0x00993E80 CDataOutputStream::WriteUWORD: the value's bytes reach the virtual Write unchanged.
#include <cstdio>
#include <cstring>
class CDataOutputStream {
public:
    virtual ~CDataOutputStream();
    virtual void SetPosition(unsigned long position) = 0;
    virtual unsigned long GetPosition() const = 0;
    virtual void Close() = 0;
    virtual void Write(const void* data, long size) = 0;
    void WriteUWORD(unsigned short value);
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
    static const unsigned char expect[] = { 0xEF,0xBE };
    CTestStream s;
    s.WriteUWORD((unsigned short)0xBEEF);
    if (s.Calls != 1 || s.Size != (long)sizeof(expect) || std::memcmp(s.Buffer, expect, sizeof(expect)) != 0 || s.Buffer[sizeof(expect)] != 0xCC) {
        std::printf("00993e80_TEST FAIL calls=%ld size=%ld\n", s.Calls, s.Size);
        return 1;
    }
    std::printf("00993e80_TEST PASS\n");
    return 0;
}
