// CSmallVector<CThingComponentSet::CEntry,8>::Allocate(unsigned long n) -- drop the current array and allocate n default entries (Name -1, DriverType 0, Override false)
// Leaf: ?Allocate@
// Retail: Fable.exe @ 0x00463B30, 109 bytes, `ret 4`. FableWin.pdb ?Allocate@?$CSmallVector@VCEntry@CThingComponentSet@@$07@@IAEXK@Z (protected).
// Layout: Ego_r.pdb CSmallVector<T,8> = Start (T*, +0) and Size (+4, signed char); the shared model below is the
//   one all twelve CSmallVector rows of this batch match with (resize/push_back/erase public, Clear/Allocate/Copy protected).
// Shape: Clear() inline (delete[], zero stores); `new T[n]` over the inline CEntry ctor becomes the null-checked count loop storing
//   -1/0/0 in member order (no cookie: CEntry has no destructor); Size is stored before Start, as retail does.
void* __cdecl operator new[](unsigned int size);          // 0x00BFEB22 (MSVCR71 ??_U thunk; the lane <new> lacks it)
void __cdecl operator delete[](void* block);              // 0x00BFEB1C (MSVCR71 ??_V thunk)
#pragma pack(push, 1)
struct CDefString {
    long TablePos;                                        // +0x00
    CDefString() : TablePos(-1) { }
};
class CThingComponentSet {
public:
    struct CEntry {                                       // Ego_r.pdb: 9 bytes, packed
        CDefString Name;                                  // +0x00
        unsigned long DriverType;                         // +0x04
        bool Override;                                    // +0x08
        CEntry() : DriverType(0), Override(false) { }
    };
};
#pragma pack(pop)
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
template <class T, int N> void CSmallVector<T, N>::Allocate(unsigned long n)
{
    Clear();
    Start = new T[n];
    Size = n;
}
template class CSmallVector<CThingComponentSet::CEntry, 8>;