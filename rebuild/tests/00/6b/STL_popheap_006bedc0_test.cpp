// Behaviour test for 0x006BEDC0 _STL::__pop_heap over pair<long, CMusicManager::CRegisteredMusicEntry>.
#include <cstdio>
#include <cstring>

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
class CWideString {
public:
    const wchar_t* PData;
    CWideString(const wchar_t* s) : PData(s) {}
};
class CRegisteredMusicEntry {
public:
    CWideString SrcFileName;
    CWideString DstFileName;
    float       Gain;
    float       Offset;
    bool        CacheToHD;
    CRegisteredMusicEntry(const wchar_t* s, float g) : SrcFileName(s), DstFileName(s), Gain(g), Offset(0.5f), CacheToHD(true) {}
};
namespace stlp {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) {}
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& p) : first(p.first), second(p.second) {}
    pair(const pair& p) : first(p.first), second(p.second) {}
};
}
typedef stlp::pair<long, CRegisteredMusicEntry> SPair;
struct TLess { bool operator()(const SPair& a, const SPair& b) const { return a.first < b.first; } };
static void PushHeap(SPair* first, int hole, int top, SPair val, TLess comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], val)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = val;
}
static void AdjustHeap(SPair* first, int hole, int len, SPair val, TLess comp)
{
    int top = hole; int second = 2 * hole + 2;
    while (second < len) { if (comp(first[second], first[second - 1])) --second; first[hole] = first[second]; hole = second; second = 2 * (second + 1); }
    if (second == len) { first[hole] = first[second - 1]; hole = second - 1; }
    PushHeap(first, hole, top, val, comp);
}
static void PopHeap(SPair* first, SPair* last, SPair* result, SPair val, TLess comp, int*)
{
    *result = *first;
    AdjustHeap(first, 0, int(last - first), val, comp);
}

int main()
{
    static const wchar_t w10[] = L"Music10", w20[] = L"Music20", w30[] = L"Music30", w40[] = L"Music40";
    SPair h[4] = { SPair(40, CRegisteredMusicEntry(w40, 4.0f)), SPair(30, CRegisteredMusicEntry(w30, 3.0f)),
                   SPair(20, CRegisteredMusicEntry(w20, 2.0f)), SPair(10, CRegisteredMusicEntry(w10, 1.0f)) };
    PopHeap(h, h + 3, h + 3, SPair(h[3]), TLess(), (int*)0);
    if (!(h[3].first == 40 && h[3].second.SrcFileName.PData == w40 && Near(h[3].second.Gain, 4.0f) && h[0].first == 30 && h[0].second.SrcFileName.PData == w30 && h[1].first <= h[0].first && h[2].first <= h[0].first)) {
        std::printf("006bedc0_TEST FAIL\n");
        return 1;
    }
    std::printf("006bedc0_TEST PASS\n");
    return 0;
}