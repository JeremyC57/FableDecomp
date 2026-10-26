// CSmallVector<CThingComponentSet::CEntry,8>::Copy(const CSmallVector<CThingComponentSet::CEntry,8>& other) -- make this a copy of `other`: Allocate(other.Size) then element-wise CEntry copy (dword, dword, byte)
// Leaf: ?Copy@
// Retail: Fable.exe @ 0x00463AE0, 80 bytes, `ret 4`. FableWin.pdb ?Copy@?$CSmallVector@VCEntry@CThingComponentSet@@$07@@IAEXABV1@@Z (protected);
//   Allocate is the out-of-line 0x00463B30.
// Layout: Ego_r.pdb CSmallVector<T,8> = Start (T*, +0) and Size (+4, signed char); the shared model below is the
//   one all twelve CSmallVector rows of this batch match with (resize/push_back/erase public, Clear/Allocate/Copy protected).
// Shape: `unsigned long i < other.Size` gives retail's unsigned `jb`; the packed 9-byte CEntry's implicit operator= copies 4+4+1.
void* __cdecl operator new[](unsigned int size);          // 0x00BFEB22 (MSVCR71 ??_U thunk; the lane <new> lacks it)
void __cdecl operator delete[](void* block);              // 0x00BFEB1C (MSVCR71 ??_V thunk)
#pragma pack(push, 1)
class CDefString { public:
    long TablePos;                                        // +0x00
    CDefString() : TablePos(-1) { }
};
class CThingComponentSet {
public:
    class CEntry { public:                                       // Ego_r.pdb: 9 bytes, packed
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
    void Allocate(unsigned long n);                       // 0x00463B30 (out of line)
    void Copy(const CSmallVector& other);
};
template <class T, int N> void CSmallVector<T, N>::Copy(const CSmallVector& other)
{
    Allocate(other.Size);
    for (unsigned long i = 0; i < other.Size; ++i)
        Start[i] = other.Start[i];
}
template class CSmallVector<CThingComponentSet::CEntry, 8>;