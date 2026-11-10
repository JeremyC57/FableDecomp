#include <cstdio>
class CASoundBank { public: int id; };
struct SEntry { long first; CASoundBank* second; };
struct CBankMap {
    SEntry* _M_start; SEntry* _M_finish;
    const SEntry* LowerBound(const long& key) const
    {
        const SEntry* it = _M_start;
        while (it != _M_finish && it->first < key) ++it;
        return it;
    }
    const SEntry* end() const { return _M_finish; }
    const SEntry* find(const long& key) const
    {
        const SEntry* it = LowerBound(key);
        if (it == end() || key < it->first)
            return end();
        return it;
    }
};
struct CASoundManager {
    CBankMap Banks;
    CASoundBank* GetPSoundBank(long id)
    {
        const SEntry* it = Banks.find(id);
        if (it != Banks.end())
            return it->second;
        return 0;
    }
};
int main()
{
    CASoundBank a, b; a.id = 1; b.id = 2;
    SEntry e[2]; e[0].first = 10; e[0].second = &a; e[1].first = 20; e[1].second = &b;
    CASoundManager r; r.Banks._M_start = e; r.Banks._M_finish = e + 2;
    if (r.GetPSoundBank(10) != &a || r.GetPSoundBank(20) != &b || r.GetPSoundBank(15) != 0 || r.GetPSoundBank(5) != 0 || r.GetPSoundBank(25) != 0)
    { std::printf("00991840_TEST FAIL\n"); return 1; }
    std::printf("00991840_TEST PASS\n");
    return 0;
}