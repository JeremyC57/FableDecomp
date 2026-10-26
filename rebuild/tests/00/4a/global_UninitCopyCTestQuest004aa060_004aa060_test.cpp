// Behaviour test for 004aa060: placement-copy 28-byte CTestQuest records (six CCharString members through their copy ctor, the colour index copied)
#include <cstdio>
#include <new>

static int g_stringCopies = 0;

class CCharString {
public:
    CCharString() : PData(0) { }
    CCharString(const CCharString& other) : PData(other.PData) { ++g_stringCopies; }   // retail 0x0099EC30
    void* PData;
};

class CTestQuest { public:
    CCharString QuestName;
    CCharString HSPName;
    long        ColourIndex;
    CCharString DisplayName;
    CCharString IniFile;
    CCharString OutiFile;
    CCharString QuestCardDefName;
    CTestQuest() : ColourIndex(0) { }
};

struct FalseType { FalseType() { } };

template <class T1, class T2>
inline void Construct(T1* p, const T2& value) { new (p) T1(value); }

CTestQuest* UninitCopy(const CTestQuest* first, const CTestQuest* last, CTestQuest* result, const FalseType&)
{
    CTestQuest* cur = result;
    for (; first != last; ++first, ++cur)
        Construct(cur, *first);
    return cur;
}

int main()
{
    static char text[16];
    CTestQuest src[2];
    for (int i = 0; i < 2; ++i) {
        src[i].QuestName.PData = text + i;
        src[i].HSPName.PData = text + 2 + i;
        src[i].ColourIndex = 40 + i;
        src[i].DisplayName.PData = text + 4 + i;
        src[i].IniFile.PData = text + 6 + i;
        src[i].OutiFile.PData = text + 8 + i;
        src[i].QuestCardDefName.PData = text + 10 + i;
    }
    void* raw = ::operator new(2 * sizeof(CTestQuest));
    CTestQuest* dst = static_cast<CTestQuest*>(raw);
    CTestQuest* end = UninitCopy(src, src + 2, dst, FalseType());
    bool ok = end == dst + 2 && g_stringCopies == 12;
    for (int i = 0; i < 2; ++i) {
        ok = ok && dst[i].QuestName.PData == text + i && dst[i].HSPName.PData == text + 2 + i;
        ok = ok && dst[i].ColourIndex == 40 + i && dst[i].DisplayName.PData == text + 4 + i;
        ok = ok && dst[i].IniFile.PData == text + 6 + i && dst[i].OutiFile.PData == text + 8 + i;
        ok = ok && dst[i].QuestCardDefName.PData == text + 10 + i;
    }
    ::operator delete(raw);
    if (!ok) { std::printf("004aa060_TEST FAIL\n"); return 1; }
    std::printf("004aa060_TEST PASS\n");
    return 0;
}