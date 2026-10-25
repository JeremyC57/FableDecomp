// std::__destroy_aux(pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> >* first, last, const __false_type&) -- run the pair destructor (the counted-pointer release) on [first, last)
// Leaf: ??$__destroy_aux@PAU?$pair@_KV?$CCountedPointer@VCActiveEntityScriptBase@@@@@std@@@std@@YIXPAU?$pair@_KV?$CCountedPointer@VCActiveEntityScriptBase@@@@@0@0ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBB1B0, 80 bytes, `__fastcall` (first ecx, last edx, the tag by reference on the stack,
//   never read; `ret 4`); the inlined CCountedPointer release runs on the count-block induction register (`lea esi,[edi+0Ch]`),
//   operator delete 0x00BFE9BC for the block.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 451 "0xCBB1B0/0xCBB2D0 carry Dinkumware names in an STLport TU"): the
//   landed CScriptBase::RunEntityThreads 0x00CB8170 calls it from STLport 4.6 `_Destroy(first, last)` -> `__destroy` ->
//   `__destroy_aux(first, last, __false_type())` and binds this Leaf -> 00cbb1b0 in reloc_bindings; the element is the
//   EntityScripts map's pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > (16 bytes: the 8-byte key, the
//   counted pointer {Data +8, Info +0xC}), Ego_r.pdb CScriptBase::EntityScripts. Fourteen call sites 0x00CB80DA ..
//   0x00CBD56A in the script-base TU. Dinkumware's `_Destroy_range(first, last, allocator&)` (the landed name) has the
//   same calling shape. Namespace std (owner decision 2026-10-25).
// Model: the landed row's CCountedPointer release and the 0x00CB8170 row's STLport pieces.
struct CCPPointerInfo {                             // CCountedPointer count block
    long  RefCount;                                 // +0x00
    void  (__fastcall* DeleteFunc)(void* data);     // +0x04
    void* Data;                                     // +0x08
};
template <class T> class CCountedPointer {
public:
    T*              Data;                           // +0x00
    CCPPointerInfo* Info;                           // +0x04
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CActiveEntityScriptBase;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                      // +0x00
    _T2 second;                                     // +0x08 (8-aligned after the __int64 key)
};
struct __false_type { __false_type() { } };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)   // _construct.h
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template void __fastcall __destroy_aux(pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> >*,
                                       pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> >*, const __false_type&);
}
