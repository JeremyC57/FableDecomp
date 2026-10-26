// CTCStealth::GetNumberOfAttackersOverMaximum(vector<const CThing*>* attackers) const -- collect the danger things that are attacking (status 2 or 3) into the caller's vector (or a local one) and return by how many they exceed the smallest of the global, per-thing and region attacker limits (-1 when no limit applies or none is exceeded)
// Leaf: GetNumberOfAttackersOverMaximum
// Retail: Fable.exe (TLC, VC7.1) @ 0x00747890, 276 bytes, `ret 4`.
// Callees: vector<const CThing*>::reserve @ 0x007463E0 (manifest "Vector_Reserve"), CBaseIntelligentPointer::GetPItem
//   @ 0x00A01B50, the vector's reallocating insert @ 0x0049C370 (manifest "Std_Vector_Insert_Range"; a same-TU stand-in body
//   under auto_inline(off), as the landed 0x00778B30 / 0x0049C450 push_back rows), free @ 0x00BFEA14. Global attacker limit
//   @ 0x01375728 (named descriptively).
// Layout: Ego_r.pdb CTCStealth DangerThings (vector<pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus> >,
//   16-byte elements: status +8) +0x38, MaxNumberOfDangerThings +0x44 (PDB 0x48), RegionBasedMaxNumberOfDangerThings +0x48.
// Shape: a default-constructed local vector stands in for a null `attackers`; the inline STLport push_back (placement new
//   under its null test, the value temporary and the empty tag both homed in the dead `attackers` slot); each limit narrows
//   `maximum` only when set (-1 = none); the local vector's destructor runs on both exits.
#include <new>
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
struct SScalarTag { SScalarTag() { } };                 // STLport __true_type
template <class T> struct CRetailVector {               // STLport vector: {start, finish, end_of_storage}
    T* first;
    T* last;
    T* end;
    CRetailVector() : first(0), last(0), end(0) {}
    ~CRetailVector() { if (first) free(first); }
    unsigned int size() const { return last - first; }
    void reserve(unsigned int count);                   // 0x007463E0
    void InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x0049C370
    void push_back(const T& value)
    {
        if (last != end) {
            new (last) T(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};
void* GrowVectorStorage_747890(void* vector, unsigned long count);   // stand-in callee of the stand-in below
#pragma auto_inline(off)
// Same-TU body of the vector growth template (InsertGrow, instantiated in retail's TU): reads the value through its const
// reference into the new storage and never touches the empty tag (the landed push_back idiom).
template <class T>
__declspec(noinline) void CRetailVector<T>::InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool noTail)
{
    T* p = (T*)GrowVectorStorage_747890(this, count);
    if (p)
        *p = value;
}
#pragma auto_inline(on)
class CThing;
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
namespace NStealth {
class CDangerStatus { public:
    long Status;                                        // +0x00 (2, 3 = attacking)
    long Timer;                                         // +0x04
};
}
struct SDangerThing {                                   // pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus>
    CIntelligentPointer<CThing> first;
    NStealth::CDangerStatus second;
};
extern long g_maxNumberOfAttackers;                     // 0x01375728
class CTCStealth { public:
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    unsigned char _pad08[0x38 - 0x08];
    CRetailVector<SDangerThing> DangerThings;           // +0x38
    long          MaxNumberOfDangerThings;              // +0x44
    long          RegionBasedMaxNumberOfDangerThings;   // +0x48
    long GetNumberOfAttackersOverMaximum(CRetailVector<const CThing*>* attackers) const;
};

long CTCStealth::GetNumberOfAttackersOverMaximum(CRetailVector<const CThing*>* attackers) const
{
    CRetailVector<const CThing*> localAttackers;
    if (!attackers)
        attackers = &localAttackers;
    attackers->reserve(DangerThings.size());
    for (SDangerThing* it = DangerThings.first; it != DangerThings.last; ++it) {
        if (it->second.Status == 3 || it->second.Status == 2)
            attackers->push_back(it->first.GetPItem());
    }
    long count = attackers->size();
    long maximum = -1;
    if (g_maxNumberOfAttackers != -1)
        maximum = g_maxNumberOfAttackers;
    if (MaxNumberOfDangerThings != -1 && (maximum == -1 || MaxNumberOfDangerThings < maximum))
        maximum = MaxNumberOfDangerThings;
    if (RegionBasedMaxNumberOfDangerThings != -1 && (maximum == -1 || RegionBasedMaxNumberOfDangerThings < maximum))
        maximum = RegionBasedMaxNumberOfDangerThings;
    if (maximum != -1 && count > maximum)
        return count - maximum;
    return -1;
}