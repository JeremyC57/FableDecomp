#include <cstdio>
struct CASoundBank { int id; };
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
struct CSoundBankRegistry {
    CBankMap Banks;
    CASoundBank* GetBank(long id)
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
    CSoundBankRegistry r; r.Banks._M_start = e; r.Banks._M_finish = e + 2;
    if (r.GetBank(10) != &a || r.GetBank(20) != &b || r.GetBank(15) != 0 || r.GetBank(5) != 0 || r.GetBank(25) != 0)
    { std::printf("00991840_TEST FAIL\n"); return 1; }
    std::printf("00991840_TEST PASS\n");
    return 0;
}