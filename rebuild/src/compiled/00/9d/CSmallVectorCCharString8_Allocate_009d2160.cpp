// CSmallVector<CCharString,8>::Allocate(unsigned long n) -- drop the current array (Clear) and allocate n default-constructed strings with new[] (count cookie), storing the byte-sized count
// Leaf: ?Allocate@
// Retail: Fable.exe @ 0x009D2160, 99 bytes (+ int3 padding), `ret 4`. FableWin.pdb ?Allocate@?$CSmallVector@VCCharString@@$07@@IAEXK@Z
//   (protected); catalogued as "CSmallVector_AllocateHeap" (descriptive). Callees: Clear 0x009D2110 (landed
//   CSmallVectorCCharString8_Clear_009d2110), MSVCR71 operator new[] 0x00BFEB22, CCharString::CCharString 0x0099E4B0.
//   Caller of note: Copy 0x009D21D0 (landed).
// Layout: Ego_r.pdb CSmallVector<T,8> = Start (T*, +0) and Size (+4, signed char) -- the shared model of the landed
//   CSmallVector rows (Allocate 0x00463B30 has the same body for CThingComponentSet::CEntry).
// Shape: `new T[n]` over a class with a user constructor and destructor writes the count cookie (`n*4+4`) and inlines the
//   construct loop (`lea eax,[ebx-1] ; test ; jl ; inc eax` countdown, no EH frame without /EHsc); a null result skips
//   the loop and stores 0; `Size = n` truncates to the byte (`mov [edi+4],bl`) on both paths.
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
    void Allocate(unsigned long n);                       // 0x009D2160 (this row)
    void Copy(const CSmallVector& other);
};
template <class T, int N> void CSmallVector<T, N>::Allocate(unsigned long n)
{
    Clear();
    Start = new T[n];
    Size = n;
}
template class CSmallVector<CCharString, 8>;