// std::__destroy_aux<CCountedPointer<const CInputTypeBase>*>(CCountedPointer<const CInputTypeBase>* first, CCountedPointer<const CInputTypeBase>* last, const __false_type&) -- destroy a range of counted-pointer elements
// Leaf: ??$__destroy_aux
// Retail: Fable.exe @ 0x00448860, 77 bytes, __fastcall (first ecx, last edx, the __false_type tag on the stack, `ret 4`);
// callers include the CGamePlayerInterface destructor (0x00446C1F / 0x00446C48 / 0x00446C71 on AssignableSpecialMoveButtons /
// QuickAccessItemButtons / ContextSensitiveItemButtons at +0x7b8 / +0x7c4 / +0x7d0); 16 callers in all. STLport _Destroy(first, last) for a 8-byte element whose only non-trivial member is a
// CCountedPointer (inline release: if Info, --RefCount, at zero DeleteFunc(Data) __fastcall + operator delete(Info)
// 0x00BFE9BC; Data = Info = 0). The catalog label _Destroy_range<pair<CCharString, CCountedPointer<CEntityScriptBindingBase>>> is wrong (8-byte element, no string destructor).
// Layout: CCountedPointer (Data +0, Info +4).
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
template void __fastcall __destroy_aux<CCountedPointer<const CInputTypeBase>*>(CCountedPointer<const CInputTypeBase>*, CCountedPointer<const CInputTypeBase>*, const __false_type&);
}