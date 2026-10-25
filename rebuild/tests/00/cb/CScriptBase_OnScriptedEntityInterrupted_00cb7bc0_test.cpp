// Behaviour test for 00cb7bc0: CScriptBase::OnScriptedEntityInterrupted: only the entity script bound to that UID is flagged
#include <cstdio>
#include <cstring>
#include <new>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
class CASuspendableProcess {
public:
    virtual ~CASuspendableProcess() { }
    bool Running;
};
class CSpawnedFuncBase : public CASuspendableProcess {
public:
    bool FuncPaused;
    bool Interrupted;
    CSpawnedFuncBase() : FuncPaused(false), Interrupted(false) { }
    void SetFuncAsInterrupted(bool interrupted) { Interrupted = interrupted; }
};
class CActiveEntityScriptBase : public CSpawnedFuncBase { };
template <class T> class CCountedPointer {
public:
    T*    Data;
    void* Info;
    T* operator->() const { return Data; }
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class vector {
public:
    typedef _Tp* iterator;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
};
}
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    typedef value_type* iterator;
    std::vector<value_type> Vector;
    C    Compare;
    bool Dirty;
    iterator LowerBound(const K& key)                      // the out-of-line 0x00CB9840, a plain lower bound
    {
        iterator first = Vector.begin();
        int count = (int)(Vector.end() - Vector.begin());
        while (count > 0) {
            int half = count / 2;
            iterator middle = first + half;
            if (middle->first < key) { first = middle + 1; count -= half + 1; }
            else count = half;
        }
        return first;
    }
    iterator begin() { return Vector.begin(); }
    iterator end() { return Vector.end(); }
    iterator Find(const K& key)
    {
        iterator it = LowerBound(key);
        if (it == end() || key < it->first)
            return end();
        return it;
    }
};
typedef CVectorMap<u64, CCountedPointer<CActiveEntityScriptBase>, CKeyPairCompareLess<u64, CCountedPointer<CActiveEntityScriptBase> > > CEntityScriptMap;
class CScriptBase {
public:
    CEntityScriptMap EntityScripts;
    void OnScriptedEntityInterrupted(u64 entityID);
};
// ---- the function under test, copied from src/00cb7bc0.cpp (calling convention dropped) ----
void CScriptBase::OnScriptedEntityInterrupted(u64 entityID)
{
    CEntityScriptMap::iterator it = EntityScripts.Find(entityID);
    if (it != EntityScripts.end())
        it->second->SetFuncAsInterrupted(true);
}
static int Fail() { std::printf("00cb7bc0_TEST FAIL\n"); return 1; }
int main()
{
    CActiveEntityScriptBase scripts[3];
    CEntityScriptMap::value_type entries[3];
    const u64 high = (u64)1 << 32;
    const u64 uids[3] = { 10, 20, high };
    int i;
    for (i = 0; i < 3; ++i) { entries[i].first = uids[i]; entries[i].second.Data = &scripts[i]; entries[i].second.Info = 0; }
    CScriptBase base;
    base.EntityScripts.Vector._M_start = entries;
    base.EntityScripts.Vector._M_finish = entries + 3;
    base.EntityScripts.Vector._M_end_of_storage = entries + 3;
    base.EntityScripts.Dirty = false;
    base.OnScriptedEntityInterrupted(20);
    if (scripts[0].Interrupted || !scripts[1].Interrupted || scripts[2].Interrupted) return Fail();
    base.OnScriptedEntityInterrupted(15);                  // absent, between keys
    base.OnScriptedEntityInterrupted(high << 1);            // absent, past the end
    if (scripts[0].Interrupted || scripts[2].Interrupted) return Fail();
    base.OnScriptedEntityInterrupted(high);                 // high dword decides
    if (!scripts[2].Interrupted || scripts[0].Interrupted) return Fail();
    std::printf("00cb7bc0_TEST PASS\n");
    return 0;
}