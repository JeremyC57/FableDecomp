// _global::UninitFillN_LongCountedPointer_006b8de0(SLongCountedPointer_006b8de0* first, unsigned int n, const SLongCountedPointer_006b8de0& x, const __false_type&) -- STLport __uninitialized_fill_n over 12-byte pair<long, CCountedPointer<T> >: copy-construct n copies (add a reference to the count block), return first + n
// Leaf: UninitFillN_LongCountedPointer_006b8de0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B8DE0, length 52 bytes (+12 int3 pad), `ret 8`.
// Callers: one site, 0x006BAB28 in the vector resize helper 0x006BAA90 (catalog "Vector_ResizeAndFillCounted").
// Identity: the catalog label `_Uninit_fill_n<pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>>*, ...>` is
//   propagated (a 12-byte element here, not the 16-byte pair). The element is a key word followed by a CCountedPointer<T>
//   {Data +4, Info +8} whose copy bumps the count block's first word (as CScriptThing::PImp, 0x004ABE90); T and the key
//   are not named here -- descriptive.
// Shape: STLport 4.x `__uninitialized_fill_n(first, n, x, const __false_type&)`, `__fastcall` (first in ecx -> eax as `cur`,
//   n in edx -> esi counted down, &x on the stack -> edx loaded once). STLport pair's memberwise copy: the key, then the
//   counted pointer's user copy (Data, Info, `inc dword ptr [ecx]` when non-null); placement-new null test; /O2 pad.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct SCountBlock_006b8de0 {
    long m_references;                          // +0 reference count
};

struct SCountedPointer_006b8de0 {               // CCountedPointer<T>, 8 bytes
    void*                 Data;                 // +0
    SCountBlock_006b8de0* Info;                 // +4
    SCountedPointer_006b8de0(const SCountedPointer_006b8de0& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_references;
    }
};

struct SLongCountedPointer_006b8de0 {           // std::pair<long, CCountedPointer<T> >, 12 bytes
    long                     first;             // +0x00
    SCountedPointer_006b8de0 second;            // +0x04
    SLongCountedPointer_006b8de0(const SLongCountedPointer_006b8de0& o) : first(o.first), second(o.second) { }
};

SLongCountedPointer_006b8de0* __fastcall UninitFillN_LongCountedPointer_006b8de0(SLongCountedPointer_006b8de0* first, unsigned int n, const SLongCountedPointer_006b8de0& x, const __false_type&)
{
    SLongCountedPointer_006b8de0* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) SLongCountedPointer_006b8de0(x);
    return cur;
}