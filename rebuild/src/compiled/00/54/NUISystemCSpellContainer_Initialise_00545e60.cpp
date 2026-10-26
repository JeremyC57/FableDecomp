// NUISystem::CSpellContainer::Initialise() -- the state-component initialisation, then copy every child into the base-spell list and clear the pending spell
// Leaf: Initialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x00545E60, 147 bytes (plain `ret`).
// Callees: NUISystem::NUISystem::CChangingStateComponent::Initialise @ 0x0052C730 (landed), vector<CCountedPointer<CComponent>>::
// _M_insert_overflow(pointer, const T&, const __false_type&, size_type, bool) @ 0x005359D0 (manifest "std::vector::_Reallocate").
// Label: CSpellContainer vtable 0x0124786C slot 43 (rtti-vtables.tsv; slot 43 is Initialise, as the landed CViewport::Initialise
// 0x0054E640); Ego_r.pdb names ?Initialise@CSpellContainer@NUISystem@@UAEXXZ.
// Layout: Children at retail +0xb0, BaseSpells (12-byte STLport vector: start, finish, end of storage) +0x15c, NewSpell
// +0x168 / +0x16c (Ego_r.pdb offsets minus 0x34, as AcceptAssignments 0x00545670).
// Shape: the index loop re-evaluates Children.size() each pass; STLport push_back inlined: room left -> the null-tested
// placement copy (the counted-pointer copy constructor bumps the count) and `_M_finish += 1`; full -> _M_insert_overflow with
// the empty __false_type temporary homed in the top byte of the `push ecx` word, fill 1, at end. ebp holds the zero for the
// placement test and the two NewSpell stores.
#include <stddef.h>
inline void* operator new(size_t, void* where) { return where; }

class CCPPointerInfo { public:
    long  Count;                                   // +0x00
    void (__fastcall* Destroy)(void* item);        // +0x04
    void* Item;                                    // +0x08
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info)
    {
        if (Info)
            ++Info->Count;
    }
};
struct __false_type {
    __false_type() {}
};
template <class T> class CCountedVector {         // STLport vector<CCountedPointer<T>> (12 bytes)
public:
    CCountedPointer<T>* _M_start;
    CCountedPointer<T>* _M_finish;
    CCountedPointer<T>* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    CCountedPointer<T>& operator[](unsigned long n) { return _M_start[n]; }
    void _M_insert_overflow(CCountedPointer<T>* position, const CCountedPointer<T>& x, const __false_type&,
                            unsigned long fillLength, bool atEnd);   // 0x005359D0
    void push_back(const CCountedPointer<T>& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CCountedPointer<T>(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};

namespace NUISystem {
class CChangingStateComponent {
public:
    virtual void v000();
    unsigned char _pad04[0xb0 - 4];
    CCountedVector<CChangingStateComponent> Children;   // +0xb0
    unsigned char _padbc[0x15c - 0xbc];
public:
    virtual void Initialise();   // 0x0052C730
};
class CSpellContainer : public CChangingStateComponent {
public:
    CCountedVector<CChangingStateComponent> BaseSpells;   // +0x15c
    CChangingStateComponent* NewSpellComponent;    // +0x168
    unsigned long            NewSpellIndex;        // +0x16c
    void Initialise();
};

void CSpellContainer::Initialise()
{
    CChangingStateComponent::Initialise();
    for (unsigned long i = 0; i < Children.size(); ++i)
        BaseSpells.push_back(Children[i]);
    NewSpellComponent = 0;
    NewSpellIndex = 0;
}
}