// std::__destroy_aux<std::pair<EGameAction, CCountedPointer<const CInputTypeBase> >*>(std::pair<EGameAction, CCountedPointer<const CInputTypeBase> >* first, std::pair<EGameAction, CCountedPointer<const CInputTypeBase> >* last, const __false_type&) -- destroy a range of counted-pointer elements
// Leaf: ??$__destroy_aux@PAU?$pair@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@std@@@std@@YIXPAU?$pair@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@0@0ABU__false_type@0@@Z
// Retail: Fable.exe @ 0x004488B0, 77 bytes, __fastcall (first ecx, last edx, the __false_type tag on the stack, `ret 4`);
// callers include the CGamePlayerInterface destructor (0x00446C9A on the Controls CVectorMap<EGameAction, CCountedPointer<const
// CInputTypeBase>> at +0x7a8); 11 callers in all. STLport _Destroy(first, last) for a 12-byte element whose only non-trivial member is a
// CCountedPointer (inline release: if Info, --RefCount, at zero DeleteFunc(Data) __fastcall + operator delete(Info)
// 0x00BFE9BC; Data = Info = 0). The catalog label pair<long, CCountedPointer<CRegionConnectionNode>> names a byte-identical (folded) twin; the element type here follows the CGamePlayerInterface destructor.
// Layout: pair {EGameAction first +0, CCountedPointer second +4/+8}.
// Shape: /O2 (16-aligned entry, loop-head pad).
struct CCPPointerInfo {
    long  RefCount;
    void  (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                // +0x00
    CCPPointerInfo* Info;                                // +0x04
    ~CCountedPointer()
    {
        if (Info)
        {
            --Info->RefCount;
            if (Info->RefCount == 0)
            {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CInputTypeBase;
enum EGameAction { GAME_ACTION_NONE };
namespace std {
struct __false_type {};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class ForwardIter>
void __fastcall __destroy_aux(ForwardIter first, ForwardIter last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
template void __fastcall __destroy_aux<std::pair<EGameAction, CCountedPointer<const CInputTypeBase> >*>(std::pair<EGameAction, CCountedPointer<const CInputTypeBase> >*, std::pair<EGameAction, CCountedPointer<const CInputTypeBase> >*, const __false_type&);
}