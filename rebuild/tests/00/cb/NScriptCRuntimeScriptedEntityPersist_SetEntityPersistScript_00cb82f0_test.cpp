// Behaviour test for 00cb82f0: CRuntimeScriptedEntityPersist::SetEntityPersistScript: update an existing UID in place, else add and re-sort
#include <cstdio>
#include <cstring>
#include <new>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
class CCharString {
public:
    const char* m_pText;
    CCharString(const char* text) : m_pText(text) { }
    CCharString(const CCharString& other) : m_pText(other.m_pText) { }
    const CCharString& operator=(const CCharString& other) { m_pText = other.m_pText; return *this; }
};
namespace NScript {
class CRuntimeScriptedEntityPersist {
public:
    class CPersistInfo {
    public:
        u64         UID;
        CCharString PersistScript;
        CPersistInfo(u64 uid, const CCharString& script) : UID(uid), PersistScript(script) { }
    };
    void SetEntityPersistScript(u64 entityID, const CCharString& script);
};
}
typedef NScript::CRuntimeScriptedEntityPersist::CPersistInfo CPersistInfo;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) { }
};
struct __false_type { __false_type() { } };
template <class _Tp> class vector {                      // STLport layout: start, finish, end of storage
public:
    typedef _Tp* iterator;
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector()
    {
        for (_Tp* p = _M_start; p != _M_finish; ++p) p->~_Tp();
        operator delete(_M_start);
    }
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    size_type size() const { return (size_type)(_M_finish - _M_start); }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend)
    {
        size_type __old = size();
        size_type __len = __old + (__old > __fill_len ? __old : __fill_len);
        _Tp* __new = (_Tp*)operator new(__len * sizeof(_Tp));
        _Tp* __cur = __new;
        _Tp* __p;
        for (__p = _M_start; __p != __position; ++__p, ++__cur) new (__cur) _Tp(*__p);
        for (size_type __n = 0; __n < __fill_len; ++__n, ++__cur) new (__cur) _Tp(__x);
        if (!__atend)
            for (__p = __position; __p != _M_finish; ++__p, ++__cur) new (__cur) _Tp(*__p);
        for (__p = _M_start; __p != _M_finish; ++__p) __p->~_Tp();
        operator delete(_M_start);
        _M_start = __new;
        _M_finish = __cur;
        _M_end_of_storage = __new + __len;
    }
    void push_back(const _Tp& __x);
};
}
template <class _Tp>
void std::vector<_Tp>::push_back(const _Tp& __x)
{
    if (this->_M_finish != this->_M_end_of_storage) {
        new (this->_M_finish) _Tp(__x);
        ++this->_M_finish;
    }
    else
        _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
}
namespace std {
template <class _Tp> void SwapValues(_Tp* a, _Tp* b) { _Tp tmp(*a); *a = *b; *b = tmp; }
template <class _Iter, class _Compare> void sort(_Iter first, _Iter last, _Compare comp)   // insertion sort stand-in for 0x00CBD370
{
    if (first == last) return;
    for (_Iter i = first + 1; i != last; ++i)
        for (_Iter j = i; j != first && comp(*j, *(j - 1)); --j)
            SwapValues(j, j - 1);
}
}
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
template <class K, class V, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    typedef value_type* iterator;
    std::vector<value_type> Vector;
    C    Compare;
    bool Dirty;
    CVectorMap() : Dirty(false) { }
    iterator LowerBound(const K& key)
    {
        iterator first = Vector.begin();
        int count = (int)(Vector.end() - Vector.begin());
        while (count > 0) {
            int half = count / 2;
            iterator middle = first + half;
            if (middle->first < key) {
                first = middle + 1;
                count -= half + 1;
            }
            else
                count = half;
        }
        return first;
    }
    iterator end() { return Vector.end(); }
    iterator Find(const K& key)
    {
        iterator it = LowerBound(key);
        if (it == end() || key < it->first)
            return end();
        return it;
    }
    void Sort()
    {
        std::sort(Vector.begin(), Vector.end(), Compare);
        Dirty = false;
    }
    void Add(const K& key, const V& value)
    {
        Vector.push_back(value_type(key, value));
        Dirty = true;
        Sort();
    }
};
static CVectorMap<u64, CPersistInfo, CKeyPairCompareLess<u64, CPersistInfo> > g_persistScripts;   // the object's PersistScripts
// ---- the function under test, copied from src/00cb82f0.cpp (calling convention dropped) ----
void NScript::CRuntimeScriptedEntityPersist::SetEntityPersistScript(u64 entityID, const CCharString& script)
{
    CVectorMap<u64, CPersistInfo, CKeyPairCompareLess<u64, CPersistInfo> >::iterator it = g_persistScripts.Find(entityID);
    CPersistInfo info(entityID, script);
    if (it != g_persistScripts.end())
        it->second = info;
    else
        g_persistScripts.Add(entityID, info);
}
static int Fail() { std::printf("00cb82f0_TEST FAIL\n"); return 1; }
int main()
{
    NScript::CRuntimeScriptedEntityPersist persist;
    persist.SetEntityPersistScript(5, CCharString("five"));
    persist.SetEntityPersistScript(2, CCharString("two"));
    persist.SetEntityPersistScript(9, CCharString("nine"));
    if (g_persistScripts.Vector.size() != 3 || g_persistScripts.Dirty) return Fail();
    unsigned int i;
    for (i = 1; i < 3; ++i)
        if (g_persistScripts.Vector._M_start[i].first < g_persistScripts.Vector._M_start[i - 1].first) return Fail();
    persist.SetEntityPersistScript(5, CCharString("FIVE"));       // existing UID: updated in place
    if (g_persistScripts.Vector.size() != 3) return Fail();
    CVectorMap<u64, CPersistInfo, CKeyPairCompareLess<u64, CPersistInfo> >::iterator it = g_persistScripts.Find(5);
    if (it == g_persistScripts.end() || std::strcmp(it->second.PersistScript.m_pText, "FIVE") != 0 || it->second.UID != 5) return Fail();
    if (g_persistScripts.Find(7) != g_persistScripts.end()) return Fail();
    std::printf("00cb82f0_TEST PASS\n");
    return 0;
}