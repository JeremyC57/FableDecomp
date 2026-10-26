#include <cstdio>
#include <cstring>
class CMemoryDataInputStream { public:
    unsigned long m_position; const unsigned char* m_pData; long m_remaining;
    void Read(void* buffer, long size)
    {
        long long newPosition = (long long)(m_position + size);
        if (newPosition < 0 || newPosition > 0x7fffffff) return;
        if (m_remaining >= size) { std::memcpy(buffer, m_pData, size); m_pData += size; m_remaining -= size; m_position += size; }
        else std::memset(buffer, 0, size);
    }
};
template <class T> void TransferIn(CMemoryDataInputStream& s, T& out) { T v; s.Read(&v, sizeof(v)); out = v; }
class CDefString { public: int TablePos; void TransferBinaryIn(CMemoryDataInputStream* s) { TransferIn(*s, TablePos); } };
class CCombatAbilityData { public:
    int PercentageChance; float SecondsDuration; float SecondsToWaitBeforeRepeat; CDefString Anim;
    void TransferBinaryIn(CMemoryDataInputStream& stream, CCombatAbilityData&)
    {
        TransferIn(stream, PercentageChance); TransferIn(stream, SecondsDuration);
        TransferIn(stream, SecondsToWaitBeforeRepeat); Anim.TransferBinaryIn(&stream);
    }
};
int main()
{
    unsigned char buf[16]; int pc = 35; float d = 1.5f, w = 4.0f; int pos = 9;
    std::memcpy(buf, &pc, 4); std::memcpy(buf + 4, &d, 4); std::memcpy(buf + 8, &w, 4); std::memcpy(buf + 12, &pos, 4);
    CMemoryDataInputStream s = { 0, buf, 16 };
    CCombatAbilityData a; a.TransferBinaryIn(s, a);
    if (a.PercentageChance != 35 || a.SecondsDuration != 1.5f || a.SecondsToWaitBeforeRepeat != 4.0f || a.Anim.TablePos != 9 || s.m_position != 16 || s.m_remaining != 0) { std::printf("0044fadc_TEST FAIL\n"); return 1; }
    std::printf("0044fadc_TEST PASS\n");
    return 0;
}