// Behaviour test for 004cfac0: placement-copy pair<float, CSubtitledMessage::CCameraTag> entries (a float key and six camera enums)
#include <cstdio>
#include <new>

struct CCameraTag {
    int CameraPositionPerson;
    int CameraPosition;
    int CameraFocusPerson;
    int CameraFocusPosition;
    int CameraZoomType;
    int CameraView;
};

template <class T1, class T2>
struct Pair {
    T1 first;
    T2 second;
    Pair() { }
    Pair(const Pair<T1, T2>& o) : first(o.first), second(o.second) { }
};

typedef Pair<float, CCameraTag> CCameraTagPair;

struct FalseType { FalseType() { } };

template <class T1, class T2>
inline void Construct(T1* p, const T2& value) { new (p) T1(value); }

CCameraTagPair* UninitCopy(const CCameraTagPair* first, const CCameraTagPair* last, CCameraTagPair* result, const FalseType&)
{
    CCameraTagPair* cur = result;
    for (; first != last; ++first, ++cur)
        Construct(cur, *first);
    return cur;
}

int main()
{
    CCameraTagPair src[3];
    for (int i = 0; i < 3; ++i) {
        src[i].first = 0.5f * i;
        src[i].second.CameraPositionPerson = 10 * i + 1;
        src[i].second.CameraPosition = 10 * i + 2;
        src[i].second.CameraFocusPerson = 10 * i + 3;
        src[i].second.CameraFocusPosition = 10 * i + 4;
        src[i].second.CameraZoomType = 10 * i + 5;
        src[i].second.CameraView = 10 * i + 6;
    }
    void* raw = ::operator new(3 * sizeof(CCameraTagPair));
    CCameraTagPair* dst = static_cast<CCameraTagPair*>(raw);
    CCameraTagPair* end = UninitCopy(src, src + 3, dst, FalseType());
    bool ok = end == dst + 3;
    for (int i = 0; i < 3; ++i) {
        ok = ok && dst[i].first == 0.5f * i && dst[i].second.CameraPositionPerson == 10 * i + 1;
        ok = ok && dst[i].second.CameraPosition == 10 * i + 2 && dst[i].second.CameraFocusPerson == 10 * i + 3;
        ok = ok && dst[i].second.CameraFocusPosition == 10 * i + 4 && dst[i].second.CameraZoomType == 10 * i + 5;
        ok = ok && dst[i].second.CameraView == 10 * i + 6;
    }
    ok = ok && UninitCopy(src, src, dst, FalseType()) == dst;
    ::operator delete(raw);
    if (!ok) { std::printf("004cfac0_TEST FAIL\n"); return 1; }
    std::printf("004cfac0_TEST PASS\n");
    return 0;
}