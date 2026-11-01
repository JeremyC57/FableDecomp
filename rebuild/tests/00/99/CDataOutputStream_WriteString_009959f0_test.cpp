// Behaviour test for CDataOutputStream::WriteString (0x009959F0): the characters go to the virtual Write with no prefix or terminator;
// a null string writes "" with length 0.
#include <cstdio>
#include <cstring>
class CCharStringData { public: char* PData; unsigned int StringLength; };
class CCharString {
public:
    CCharString(CCharStringData* d) : PStringData(d) { }
    const char* GetAsCharArray() const { return PStringData == 0 ? "" : PStringData->PData; }
    long GetLength() const { return PStringData == 0 ? 0 : (long)PStringData->StringLength; }
    CCharStringData* PStringData;
};
class CDataOutputStream {
public:
    virtual ~CDataOutputStream() { }
    virtual void Write(const void* data, long length) = 0;
    void WriteString(const CCharString& text)
    {
        const char* data = text.GetAsCharArray();
        Write(data, text.GetLength());
    }
};
class CRecorder : public CDataOutputStream {
public:
    CRecorder() : Used(0) { }
    virtual void Write(const void* data, long length) { std::memcpy(Buffer + Used, data, (size_t)length); Used += length; Calls++; }
    char Buffer[64];
    long Used;
    int Calls;
};
int main()
{
    CRecorder r; r.Calls = 0;
    char text[] = "Hero";
    CCharStringData d = { text, 4 };
    r.WriteString(CCharString(&d));
    r.WriteString(CCharString(0));
    r.WriteString(CCharString(&d));
    if (r.Calls != 3 || r.Used != 8 || std::memcmp(r.Buffer, "HeroHero", 8) != 0) {
        std::printf("009959f0_TEST FAIL\n");
        return 1;
    }
    std::printf("009959f0_TEST PASS\n");
    return 0;
}