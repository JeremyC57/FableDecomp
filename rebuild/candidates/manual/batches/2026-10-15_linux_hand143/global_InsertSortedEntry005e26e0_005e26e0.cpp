// InsertSortedEntry_005e26e0(SPriorityEntry* first, SPriorityEntry* next, SPriorityEntry value, CContextSensitivePriorityCompare pred) -- one insertion-sort step over (priority, def pointer) pairs: a value ranked before the first element shifts [first, next) up one slot and goes first, otherwise it is linearly inserted from `next` down
// Leaf: InsertSortedEntry_005e26e0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E26E0, length 132 bytes.
// Label correction: catalogued as CHitParameters::SetPHitMaterialDef (propagated; the real one is the landed
// 0x00868640); its only caller is _Insertion_sort1<pair<long, CDefPointer<CContextSensitiveItemDef const> >*,
// CContextSensitivePriorityCompare> @ 0x005E2E00, which passes (first, next, *next by value, pred).
// Callees: the element `_Copy_backward` @ 0x005E0A00 (`__fastcall` first, last, then dest, the empty tag and false
// on the stack), the unguarded linear insert @ 0x005E07F0 (`__fastcall` position, then the value by value and the
// predicate), the def pointee's virtual slot 1 at a zero count.
// Shape: a `__fastcall` static (first in ecx, next in edx, `ret 0Ch`); the priority compare is inlined
// (`value.first < first->first`); `*first = value` is the pair's memberwise assignment with the compare-first
// counted-pointer operator=; the tag temporary is homed in the dead predicate slot; the by-value pair argument is
// copied into the argument area (add-ref) and the callee's own by-value parameter released at the end.
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags);               // slot 0
    virtual void Delete();                         // slot 1
    unsigned long IVRefCount;                      // +4
};
struct CDefPointer {                               // CDefPointer<const CContextSensitiveItemDef>: one counted pointee
    CIVCountedPointeeBase* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
    void Release()
    {
        CIVCountedPointeeBase* old = Object;
        if (old) {
            if (--old->IVRefCount == 0)
                old->Delete();
            Object = 0;
        }
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
    ~CDefPointer()
    {
        if (Object && --Object->IVRefCount == 0)
            Object->Delete();
    }
};
struct SPriorityEntry {                            // pair<long, CDefPointer<const CContextSensitiveItemDef> >
    long        first;
    CDefPointer second;
};
struct CContextSensitivePriorityCompare {
    bool operator()(const SPriorityEntry& a, const SPriorityEntry& b) const { return a.first < b.first; }
};
struct SCopyTag { SCopyTag() { } };
SPriorityEntry* __fastcall CopyBackward_005e0a00(SPriorityEntry* first, SPriorityEntry* last, SPriorityEntry* dest, const SCopyTag& tag, bool overlap);   // 0x005E0A00
void __fastcall UnguardedLinearInsert_005e07f0(SPriorityEntry* last, SPriorityEntry value, CContextSensitivePriorityCompare pred);   // 0x005E07F0

void __fastcall InsertSortedEntry_005e26e0(SPriorityEntry* first, SPriorityEntry* next, SPriorityEntry value, CContextSensitivePriorityCompare pred)
{
    if (pred(value, *first)) {
        CopyBackward_005e0a00(first, next, next + 1, SCopyTag(), false);
        *first = value;
    } else {
        UnguardedLinearInsert_005e07f0(next, value, pred);
    }
}
