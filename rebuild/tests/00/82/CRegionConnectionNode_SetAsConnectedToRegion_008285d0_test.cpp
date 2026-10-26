// Behaviour test: CRegionConnectionNode::SetAsConnectedToRegion -- connect adds the region once; disconnect removes every copy
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct CLongVector {                                    // STLport vector<long>
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    void _M_insert_overflow(long* position, const long& x)   // stand-in for 0x00440990 (append with growth)
    {
        unsigned int size = (unsigned int)(_M_finish - _M_start);
        unsigned int len = size ? size * 2 : 1;
        long* s = (long*)std::malloc(len * sizeof(long));
        unsigned int head = (unsigned int)(position - _M_start);
        if (head)
            std::memcpy(s, _M_start, head * sizeof(long));
        s[head] = x;
        if (_M_start)
            std::free(_M_start);
        _M_start = s;
        _M_finish = s + head + 1;
        _M_end_of_storage = s + len;
    }
    long* erase(long* first, long* last)                // STLport range erase over trivially assignable longs
    {
        unsigned int bytes = (unsigned int)((char*)_M_finish - (char*)last);
        long* i = first;
        if (bytes) {
            std::memmove(first, last, bytes);
            i = (long*)((char*)first + bytes);
        }
        _M_finish = i;
        return first;
    }
    void push_back(const long& x)
    {
        if (_M_finish != _M_end_of_storage) {
            *_M_finish = x;
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x);
    }
};
static long* find(long* first, long* last, const long& value)   // STLport find 0x0050E540
{
    for (; first != last; ++first)
        if (*first == value)
            return first;
    return last;
}
static long* remove(long* first, long* last, const long& value)  // STLport remove: find, then remove_copy
{
    first = find(first, last, value);
    if (first == last)
        return first;
    long* next = first;
    long* result = first;
    for (++next; next != last; ++next)
        if (!(*next == value)) {
            *result = *next;
            ++result;
        }
    return result;
}

class CRegionConnectionNode { public:
    long        RegionID;
    CLongVector Connections;
    long        MostRecentSearchID;
    long        FirstNodeInSearch;
    void SetAsConnectedToRegion(long region, bool connected)
    {
        if (connected) {
            if (find(Connections._M_start, Connections._M_finish, region) == Connections._M_finish)
                Connections.push_back(region);
        } else
            Connections.erase(remove(Connections._M_start, Connections._M_finish, region), Connections._M_finish);
    }
};

int main()
{
    CRegionConnectionNode n;
    n.RegionID = 1;
    n.Connections._M_start = n.Connections._M_finish = n.Connections._M_end_of_storage = 0;
    n.SetAsConnectedToRegion(5, true);
    n.SetAsConnectedToRegion(7, true);
    n.SetAsConnectedToRegion(5, true);                   // already there: no duplicate
    n.SetAsConnectedToRegion(9, true);
    if (n.Connections._M_finish - n.Connections._M_start != 3 || n.Connections._M_start[0] != 5 || n.Connections._M_start[1] != 7
        || n.Connections._M_start[2] != 9)
        { std::printf("008285d0_TEST FAIL\n"); return 1; }
    n.Connections._M_start[2] = 5;                       // 5, 7, 5: a disconnect removes both
    n.SetAsConnectedToRegion(5, false);
    if (n.Connections._M_finish - n.Connections._M_start != 1 || n.Connections._M_start[0] != 7)
        { std::printf("008285d0_TEST FAIL\n"); return 1; }
    n.SetAsConnectedToRegion(42, false);                 // not connected: unchanged
    if (n.Connections._M_finish - n.Connections._M_start != 1)
        { std::printf("008285d0_TEST FAIL\n"); return 1; }
    std::free(n.Connections._M_start);
    std::printf("008285d0_TEST PASS\n");
    return 0;
}