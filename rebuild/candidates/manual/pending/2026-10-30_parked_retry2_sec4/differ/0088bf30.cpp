// CAIBrain::InitStateGroups(CDefPointer<CBrainDef const>) -- create a state group for every behaviour the brain def names that is not disabled and that the static state-group table knows, add each to the priority map, then sort the map if it changed
// Grade: DIFFER(550v545) nd=30 (was 536v545 nd=41) -- levers tried: nested length test (no change), ternary (no change), SameText helper for the length+Compare tail (fixes the al-then-bl compare result and the separate xor al,al mismatch path); remaining: retail shares the disabled-path string dtor with the found path and tail-duplicates Dirty=true into both push_back arms (ours one dtor per exit + a single Dirty store). Next: try the CGroupMap::Add push_back/Dirty shape from a landed CKeyPair map Add, and a goto-free found flag
#include <new>
struct CCharStringData {
    const char*   m_pData;
    unsigned long m_length;
};
long __fastcall Compare(const char* a, const char* b);  // 0x00411570
#pragma auto_inline(off)
// Same-TU body for the string compare 0x00411570 (a plain byte loop that writes no memory), so VC7.1 knows the call leaves the
// callers' string blocks untouched.
__declspec(noinline) long __fastcall Compare(const char* a, const char* b)
{
    for (;;) {
        long d = (long)(unsigned char)*a - (long)(unsigned char)*b;
        if (d != 0)
            return d;
        if (*a == 0)
            return 0;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const CCharString& o);
    ~CCharString();                                     // 0x0099EAE0
    bool IsEmpty() const { return PStringData == 0 || PStringData->m_length == 0; }
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData)
            return SameText(other);
        return false;
    }
    __forceinline bool SameText(const CCharString& other) const
    {
        if (PStringData->m_length != other.PStringData->m_length)
            return false;
        return Compare(PStringData->m_pData, other.PStringData->m_pData) ? false : true;
    }
};
struct CDefString { long TablePos; };
struct CDefStringTable {
    CCharString GetString(long id);                     // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828

struct CDefPointeeBase {
    virtual void v000();
    virtual void DeleteThis() const;
    mutable long RefCount;
};
template <class T> struct CArray {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    long size() const { return long(_M_finish - _M_start); }
    T& operator[](unsigned long i) const { return *(_M_start + i); }
};
struct CBrainBehaviour {
    CDefString Name;
    long Priority;
    long ScriptAIPriority;
};
struct CBrainDef : CDefPointeeBase {
    unsigned char _pad08[0x3c - 8];
    CArray<CBrainBehaviour> Behaviours;                 // +0x3c
    CArray<CDefString> DisabledBehaviours;              // +0x48
};
template <class T> struct CDefPointer {
    T* Object;
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->DeleteThis(); }
    T* operator->() const { return Object; }
};

struct CAIBrain;
struct CThingAICreature;
struct CAIStateGroupBase {
    virtual void v00();
    virtual void Init(CAIBrain* brain, CThingAICreature* creature, long priority, long scriptPriority, const CCharString& name);
    CAIBrain* PBrain;
    long ScriptAIPriority;
    CCharString Name;
    bool Terminated;
    unsigned char Priority;                             // +0x11
};
struct SStateGroupEntry {
    CCharString Name;
    CAIStateGroupBase* (*Create)();
};
extern SStateGroupEntry g_StateGroupTable[];            // 0x013BB008

struct __true_type { __true_type() { } };
struct SGroupPair {
    long first;
    CAIStateGroupBase* second;
    SGroupPair(long a, CAIStateGroupBase* b) : first(a), second(b) { }
};
struct CKeyPairCompareGreater { };
void __fastcall SortGroupPairs(SGroupPair* first, SGroupPair* last, CKeyPairCompareGreater comp);   // 0x0088D130
struct CGroupMap {
    SGroupPair* _M_start;
    SGroupPair* _M_finish;
    SGroupPair* _M_end_of_storage;
    CKeyPairCompareGreater Compare;                     // +0x0c
    bool Dirty;                                         // +0x0d
    void _M_insert_overflow(SGroupPair* pos, const SGroupPair& x, const __true_type& tag, unsigned long n, bool atEnd);  // 0x0088CC20
    void push_back(const SGroupPair& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) SGroupPair(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
    void Add(long key, CAIStateGroupBase* value)
    {
        push_back(SGroupPair(key, value));
        Dirty = true;
    }
    void Sort()
    {
        if (Dirty) {
            SortGroupPairs(_M_start, _M_finish, Compare);
            Dirty = false;
        }
    }
};
SGroupPair* GrowGroupPairs(SGroupPair* start, SGroupPair* finish, SGroupPair* pos, long first, CAIStateGroupBase* second, unsigned long n, bool atEnd, SGroupPair** newFinish, SGroupPair** newEnd);   // stand-in callee
#pragma auto_inline(off)
// Same-TU out-only stand-in for the vector's _M_insert_overflow instantiation (a template body compiled in retail's TU): hands the
// element and the bounds on BY VALUE and writes the three words itself, so neither the value nor the empty tag escapes the call.
__declspec(noinline) void CGroupMap::_M_insert_overflow(SGroupPair* pos, const SGroupPair& x, const __true_type& tag, unsigned long n, bool atEnd)
{
    SGroupPair* finish;
    SGroupPair* end;
    _M_start = GrowGroupPairs(_M_start, _M_finish, pos, x.first, x.second, n, atEnd, &finish, &end);
    _M_finish = finish;
    _M_end_of_storage = end;
}
#pragma auto_inline(on)

struct CAIBrain {
    void* _vptr;
    unsigned char _pad04[0x18 - 4];
    CThingAICreature* Creature;                         // +0x18
    unsigned char _pad1c[0x28 - 0x1c];
    CGroupMap StateGroupList;                           // +0x28
    void InitStateGroups(CDefPointer<CBrainDef> def);
};

void CAIBrain::InitStateGroups(CDefPointer<CBrainDef> def)
{
    if (def.Object) {
        long count = def->Behaviours.size();
        for (long i = 0; i < count; ++i) {
            CCharString name = g_DefStringTable.GetString(def->Behaviours[i].Name.TablePos);
            if (name.IsEmpty())
                continue;
            bool disabled = false;
            for (unsigned long d = 0; d < (unsigned long)def->DisabledBehaviours.size(); ++d) {
                {
                    CCharString disabledName = g_DefStringTable.GetString(def->DisabledBehaviours[d].TablePos);
                    disabled = name == disabledName;
                }
                if (disabled)
                    break;
            }
            if (disabled)
                continue;
            for (long j = 0; g_StateGroupTable[j].Create; ++j) {
                if (g_StateGroupTable[j].Name == name) {
                    CAIStateGroupBase* group = g_StateGroupTable[j].Create();
                    const CBrainBehaviour& behaviour = def->Behaviours[i];
                    group->Init(this, Creature, behaviour.Priority, behaviour.ScriptAIPriority, g_StateGroupTable[j].Name);
                    StateGroupList.Add(group->Priority, group);
                    break;
                }
            }
        }
    }
    StateGroupList.Sort();
}
