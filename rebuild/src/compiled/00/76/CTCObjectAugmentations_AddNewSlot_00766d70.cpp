// CTCObjectAugmentations::AddNewSlot() -- append one empty augmentation slot while below the def's slot maximum
// Leaf: ?AddNewSlot@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00766D70, 86 bytes (+int3 pad to 96), plain `ret`.
// Caller: 0x005BEA1A (slot-count top-up loop between GetNumberOfSlots 0x00765D80 and
//   GetMaximumNumberOfSlots 0x00765D90, both of which are inlined here).
// Callee: std::vector<CDefPointer<const CThingBaseDef> >::_M_insert_overflow(pos, x, const __false_type&,
//   fill_len, atend) @ 0x005B3275 (an Ego-TU frame-pointer COMDAT shared by seven counted-pointer vectors).
// Shape: `if (GetNumberOfSlots() < GetMaximumNumberOfSlots()) Augmentations.push_back(CDefPointer())` -- a
//   signed compare of the `sar`-ed size against PDef->MaximumNumberOfSlots (+0x2C), then STLport push_back
//   inline: the null temporary stored at [esp+4] before the capacity test, the in-place copy folded to
//   `mov [ecx],0`, the overflow called with the __false_type tag byte at [esp+3], fill_len 1, atend true.
// Layout: PDB CTCObjectAugmentations: PDef +0x0C (CDefPointer<const CObjectAugmentationsDef>, one word),
//   LoadedFromSaveGame +0x10, Augmentations +0x14 (STLport vector, 12 bytes in retail).
//   CObjectAugmentationsDef: NumberOfSlots +0x28, MaximumNumberOfSlots +0x2C.
namespace std {
struct __false_type { __false_type() { } };
}

struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy() const;
    mutable unsigned long m_count;                // +0x04
};

class CThingBaseDef : public CDefPointeeBase {
};

class CObjectAugmentationsDef : public CDefPointeeBase {
public:
    unsigned char _pad08[0x28 - 0x08];
    long NumberOfSlots;                           // +0x28
    long MaximumNumberOfSlots;                    // +0x2C
};

template <class T> class CDefPointer {
public:
    T* Object;                                    // +0x00
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_count;
    }
    T* operator->() const { return Object; }
};

template <class T> inline void* operator new(unsigned int, T* place) { return place; }

namespace std {
template <class _Tp> class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&,
                            unsigned int __fill_len, bool __atend = false);   // 0x005B3275
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(__x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);
    }
};
}

struct CTCBase {
    virtual void v00();
    void* ParentThing;                            // +0x04
    unsigned char _base08[4];
};

class CTCObjectAugmentations : public CTCBase {
public:
    CDefPointer<const CObjectAugmentationsDef> PDef;                 // +0x0C
    bool LoadedFromSaveGame;                                         // +0x10
    std::vector<CDefPointer<const CThingBaseDef> > Augmentations;  // +0x14
    long GetNumberOfSlots() const { return (long)Augmentations.size(); }
    long GetMaximumNumberOfSlots() const { return PDef->MaximumNumberOfSlots; }
    void AddNewSlot();
};

void CTCObjectAugmentations::AddNewSlot()
{
    if (GetNumberOfSlots() < GetMaximumNumberOfSlots())
        Augmentations.push_back(CDefPointer<const CThingBaseDef>());
}