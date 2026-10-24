// CSmallVector<CCharString,8>::Clear() -- release the string array (destroy each element back to front, free the cookie block) and reset to empty
// Leaf: ?Clear@
// Retail: Fable.exe @ 0x009D2110, 80 bytes. FableWin.pdb ?Clear@?$CSmallVector@VCCharString@@$07@@IAEXXZ (protected). Callees:
//   CCharString::~CCharString 0x0099EAE0, operator delete[] 0x00BFEB1C.
// Layout: Ego_r.pdb CSmallVector<T,8> = Start (T*, +0) and Size (+4, signed char); the shared model below is the
//   one all twelve CSmallVector rows of this batch match with (resize/push_back/erase public, Clear/Allocate/Copy protected).
// Shape: `delete[] Start` over a class with a user destructor inlines the count-cookie destroy loop (`mov eax,[ecx-4]` ... `dec ebp ; jne`).
void* __cdecl operator new[](unsigned int size);          // 0x00BFEB22 (MSVCR71 ??_U thunk; the lane <new> lacks it)
void __cdecl operator delete[](void* block);              // 0x00BFEB1C (MSVCR71 ??_V thunk)
class CCharString {                                       // one pointer to the shared string data
public:
    void* PData;                                          // +0x00
    CCharString();                                        // 0x0099E4B0
    ~CCharString();                                       // 0x0099EAE0
    CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};
template <class T> inline const T& Min(const T& a, const T& b) { return a < b ? a : b; }
template <int N> struct CSmallVectorSize;
template <> struct CSmallVectorSize<8> { typedef signed char Type; };     // CSmallVector<T,8>: schar Size
template <> struct CSmallVectorSize<32> { typedef int Type; };            // CSmallVector<T,32>: int Size
template <class T, int N> class CSmallVector {
public:
    T* Start;                                             // +0x00
    typename CSmallVectorSize<N>::Type Size;              // +0x04
    long GetSize() const { return Size; }
    void resize(long n, const T& fill);
    void push_back(const T& value);
    T* erase(T* where);
    void Clear();
    void Allocate(unsigned long n);
    void Copy(const CSmallVector& other);
};
template <class T, int N> void CSmallVector<T, N>::Clear()
{
    delete[] Start;
    Start = 0;
    Size = 0;
}
template class CSmallVector<CCharString, 8>;