// _global::UninitCopy_CIVCountedPointer_00a6afd0(const CIVCountedPointer* first, const CIVCountedPointer* last, CIVCountedPointer* result, const __false_type&) -- STLport __uninitialized_copy over 4-byte intrusive counted pointers: placement-copy each pointer (add-ref'ing the pointee), return the advanced result
// Leaf: UninitCopy_CIVCountedPointer_00a6afd0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6AFD0, length 45 bytes (+ int3 padding), `ret 8`.
// Callers: the vector insert/regrowth helpers @ 0x00A6BA60 (call 0x00A6BB18) and 0x00A6BCD0 (calls 0x00A6BD2E/0x00A6BD8B)
//   of the CTextureManager TU (landed neighbours: the CIVCountedPointer release/adopt bodies @ 0x00A6AF40 / 0x00A6B4D0
//   handle the same 4-byte element). Byte-identical to 0x0071EA10 (another TU's instance of the same template).
// Callees: none.
// Layout: CIVCountedPointer<T> = { T* Object } (4 bytes); the pointee is a CIVCountedPointeeBase (vtable +0, count +4).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` under /Gr (`__fastcall`: first in
//   ecx, last in edx, result + the tag reference on the stack). The pointer copy is `p = o.p ; if (p) ++p->count`
//   (`mov esi,[ecx] ; test esi,esi ; mov [eax],esi ; je ; inc [esi+4]`) behind the placement-new null test.
// Label correction: the catalog's `_Uninit_copy<...pair<unsigned __int64,CCountedPointer<CActiveEntityScriptBase>>...>` is
//   propagated (4-byte stride, intrusive count).
inline void* operator new(unsigned int, void* where) { return where; }

struct CIVCountedPointeeBase {
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

CIVCountedPointer* __fastcall UninitCopy_CIVCountedPointer_00a6afd0(const CIVCountedPointer* first, const CIVCountedPointer* last, CIVCountedPointer* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) CIVCountedPointer(*first);
    return result;
}