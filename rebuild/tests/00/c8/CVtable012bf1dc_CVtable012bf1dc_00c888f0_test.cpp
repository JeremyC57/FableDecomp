#include <cstdio>
// CVtable_012bf1dc: the null audio stream; its constructor ignores both arguments and only installs the vtable,
// whose slots answer GetLength 0 and -1 for everything else.
struct SStreamSegment { unsigned int m_word0, m_word4, m_word8; };
class IAudioStream {
public:
    virtual ~IAudioStream() {}
    virtual int GetLength() = 0;
    virtual int Seek(unsigned int position, bool exact) = 0;
    virtual int GetRate() = 0;
};
class CNull : public IAudioStream {
public:
    CNull(void* source, const SStreamSegment& segment) {}
    int GetLength() { return 0; }
    int Seek(unsigned int position, bool exact) { return -1; }
    int GetRate() { return -1; }
};
int main()
{
    SStreamSegment seg = { 1, 2, 3 };
    CNull n((void*)&seg, seg);
    IAudioStream* s = &n;
    if (s->GetLength() != 0 || s->Seek(10, true) != -1 || s->GetRate() != -1) { std::printf("00c888f0_TEST FAIL\n"); return 1; }
    if (sizeof(CNull) != sizeof(void*)) { std::printf("00c888f0_TEST FAIL\n"); return 1; }
    std::printf("00c888f0_TEST PASS\n");
    return 0;
}