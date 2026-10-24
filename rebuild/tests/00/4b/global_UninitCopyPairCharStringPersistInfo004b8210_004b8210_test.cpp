// Behaviour test for 004b8210: placement-copy pair<CCharString, CRuntimeQuestPersistence::CPersistInfo> entries (three CCharString copies each)
#include <cstdio>
#include <new>

static int g_stringCopies = 0;

class CCharString {
public:
    CCharString() : PData(0) { }
    CCharString(const CCharString& other) : PData(other.PData) { ++g_stringCopies; }   // retail 0x0099EC30
    void* PData;
};

template <class T1, class T2>
struct Pair {
    T1 first;
    T2 second;
    Pair() { }
    Pair(const Pair<T1, T2>& o) : first(o.first), second(o.second) { }
};

struct CPersistInfo {
    CCharString QuestName;
    CCharString PersistScript;
};

typedef Pair<CCharString, CPersistInfo> SPersistPair;

struct FalseType { FalseType() { } };

template <class T1, class T2>
inline void Construct(T1* p, const T2& value) { new (p) T1(value); }

SPersistPair* UninitCopy(const SPersistPair* first, const SPersistPair* last, SPersistPair* result, const FalseType&)
{
    SPersistPair* cur = result;
    for (; first != last; ++first, ++cur)
        Construct(cur, *first);
    return cur;
}

int main()
{
    static char text[16];
    SPersistPair src[3];
    for (int i = 0; i < 3; ++i) {
        src[i].first.PData = text + i;
        src[i].second.QuestName.PData = text + 4 + i;
        src[i].second.PersistScript.PData = text + 8 + i;
    }
    void* raw = ::operator new(3 * sizeof(SPersistPair));
    SPersistPair* dst = static_cast<SPersistPair*>(raw);
    SPersistPair* end = UninitCopy(src, src + 3, dst, FalseType());
    bool ok = end == dst + 3 && g_stringCopies == 9;
    for (int i = 0; i < 3; ++i)
        ok = ok && dst[i].first.PData == text + i && dst[i].second.QuestName.PData == text + 4 + i
                && dst[i].second.PersistScript.PData == text + 8 + i;
    ::operator delete(raw);
    if (!ok) { std::printf("004b8210_TEST FAIL\n"); return 1; }
    std::printf("004b8210_TEST PASS\n");
    return 0;
}