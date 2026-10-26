// std::__destroy_aux(pair<unsigned long, CCountedPointer<CFaction> >* first, pair<unsigned long, CCountedPointer<CFaction> >* last, const __false_type&) -- STLport _Destroy over the range: only the counted pointer has a destructor (release: --RefCount, at zero DeleteFunc(Data) and delete Info; Data = Info = 0)
// Leaf: ??$__destroy_aux@PAU?$pair@KV?$CCountedPointer@VCFaction@@@@@std@@@std@@YIXPAU?$pair@KV?$CCountedPointer@VCFaction@@@@@0@0ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AD9E0, 77 bytes (+ int3 padding), __fastcall (first ecx, last edx, the tag by reference
//   on the stack, never read), `ret 4`; operator delete 0x00BFE9BC.
// Identity (proven): std::__destroy_aux<std::pair<unsigned long, CCountedPointer<CFaction> >*>; evidence: it is the _M_clear destroy callee of the proven std::vector<std::pair<unsigned long, CCountedPointer<CFaction> > >::_M_insert_overflow 0x006BAA90 (agent951 G055); 14 callers in all (the vector's other members in the same TU).
// Label: the manifest's "_Destroy_range<std::allocator<std::pair<long,CCountedPointer<CRegionConnectionNode>_>_>_>" is a propagated Dinkumware spelling (family J074: five byte-identical
//   bodies, each its own COMDAT -- retail has no identical-code folding).
// Model: the landed 0x00A80E40 (pair<long, CCountedPointer<CNavConnectorNode> >) verbatim with this row's element; CCountedPointer's
//   inline destructor is the engine's release (Ego_r.pdb CCountedPointer<T> { T* Data; CCPPointerInfo* Info; }).
inline void* operator new(unsigned int, void* p) { return p; }
class CCPPointerInfo {                                     // 12 bytes
public:
    unsigned long RefCount;                                // +0x00
    void (__fastcall* DeleteFunc)(void* data);             // +0x04
    void* Data;                                            // +0x08
};
template <class T> class CCountedPointer {                 // 8 bytes
public:
    T* Data;                                               // +0x00
    CCPPointerInfo* Info;                                  // +0x04
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
class CFaction;
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
typedef pair<unsigned long, CCountedPointer<CFaction> > _Pair;
template void __fastcall __destroy_aux<_Pair*>(_Pair*, _Pair*, const __false_type&);
}