// std::pair<long,CDefPointer<const CContextSensitiveItemDef> >::pair(const long& a, const CDefPointer<const CContextSensitiveItemDef>& b) -- build a (key, def pointer) pair, add-referencing the def
// Leaf: ??0?$pair
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00592962, length 29 bytes (`ret 8`, unaligned: a /Os module).
// Callers: 0x00591C65 / 0x00591CF5 (the context-sensitive item map's insert path: `value_type(key, def)`).
// Layout: pair = { long first +0 ; CDefPointer<const CContextSensitiveItemDef> second +4 }; the def pointer is one raw
//   pointer to a counted def object whose reference count sits at +4 (CDefPointeeBase: vptr +0, count +4). The engine
//   header engine/CContextSensitiveItemDef.h names no member at +4 (it is the base's count), so the def is a local model.
// Shape: STLport's two-argument constructor `pair(const T1& a, const T2& b) : first(a), second(b)`; both arguments are
//   references (`mov ecx,[esp+4] ; mov ecx,[ecx]`), the inline CDefPointer copy constructor stores the pointer before its
//   null test and add-refs with `inc dword ptr [edx+4]`; `this` stays in eax (returned).
// Pragma: `#pragma optimize("s", on)` (the module is size-optimised): it keeps the key load/store in ecx before the second
//   argument is fetched; the speed form hoists the `mov ecx,[esp+8]` above the first store (edx carries the key).
#pragma optimize("s", on)
struct CDefPointeeBase {                    // counted def object: vptr, reference count at +4
    virtual void v00();
    virtual void Destroy();                 // slot 1
    mutable long m_refCount;                // +0x04 (mutable: const defs are add-ref'd)
};

struct CContextSensitiveItemDef : CDefPointeeBase { };

template <class T> struct CDefPointer {
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
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
}

template struct std::pair<long, CDefPointer<const CContextSensitiveItemDef> >;