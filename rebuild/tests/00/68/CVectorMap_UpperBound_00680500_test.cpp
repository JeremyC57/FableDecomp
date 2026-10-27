// 00680500 behaviour test for CVectorMap<EReactionSpeechType, long, ...>::UpperBound(const EReactionSpeechType&) const
#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#ifndef __fastcall
#define __fastcall
#endif
#ifndef __cdecl
#define __cdecl
#endif
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed at line %d\n", __LINE__); g_fail = 1; } } while (0)
enum EReactionSpeechType { SPEECH_A = 1, SPEECH_B = 4, SPEECH_C = 9 };
template <class A, class B> struct Pair { A first; B second; };
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const K& key, const Pair<K, V>& entry) const { return key < entry.first; }
};
template <class K, class V, class C> class CVectorMap {
public:
    typedef Pair<K, V> value_type;
    value_type* First;
    value_type* Last;
    const value_type* UpperBound(const K& key) const;
};
template <class K, class V, class C>
const typename CVectorMap<K, V, C>::value_type* CVectorMap<K, V, C>::UpperBound(const K& key) const
{
    const value_type* first = First;
    int count = (int)(Last - First);
    C comp;
    while (count > 0) {
        int half = count / 2;
        const value_type* middle = first + half;
        if (!comp(key, *middle)) {
            first = middle + 1;
            count -= half + 1;
        }
        else
            count = half;
    }
    return first;
}
static void run()
{
    Pair<EReactionSpeechType, long> e[6] = { { SPEECH_A, 10 }, { SPEECH_B, 11 }, { SPEECH_B, 12 }, { SPEECH_B, 13 }, { SPEECH_C, 14 }, { SPEECH_C, 15 } };
    CVectorMap<EReactionSpeechType, long, CKeyPairCompareLess<EReactionSpeechType, long> > m; m.First = e; m.Last = e + 6;
    CHECK(m.UpperBound(SPEECH_B) == e + 4);
    CHECK(m.UpperBound(SPEECH_A) == e + 1);
    CHECK(m.UpperBound(SPEECH_C) == e + 6);
    CHECK(m.UpperBound((EReactionSpeechType)0) == e);
    CHECK(m.UpperBound((EReactionSpeechType)5) == e + 4);
    m.Last = e;
    CHECK(m.UpperBound(SPEECH_B) == e);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("00680500_TEST FAIL\n");
        return 1;
    }
    std::printf("00680500_TEST PASS\n");
    return 0;
}