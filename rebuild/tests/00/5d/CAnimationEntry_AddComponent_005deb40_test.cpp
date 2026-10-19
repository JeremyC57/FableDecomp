#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
// CAnimationEntry::AddComponent(CAnimComponentBase*) -- replace a component of the same type, then sort the intrusive component map.
// Retail: 0x005DEB40, 295 bytes. Leaf: AddComponent
// No shared CAnimationEntry/CAnimComponentMap/CAnimComponentBase headers exist.
// PDB entry layout and landed constructors 005DEC70/005DED00 confirm Components+0xc.
// Retail map is 24 bytes versus donor28 because its vector is 12 bytes; First/Last/Limit
// at +8/+c/+10, comparator +14, NeedsSort +15. CIVCountedPointeeBase.h is data-only;
// local virtual shell preserves IVRefCount+4 and Delete slot1, GetType slot2.
// Calls: new00BFEA1A, LowerBound005DFA50, Erase005E1E60, Insert005E29D0,
// Sort005E3AD0 (__fastcall), plus component/map virtual slots.
#include <new>
struct CIVCountedPointeeBase {
    virtual void Slot0(); virtual void Delete();
    unsigned long IVRefCount;
    CIVCountedPointeeBase() : IVRefCount(1) {}
};
struct CAnimComponentBase : CIVCountedPointeeBase { virtual long GetType() const; };
template<class T> struct CIVCountedPointer {
    T* Object;
    CIVCountedPointer(T* p=0) : Object(p) {}
    CIVCountedPointer(const CIVCountedPointer& other) { T* p=other.Object; Object=p; if (p) ++p->IVRefCount; }
    ~CIVCountedPointer() { Release(); }
    void Release() { T* p=Object; if (p) { if (--p->IVRefCount==0) p->Delete(); Object=0; } }
    void Reset(T* p) { Release(); Object=p; }
};
struct CAnimComponentEntry { long Key; CAnimComponentBase* Object; };
struct CComponentCompare { unsigned char State; };
void __fastcall SortComponents(CAnimComponentEntry*, CAnimComponentEntry*, CComponentCompare); //005E3AD0
struct CAnimComponentVectorMap {
    CAnimComponentEntry* First; CAnimComponentEntry* Last; CAnimComponentEntry* Limit;
    CComponentCompare Compare; bool NeedsSort;
    CAnimComponentVectorMap() : First(0),Last(0),Limit(0),NeedsSort(false) {}
    CAnimComponentEntry* LowerBound(const long&); //005DFA50
    CAnimComponentEntry* Erase(CAnimComponentEntry*); //005E1E60
    void Insert(CAnimComponentEntry*,const CAnimComponentEntry*,const CComponentCompare*,unsigned long,unsigned long); //005E29D0
    CAnimComponentEntry* Find(long key) {
        long lookup=key;
        CAnimComponentEntry* p=LowerBound(lookup);
        if (p==Last || key<p->Key) p=Last;
        return p;
    }
    __forceinline void Add(long key,CAnimComponentBase* component) {
        ++component->IVRefCount;
        CAnimComponentEntry value={key,component};
        if (Last!=Limit) {
            CAnimComponentEntry* destination=Last;
            if (destination) { destination->Key=key; destination->Object=component; ++component->IVRefCount; }
            ++Last;
        } else { CComponentCompare allocator; Insert(Last,&value,&allocator,1,1); }
        if (--component->IVRefCount==0) component->Delete();
        NeedsSort=true;
    }
    void Sort() { if (NeedsSort) { SortComponents(First,Last,Compare); NeedsSort=false; } }
};
struct CAnimComponentMap : CIVCountedPointeeBase {
    virtual void Slot0(); //vtable01255878
    CAnimComponentVectorMap Map;
};
struct CAnimationEntry {
    long BankIndex,AnimName,GroupName;
    CIVCountedPointer<CAnimComponentMap> Components;
    void AddComponent(CAnimComponentBase*);
};
void CAnimationEntry::AddComponent(CAnimComponentBase* component)
{
    if (!Components.Object) Components.Reset(new CAnimComponentMap);
    const long type=component->GetType();
    CAnimComponentMap* map=Components.Object;
    CAnimComponentEntry* old=map->Map.Find(type);
    if (old!=map->Map.Last) map->Map.Erase(old);
    Components.Object->Map.Add(type,component);
    if (--component->IVRefCount==0) component->Delete();
    Components.Object->Map.Sort();
}

#include <stdio.h>
static CAnimationEntry* active;
static CAnimComponentBase* incoming;
static CAnimComponentMap* replacementMap;
static int lowerCalls, eraseCalls, insertCalls, sortCalls, typeCalls, invalid, baseDeletes;
static unsigned long refAtSort;
static long requestedKey;
static CAnimComponentEntry* eraseExpected;
static CAnimComponentEntry storage[32];
void CIVCountedPointeeBase::Slot0() { ++invalid; }
void CIVCountedPointeeBase::Delete() { ++baseDeletes; }
void CAnimComponentMap::Slot0() { ++invalid; }
long CAnimComponentBase::GetType() const { ++invalid; return 0; }
struct Component : CAnimComponentBase {
    long Key; int deletes;
    Component(long key) : Key(key),deletes(0) {}
    virtual long GetType() const { ++typeCalls; if(this!=incoming) ++invalid; return Key; }
    virtual void Delete() { if(IVRefCount!=0) ++invalid; ++deletes; }
};
CAnimComponentEntry* CAnimComponentVectorMap::LowerBound(const long& key) {
    ++lowerCalls; requestedKey=key;
    if(typeCalls!=1 || eraseCalls || insertCalls || sortCalls) ++invalid;
    CAnimComponentEntry* p=First;
    while(p!=Last && p->Key<key) ++p;
    return p;
}
CAnimComponentEntry* CAnimComponentVectorMap::Erase(CAnimComponentEntry* p) {
    ++eraseCalls;
    if(p!=eraseExpected || p==Last || p->Key!=requestedKey || lowerCalls!=1) ++invalid;
    CAnimComponentBase* old=p->Object;
    for(CAnimComponentEntry* next=p+1;next!=Last;++next) next[-1]=*next;
    --Last;
    if(old && --old->IVRefCount==0) old->Delete();
    if(replacementMap) active->Components.Object=replacementMap;
    return p;
}
void CAnimComponentVectorMap::Insert(CAnimComponentEntry* p,const CAnimComponentEntry* value,const CComponentCompare*,unsigned long a,unsigned long b) {
    ++insertCalls;
    if(p!=Last || Last!=Limit || a!=1 || b!=1 || value->Object!=incoming || value->Key!=requestedKey) ++invalid;
    long n=First?(long)(Last-First):0;
    for(long i=0;i<n;++i) storage[i]=First[i];
    storage[n]=*value; ++storage[n].Object->IVRefCount;
    First=storage; Last=storage+n+1; Limit=storage+32;
}
void __fastcall SortComponents(CAnimComponentEntry* first,CAnimComponentEntry* last,CComponentCompare) {
    ++sortCalls;
    CAnimComponentVectorMap& map=active->Components.Object->Map;
    if(first!=map.First || last!=map.Last || !map.NeedsSort || typeCalls!=1 || lowerCalls!=1) ++invalid;
    refAtSort=incoming->IVRefCount;
    for(CAnimComponentEntry* i=first;i!=last;++i) for(CAnimComponentEntry* j=i+1;j!=last;++j)
        if(j->Key<i->Key) { CAnimComponentEntry tmp=*i; *i=*j; *j=tmp; }
}
static void reset(CAnimationEntry& entry,CAnimComponentMap& map,Component& value,CAnimComponentEntry* data,long n,long capacity) {
    active=&entry; incoming=&value; replacementMap=0;
    lowerCalls=eraseCalls=insertCalls=sortCalls=typeCalls=invalid=baseDeletes=0; requestedKey=-1; refAtSort=0;
    entry.Components.Object=&map;
    map.Map.First=data; map.Map.Last=data+n; map.Map.Limit=data+capacity; map.Map.NeedsSort=false; map.Map.Compare.State=0;
    value.IVRefCount=1; value.deletes=0; eraseExpected=0;
}
static bool common(CAnimationEntry& entry) {
    return invalid==0 && lowerCalls==1 && typeCalls==1 && sortCalls==1 && refAtSort==1
        && incoming->IVRefCount==1 && !entry.Components.Object->Map.NeedsSort;
}
int main() {
    CAnimationEntry entry; CAnimComponentMap map,otherMap; CAnimComponentEntry data[8],other[8];
    Component low(1),old(5),high(9),newValue(5);
    int ok=1;
    data[0].Key=1;data[0].Object=&low; data[1].Key=9;data[1].Object=&high;
    reset(entry,map,newValue,data,2,8); entry.AddComponent(&newValue);
    ok=ok && common(entry) && eraseCalls==0 && insertCalls==0 && map.Map.Last==data+3
        && data[1].Key==5 && data[1].Object==&newValue && low.IVRefCount==1 && high.IVRefCount==1;
    data[0].Key=1;data[0].Object=&low;data[1].Key=5;data[1].Object=&old;data[2].Key=9;data[2].Object=&high;
    old.IVRefCount=1; old.deletes=0;
    reset(entry,map,newValue,data,3,8); eraseExpected=data+1; entry.AddComponent(&newValue);
    ok=ok && common(entry) && eraseCalls==1 && insertCalls==0 && old.deletes==1 && old.IVRefCount==0
        && map.Map.Last==data+3 && data[1].Object==&newValue;
    // Full storage invokes the insertion helper, which copies and retains the incoming component.
    data[0].Key=1;data[0].Object=&low;data[1].Key=9;data[1].Object=&high;
    reset(entry,map,newValue,data,2,2); entry.AddComponent(&newValue);
    ok=ok && common(entry) && insertCalls==1 && eraseCalls==0 && map.Map.First==storage && storage[1].Object==&newValue;
    // End-of-range lookup appends without accidentally erasing the last lower key.
    data[0].Key=1;data[0].Object=&low;
    reset(entry,map,newValue,data,1,8); entry.AddComponent(&newValue);
    ok=ok && common(entry) && eraseCalls==0 && data[1].Object==&newValue;
    // Erase callbacks may replace the owner's map; insertion/sort must re-read Components.
    data[0].Key=5;data[0].Object=&old;old.IVRefCount=1;old.deletes=0;
    reset(entry,map,newValue,data,1,8); eraseExpected=data; replacementMap=&otherMap;
    otherMap.Map.First=other;otherMap.Map.Last=other;otherMap.Map.Limit=other+8;otherMap.Map.NeedsSort=false;otherMap.Map.Compare.State=0;
    entry.AddComponent(&newValue);
    ok=ok && common(entry) && eraseCalls==1 && otherMap.Map.Last==other+1 && other[0].Object==&newValue && old.deletes==1;
    // Missing map exercises the real allocation and inline construction in the target source.
    reset(entry,map,newValue,data,0,0);entry.Components.Object=0;entry.AddComponent(&newValue);
    CAnimComponentMap* allocated=entry.Components.Object;
    ok=ok && common(entry) && allocated && allocated!=&map && allocated->IVRefCount==1
        && insertCalls==1 && allocated->Map.First[0].Object==&newValue;
    entry.Components.Object=0; if (allocated!=&map && allocated!=&otherMap) delete allocated;
    printf("005deb40_TEST %s\n",ok?"PASS":"FAIL"); return ok?0:1;
}