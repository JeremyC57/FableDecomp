// CSmallVector<CCharString,8>::Copy(const CSmallVector<CCharString,8>& other) -- make this a copy of `other`: Allocate(other.Size) then element-wise CCharString::operator=
// Leaf: ?Copy@
// Retail: Fable.exe @ 0x009D21D0, 68 bytes, `ret 4`. FableWin.pdb ?Copy@?$CSmallVector@VCCharString@@$07@@IAEXABV1@@Z (protected); the manifest
//   label (CThingComponentSet::CEntry) is propagated -- the elements are 4 bytes and copied with CCharString::operator=
//   0x0099EFB0. Allocate is the out-of-line 0x009D2160.
// Layout: Ego_r.pdb CSmallVector<T,8> = Start (T*, +0) and Size (+4, signed char); the shared model below is the
//   one all twelve CSmallVector rows of this batch match with (resize/push_back/erase public, Clear/Allocate/Copy protected).
// Shape: `unsigned long i < other.Size` gives retail's unsigned `jb` (the first test is `test al,al ; jbe` on the byte).
void* __cdecl operator new[](unsigned int size);          // 0x00BFEB22 (MSVCR71 ??_U thunk; the lane <new> lacks it)
void __cdecl operator delete[](void* block);              // 0x00BFEB1C (MSVCR71 ??_V thunk)
class CCharString {                                       // one pointer to the shared string data
public:
    void* PData;                                          // +0x00
    CCharString();                                        // 0x0099E4B0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);     // 0x0099EFB0
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
    void Allocate(unsigned long n);                       // 0x009D2160 (out of line)
    void Copy(const CSmallVector& other);
};
template <class T, int N> void CSmallVector<T, N>::Copy(const CSmallVector& other)
{
    Allocate(other.Size);
    for (unsigned long i = 0; i < other.Size; ++i)
        Start[i] = other.Start[i];
}
template class CSmallVector<CCharString, 8>;