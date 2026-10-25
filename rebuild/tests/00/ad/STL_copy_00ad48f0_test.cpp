// Behaviour test for std::__copy(set<unsigned long>::const_iterator, const_iterator, insert_iterator<set<unsigned long> >, const input_iterator_tag&, int*) @ 0x00AD48F0
// Input-iterator copy: `for (; first != last; ++result, ++first) *result = *first; return result;` into an inserter.
#include <cstdio>
#include <set>

typedef std::set<unsigned int> TIndexSet;

struct CSetInserter {
    TIndexSet* container;
    TIndexSet::iterator iter;
    CSetInserter(TIndexSet& c, TIndexSet::iterator i) : container(&c), iter(i) { }
    CSetInserter& operator=(const unsigned int& value)
    {
        iter = container->insert(iter, value);
        ++iter;
        return *this;
    }
    CSetInserter& operator*() { return *this; }
    CSetInserter& operator++() { return *this; }
};

static CSetInserter CopyRange(TIndexSet::const_iterator first, TIndexSet::const_iterator last, CSetInserter result)
{
    for ( ; first != last; ++result, ++first)
        *result = *first;
    return result;
}

int main()
{
    TIndexSet src;
    for (unsigned int i = 1; i <= 6; ++i) src.insert(i * 7);
    TIndexSet dst;
    dst.insert(3);
    dst.insert(100);
    CSetInserter out = CopyRange(src.begin(), src.end(), CSetInserter(dst, dst.begin()));
    int ok = dst.size() == 8 && out.container == &dst && *out.iter == 100;  // the hint ends just after the last inserted value (42)
    if (ok) {
        TIndexSet::iterator it = out.iter;
        --it;
        if (*it != 42) ok = 0;
    }
    for (unsigned int i = 1; i <= 6; ++i) if (dst.find(i * 7) == dst.end()) ok = 0;
    CSetInserter none = CopyRange(src.begin(), src.begin(), CSetInserter(dst, dst.end()));   // empty range
    if (none.iter != dst.end() || dst.size() != 8) ok = 0;
    if (!ok) { std::printf("00ad48f0_TEST FAIL\n"); return 1; }
    std::printf("00ad48f0_TEST PASS\n");
    return 0;
}