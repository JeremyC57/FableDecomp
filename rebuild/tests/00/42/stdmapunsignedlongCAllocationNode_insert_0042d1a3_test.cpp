#include <cstdio>
template <class T1, class T2> struct pair {
    T1 first; T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
    pair(const pair& o) : first(o.first), second(o.second) {}
};
struct CAllocationNode { long Size; };
struct CTreeIterator { long Index; };
// Tree stand-in: insert_unique keeps sorted-insertion-free unique keys.
struct CTree {
    unsigned long Keys[8];
    CAllocationNode* Values[8];
    long Count;
    CTree() : Count(0) {}
    pair<CTreeIterator, bool> insert_unique(const pair<const unsigned long, CAllocationNode*>& v)
    {
        CTreeIterator it;
        long i = 0;
        for (; i < Count; ++i)
            if (Keys[i] == v.first) { it.Index = i; return pair<CTreeIterator, bool>(it, false); }
        Keys[Count] = v.first; Values[Count] = v.second; it.Index = Count++;
        return pair<CTreeIterator, bool>(it, true);
    }
};
// The candidate logic: map::insert forwards to the tree's insert_unique.
struct CMap {
    typedef pair<const unsigned long, CAllocationNode*> value_type;
    CTree _M_t;
    pair<CTreeIterator, bool> insert(const value_type& v) { return _M_t.insert_unique(v); }
};
int main()
{
    CAllocationNode a = { 16 }, b = { 32 };
    CMap map;
    pair<CTreeIterator, bool> r1 = map.insert(CMap::value_type(0x1000, &a));
    pair<CTreeIterator, bool> r2 = map.insert(CMap::value_type(0x2000, &b));
    pair<CTreeIterator, bool> r3 = map.insert(CMap::value_type(0x1000, &b));
    bool ok = r1.second && r2.second && !r3.second && r3.first.Index == 0 && r2.first.Index == 1 && map._M_t.Count == 2 && map._M_t.Values[0] == &a;
    if (!ok) { std::printf("0042d1a3_TEST FAIL\n"); return 1; }
    std::printf("0042d1a3_TEST PASS\n");
    return 0;
}