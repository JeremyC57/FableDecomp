// _global::UninitCopy_CIVCountedPointer_0071ea10(const CIVCountedPointer* first, const CIVCountedPointer* last, CIVCountedPointer* result, const __false_type&) -- STLport __uninitialized_copy over 4-byte intrusive counted pointers: placement-copy each pointer (add-ref'ing the pointee), return the advanced result
// Leaf: UninitCopy_CIVCountedPointer_0071ea10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071EA10, length 45 bytes (+ int3 padding), `ret 8`.
// Callers: one call @ 0x00721220 in the vector assign/regrowth helper @ 0x00721150 (catalog "Vector_CopyOrMove_CountedPtr")
//   of the CTCHeroMorph TU (landed neighbours 0x0071D7C0..0x0071EE30; the CIVCountedPointer release bodies @ 0x0071E4A0 /
//   0x0071EF40 handle the same 4-byte element).
// Callees: none.
// Layout: CIVCountedPointer<T> = { T* Object } (4 bytes); the pointee is a CIVCountedPointeeBase (vtable +0, count +4).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` under /Gr (`__fastcall`: first in
//   ecx, last in edx, result + the tag reference on the stack). The pointer copy is `p = o.p ; if (p) ++p->count`
//   (`mov esi,[ecx] ; test esi,esi ; mov [eax],esi ; je ; inc [esi+4]`) behind the placement-new null test.
// Label correction: the catalog's `_Uninit_copy<...pair<unsigned __int64,CCountedPointer<CActiveEntityScriptBase>>...>` is
//   propagated (4-byte stride, intrusive count).
inline void* operator new(unsigned int, void* where) { return where; }

class CIVCountedPointeeBase { public:
    virtual void Destroy(int flags);         // slot 0
    virtual void Delete();                   // slot 1 (+0x04)
    unsigned long IVRefCount;                // +0x04
};

struct CIVCountedPointer {                   // CIVCountedPointer<T>, 4 bytes
    CIVCountedPointeeBase* Object;           // +0x00
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
};

struct __false_type { __false_type() { } };

CIVCountedPointer* __fastcall UninitCopy_CIVCountedPointer_0071ea10(const CIVCountedPointer* first, const CIVCountedPointer* last, CIVCountedPointer* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) CIVCountedPointer(*first);
    return result;
}