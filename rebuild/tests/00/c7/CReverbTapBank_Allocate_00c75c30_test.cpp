#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __declspec(x)
typedef long long __int64;
#endif
struct CDspObject {
    virtual ~CDspObject();                 // 0x00C74450
    CDspObject();                          // 0x00C74420
};

struct CDspGain : CDspObject {             // vtable 0x012B9578
    float m_gain[4];                       // +0x04
    void Reset();                          // 0x00C744E0
};

struct CReverbFilterTap : CDspObject {     // vtable 0x012B958C
    CDspGain m_gain;                       // +0x04
    float* m_data;                         // +0x18
    unsigned int m_size;                   // +0x1C
    unsigned int m_pos;                    // +0x20
    unsigned int m_index;                  // +0x24
    void Allocate(unsigned int length, float** pMemory, unsigned int* pBytes, unsigned int index);   // 0x00C896C0
};

struct CReverbTapBank : CDspObject {       // vtable 0x012B9590
    CReverbFilterTap m_taps[4];            // +0x04
    unsigned int m_channels;               // +0xA4 output channels (Process mixes tap i into channel i % m_channels)
    unsigned int m_firstIndex;             // +0xA8
    unsigned int m_endIndex;               // +0xAC
    void Allocate(unsigned int channels, const unsigned int* lengths, float** pMemory, unsigned int* pBytes, unsigned int* pIndex);
};

void CReverbTapBank::Allocate(unsigned int channels, const unsigned int* lengths, float** pMemory, unsigned int* pBytes, unsigned int* pIndex)
{
    if (*pMemory)
        m_channels = channels;
    m_firstIndex = *pIndex;
    for (unsigned int i = 0; i < 4; ++i) {
        m_taps[i].Allocate(lengths[i], pMemory, pBytes, *pIndex);
        ++*pIndex;
    }
    m_endIndex = *pIndex;
}
static int fail() { std::printf("00c75c30_TEST FAIL\n"); return 1; }
static bool nearly(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-3f; }
CDspObject::CDspObject() { }
CDspObject::~CDspObject() { }
void CReverbFilterTap::Allocate(unsigned int length, float** pMemory, unsigned int* pBytes, unsigned int index)
{
    if (*pMemory) { m_data = *pMemory; m_size = length; m_pos = 0; m_index = index; *pMemory += length; }
    *pBytes += length * sizeof(float);
}
int main()
{
    static float pool[64];
    const unsigned int lengths[4] = { 3, 5, 7, 11 };
    CReverbTapBank bank;
    bank.m_channels = 99;
    for (int i = 0; i < 4; ++i) bank.m_taps[i].m_index = 1234;
    float* memory = 0;
    unsigned int bytes = 0, index = 5;
    bank.Allocate(2, lengths, &memory, &bytes, &index);          // sizing pass
    if (bytes != 26 * sizeof(float) || index != 9 || bank.m_firstIndex != 5 || bank.m_endIndex != 9) return fail();
    if (bank.m_channels != 99 || memory != 0) return fail();       // nothing kept without memory
    memory = pool; bytes = 0; index = 1;
    bank.Allocate(2, lengths, &memory, &bytes, &index);          // memory pass
    if (bank.m_channels != 2 || memory != pool + 26 || bytes != 26 * sizeof(float)) return fail();
    if (bank.m_firstIndex != 1 || bank.m_endIndex != 5 || index != 5) return fail();
    float* expect = pool;
    for (unsigned int i = 0; i < 4; ++i) {
        if (bank.m_taps[i].m_index != 1 + i || bank.m_taps[i].m_data != expect || bank.m_taps[i].m_size != lengths[i]) return fail();
        expect += lengths[i];
    }
    std::printf("00c75c30_TEST PASS\n");
    return 0;
}