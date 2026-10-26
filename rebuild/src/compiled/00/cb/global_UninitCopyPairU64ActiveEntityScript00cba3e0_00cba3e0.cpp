// _global::UninitCopy_PairU64ActiveEntityScript_00cba3e0(const SEntityScriptEntry* first, const SEntityScriptEntry* last, SEntityScriptEntry* result, const __false_type&) -- STLport __uninitialized_copy over pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>>: placement-copy each 16-byte entry (add-ref'ing the shared count block), return the advanced result
// Leaf: UninitCopy_PairU64ActiveEntityScript_00cba3e0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBA3E0, length 62 bytes (+ int3 padding), `ret 8`.
// Callers: the landed CActiveEntityScriptVector::_M_insert_overflow @ 0x00CBC850 (calls @ 0x00CBC89B / 0x00CBC8FC: copy the
//   head and the tail of the old block), which names this callee "__uninitialized_copy 0x00CBA3E0".
// Callees: none (the element copy is fully inline).
// Layout (from the landed caller): pair<unsigned __int64 key, CCountedPointer<CActiveEntityScriptBase>> = 16 bytes:
//   key low +0x0, key high +0x4, CCountedPointer {Data +0x8, Info +0xC}; the shared count block keeps its count as word 0.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` under /Gr (`__fastcall`: first in
//   ecx, last in edx, result + the tag reference on the stack, `ret 8`). The empty test runs before esi is saved
//   (`cmp ecx,edx ; mov eax,[esp+4] ; je`), the placement-new null test guards each element, the four words are copied
//   through esi in member order and the count block is add-ref'd (`test esi,esi ; mov [eax+0Ch],esi ; je ; inc [esi]`).
// Label correction: none (the catalog `_Uninit_copy<...pair<unsigned __int64,CCountedPointer<CActiveEntityScriptBase>>...>`
//   names the right element type; the body is STLport's `__uninitialized_copy`, not Dinkumware's `_Uninit_copy`).
inline void* operator new(unsigned int, void* where) { return where; }

class CActiveEntityScriptBase;
struct SCountBlock { long RefCount; };

struct CCountedPointer_CActiveEntityScriptBase {
    CActiveEntityScriptBase* Data;                     // +0
    SCountBlock* Info;                                 // +4
    CCountedPointer_CActiveEntityScriptBase(const CCountedPointer_CActiveEntityScriptBase& other)
        : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};

struct SEntityScriptEntry {                            // pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> >
    unsigned long KeyLow;                              // +0x0
    unsigned long KeyHigh;                             // +0x4
    CCountedPointer_CActiveEntityScriptBase Script;    // +0x8
    SEntityScriptEntry(const SEntityScriptEntry& other)
        : KeyLow(other.KeyLow), KeyHigh(other.KeyHigh), Script(other.Script) { }
};

struct __false_type { __false_type() { } };

SEntityScriptEntry* __fastcall UninitCopy_PairU64ActiveEntityScript_00cba3e0(const SEntityScriptEntry* first, const SEntityScriptEntry* last, SEntityScriptEntry* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) SEntityScriptEntry(*first);
    return result;
}