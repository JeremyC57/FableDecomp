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