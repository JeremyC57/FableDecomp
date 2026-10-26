// std::pair<long,CDefPointer<const CHeroAbilityDef> >::pair(const pair& other) -- copy the long key and the def pointer, add-referencing the def
// Leaf: ??0?$pair
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00592DFB, length 26 bytes (`ret 4`, unaligned: a /Os module).
// Identity: the element copy constructor of the hero-ability heap; the landed __pop_heap_aux 0x0059474E builds its
//   by-value argument through this function (`push ecx ; push ecx ; mov ecx,esp ; call 0x00592DFB`), and 15 call sites in
//   the 0x00593xxx-0x00594xxx heap/sort helpers use it. Byte-identical twin: 0x005F8578.
// Layout: pair = { long first +0 ; CDefPointer<const CHeroAbilityDef> second +4 }; the def pointer is one raw pointer to a
//   counted def object whose reference count sits at +4 (CDefPointeeBase: vptr +0, count +4).
// Shape: STLport's pair has an explicit memberwise copy constructor; the inline CDefPointer copy constructor stores the
//   pointer before the null test (`test edx,edx ; mov [eax+4],edx ; je`) and add-refs with `inc dword ptr [edx+4]`.
//   `this` stays in eax (`mov eax,ecx` first: the constructor returns it), so no register is saved.
class CDefPointeeBase { public:                    // counted def object: vptr, reference count at +4
    virtual void v00();
    virtual void Destroy();                 // slot 1
    mutable long m_refCount;                // +0x04 (mutable: const defs are add-ref'd)
};

class CHeroAbilityDef : public CDefPointeeBase { public: };

template <class T> class CDefPointer { public:
    T* Object;                              // +0x00
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) { }
};
}

template struct std::pair<long, CDefPointer<const CHeroAbilityDef> >;