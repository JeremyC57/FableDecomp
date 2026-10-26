// Behaviour test for 0x0098DB20 CIndexedArray<CCharString>::push_back: the map receives (item, old size) and the
// array grows by one copy of the item; an existing key keeps its first index (map insert does not overwrite).
#include <cstdio>
#include <cstring>
#include <new>
class CCharString { public:
    char text[16];
    CCharString(const char* s) { std::strncpy(text, s, 15); text[15] = 0; }
    CCharString(const CCharString& o) { std::memcpy(text, o.text, 16); }
};
template <class T1, class T2> struct pair { T1 first; T2 second; pair(const T1& a, const T2& b) : first(a), second(b) { } };
struct Map {
    const char* keys[8]; long values[8]; int count;
    bool insert(const pair<const CCharString, long>& v)
    {
        for (int i = 0; i < count; ++i) if (std::strcmp(keys[i], v.first.text) == 0) return false;
        static char store[8][16];
        std::memcpy(store[count], v.first.text, 16);
        keys[count] = store[count]; values[count] = v.second; ++count; return true;
    }
};
struct Arr {
    CCharString* _M_start; CCharString* _M_finish; CCharString* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void push_back(const CCharString& x) { if (_M_finish != _M_end_of_storage) { new (_M_finish) CCharString(x); ++_M_finish; } }
};
struct CIndexedArray {
    Arr Array; Map IndexMap;
    void push_back(const CCharString& item)
    {
        IndexMap.insert(pair<const CCharString, long>(item, Array.size()));
        Array.push_back(item);
    }
};
int main()
{
    int fails = 0;
    unsigned char raw[4 * sizeof(CCharString)];
    CIndexedArray a; a.IndexMap.count = 0;
    a.Array._M_start = a.Array._M_finish = (CCharString*)raw; a.Array._M_end_of_storage = a.Array._M_start + 4;
    a.push_back(CCharString("alpha"));
    a.push_back(CCharString("beta"));
    a.push_back(CCharString("alpha"));
    if (a.Array.size() != 3 || a.IndexMap.count != 2) ++fails;
    if (a.IndexMap.values[0] != 0 || a.IndexMap.values[1] != 1) ++fails;
    if (std::strcmp(a.Array._M_start[1].text, "beta") != 0 || std::strcmp(a.Array._M_start[2].text, "alpha") != 0) ++fails;
    std::printf(fails ? "0098db20_TEST FAIL\n" : "0098db20_TEST PASS\n");
    return fails ? 1 : 0;
}