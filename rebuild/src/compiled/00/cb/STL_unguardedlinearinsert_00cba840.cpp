// _STL::__unguarded_linear_insert(CActiveScriptPair* last, CActiveScriptPair val, CActiveScriptLess comp) -- shift every earlier (UID, active entity script) pair with a larger UID up one slot and drop val into the gap
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBA840, 150 bytes (`ret 14h`: by-value 16-byte val + comparator slot).
// TU: the scripting library's STLport 4.x instantiations (0x00CB6xxx-0x00CBDxxx); the game is built /Gr, so the
//   algorithm templates are __fastcall (first two pointer arguments in ecx/edx, callee pops the rest).
// Identity: STLport __unguarded_linear_insert over (UID, active entity script) = _STL::pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > (0x10 bytes);
//   callers __unguarded_insertion_sort_aux 0x00CBB2A4 and __linear_insert 0x00CBC390. The manifest label
//   "SpeechFrame_InsertSorted" is propagated and wrong.
// Callees: pair operator= 0x00CBA520 (VC7.1 emits the implicit pair assignment out of line: UID copy plus the inline
//   CCountedPointer::operator=), operator delete 0x00BFE9BC (val's release; the count block's delete function via +4).
// Shape: `*__last = *__next` in the loop; the u64 compare is high dword first; val's destructor releases at the end.
//   CCountedPointer is modelled with the release form the D rows need (decrement, re-read, test, then Data = Info = 0):
//   VC7.1 emits the same bytes for this row as with the short `if (Info && --Info->RefCount == 0)` form.
void __cdecl operator delete(void* block);                 // 0x00BFE9BC
struct CCPPointerInfo {                                    // CCountedPointer count block
    long  RefCount;                                        // +0x00
    void  (__fastcall* DeleteFunc)(void* data);            // +0x04
    void* Data;                                            // +0x08
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                  // +0x00
    CCPPointerInfo* Info;                                  // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    operator delete(Info);
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
class CActiveEntityScriptBase;
namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
template <class K, class V> class CKeyPairCompareLess {    // empty, passed by value (4-byte slot)
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};
typedef _STL::pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > CActiveScriptPair;   // 16 bytes
typedef CKeyPairCompareLess<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > CActiveScriptLess;
namespace _STL {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}
template void __fastcall __unguarded_linear_insert<CActiveScriptPair*, CActiveScriptPair, CActiveScriptLess>(CActiveScriptPair*, CActiveScriptPair, CActiveScriptLess);
}