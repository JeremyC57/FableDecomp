#include <cstdio>
#include <cstring>
struct CDateAndTime {
    long Year; unsigned long Month, Day, Hour, Minute, Second, Millisecond;
};
struct CDataOutputStream {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {}
    virtual void Write(const void* data, long length) = 0;
    void* NewLine_PStringData;
    void WriteLong(long value) { int v = (int)value; Write(&v, 4); }
    void WriteUByte(unsigned char value) { Write(&value, 1); }
    void WriteUWord(unsigned short value) { Write(&value, 2); }
    void WriteDateAndTime(const CDateAndTime& date);
};
void CDataOutputStream::WriteDateAndTime(const CDateAndTime& date)
{
    WriteLong(date.Year);
    WriteUByte((unsigned char)date.Month);
    WriteUByte((unsigned char)date.Day);
    WriteUByte((unsigned char)date.Hour);
    WriteUByte((unsigned char)date.Minute);
    WriteUWord((unsigned short)date.Second);
    WriteUWord((unsigned short)date.Millisecond);
}
struct Recorder : CDataOutputStream {
    unsigned char buf[64]; int n; int calls; long lens[8];
    Recorder() : n(0), calls(0) {}
    virtual void Write(const void* data, long length) { std::memcpy(buf + n, data, length); n += (int)length; lens[calls++] = length; }
};
int main()
{
    CDateAndTime d = { 2005, 0x10B, 22, 13, 45, 0x12345, 999 };
    Recorder r;
    r.WriteDateAndTime(d);
    const unsigned char expect[] = { 0xD5, 0x07, 0, 0, 0x0B, 22, 13, 45, 0x45, 0x23, 0xE7, 0x03 };
    long lens[] = { 4, 1, 1, 1, 1, 2, 2 };
    bool ok = r.n == 12 && r.calls == 7 && std::memcmp(r.buf, expect, 12) == 0;
    for (int i = 0; ok && i < 7; ++i) if (r.lens[i] != lens[i]) ok = false;
    if (!ok) { std::printf("00994180_TEST FAIL\n"); return 1; }
    std::printf("00994180_TEST PASS\n");
    return 0;
}
