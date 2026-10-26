// Behaviour test for 0x00747890 CTCStealth::GetNumberOfAttackersOverMaximum (copy of the source logic; a working vector model).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
template <class T> struct CRetailVector {
    T* first; T* last; T* end;
    CRetailVector() : first(0), last(0), end(0) {}
    ~CRetailVector() { if (first) std::free(first); }
    unsigned int size() const { return (unsigned int)(last - first); }
    void reserve(unsigned int count)
    {
        if ((unsigned int)(end - first) >= count) return;
        unsigned int n = size();
        T* p = (T*)std::malloc((count ? count : 1) * sizeof(T));
        if (n) std::memcpy(p, first, n * sizeof(T));
        if (first) std::free(first);
        first = p; last = p + n; end = p + (count ? count : 1);
    }
    void InsertGrow(T*, const T& value, unsigned long, bool) { reserve(size() * 2 + 1); *last++ = value; }
    void push_back(const T& value)
    {
        if (last != end) { new (last) T(value); ++last; }
        else InsertGrow(last, value, 1, true);
    }
};
class CThing { public: int id; };
struct CBaseIntelligentPointer { CThing* m_pItem; const CThing* GetPItem() const { return m_pItem; } };
struct CDangerStatus { long Status; long Timer; };
struct SDangerThing { CBaseIntelligentPointer first; CDangerStatus second; };
static long g_maxNumberOfAttackers = -1;
class CTCStealth { public:
    CRetailVector<SDangerThing> DangerThings;
    long MaxNumberOfDangerThings;
    long RegionBasedMaxNumberOfDangerThings;
    long GetNumberOfAttackersOverMaximum(CRetailVector<const CThing*>* attackers) const;
};
long CTCStealth::GetNumberOfAttackersOverMaximum(CRetailVector<const CThing*>* attackers) const
{
    CRetailVector<const CThing*> localAttackers;
    if (!attackers)
        attackers = &localAttackers;
    attackers->reserve(DangerThings.size());
    for (SDangerThing* it = DangerThings.first; it != DangerThings.last; ++it) {
        if (it->second.Status == 3 || it->second.Status == 2)
            attackers->push_back(it->first.GetPItem());
    }
    long count = attackers->size();
    long maximum = -1;
    if (g_maxNumberOfAttackers != -1)
        maximum = g_maxNumberOfAttackers;
    if (MaxNumberOfDangerThings != -1 && (maximum == -1 || MaxNumberOfDangerThings < maximum))
        maximum = MaxNumberOfDangerThings;
    if (RegionBasedMaxNumberOfDangerThings != -1 && (maximum == -1 || RegionBasedMaxNumberOfDangerThings < maximum))
        maximum = RegionBasedMaxNumberOfDangerThings;
    if (maximum != -1 && count > maximum)
        return count - maximum;
    return -1;
}
static int fail() { std::printf("00747890_TEST FAIL\n"); return 1; }
int main()
{
    CThing t[5];
    CTCStealth s; s.MaxNumberOfDangerThings = -1; s.RegionBasedMaxNumberOfDangerThings = -1;
    s.DangerThings.reserve(5);
    long statuses[5] = { 3, 1, 2, 2, 0 };
    for (int i = 0; i < 5; ++i) { SDangerThing d; d.first.m_pItem = &t[i]; d.second.Status = statuses[i]; d.second.Timer = 0; s.DangerThings.push_back(d); }
    if (s.GetNumberOfAttackersOverMaximum(0) != -1) return fail();           // no limit
    s.MaxNumberOfDangerThings = 1;
    CRetailVector<const CThing*> out;
    if (s.GetNumberOfAttackersOverMaximum(&out) != 2) return fail();         // 3 attackers, limit 1
    if (out.size() != 3 || out.first[0] != &t[0] || out.first[1] != &t[2] || out.first[2] != &t[3]) return fail();
    g_maxNumberOfAttackers = 5; s.RegionBasedMaxNumberOfDangerThings = 2;     // smallest limit wins: 1
    if (s.GetNumberOfAttackersOverMaximum(0) != 2) return fail();
    s.MaxNumberOfDangerThings = -1;                                          // now 2
    if (s.GetNumberOfAttackersOverMaximum(0) != 1) return fail();
    s.RegionBasedMaxNumberOfDangerThings = -1;                               // global 5: not exceeded
    if (s.GetNumberOfAttackersOverMaximum(0) != -1) return fail();
    std::printf("00747890_TEST PASS\n");
    return 0;
}