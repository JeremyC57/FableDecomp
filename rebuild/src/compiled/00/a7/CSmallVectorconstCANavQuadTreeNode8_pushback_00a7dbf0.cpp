// CSmallVector<const CANavQuadTreeNode*,8>::push_back(const const CANavQuadTreeNode*& value) -- append a node pointer: resize to Size+1 with a null fill, then store the value in the last slot
// Leaf: ?push_back@
// Retail: Fable.exe @ 0x00A7DBF0, 51 bytes, `ret 4`. FableWin.pdb ?push_back@?$CSmallVector@PBVCANavQuadTreeNode@@$07@@QAEXABQBVCANavQuadTreeNode@@@Z;
//   resize is the out-of-line 0x00A7DB10.
// Layout: Ego_r.pdb CSmallVector<T,8> = Start (T*, +0) and Size (+4, signed char); the shared model below is the
//   one all twelve CSmallVector rows of this batch match with (resize/push_back/erase public, Clear/Allocate/Copy protected).
// Shape: `T()` for a pointer is the zeroed `push ecx` frame slot passed by reference; the value is re-read after the call.
void* __cdecl operator new[](unsigned int size);          // 0x00BFEB22 (MSVCR71 ??_U thunk; the lane <new> lacks it)
void __cdecl operator delete[](void* block);              // 0x00BFEB1C (MSVCR71 ??_V thunk)
class CANavQuadTreeNode;
template <class T> inline const T& Min(const T& a, const T& b) { return a < b ? a : b; }
template <int N> struct CSmallVectorSize;
template <> struct CSmallVectorSize<8> { typedef signed char Type; };     // CSmallVector<T,8>: schar Size
template <> struct CSmallVectorSize<32> { typedef int Type; };            // CSmallVector<T,32>: int Size
template <class T, int N> class CSmallVector {
public:
    T* Start;                                             // +0x00
    typename CSmallVectorSize<N>::Type Size;              // +0x04
    long GetSize() const { return Size; }
    void resize(long n, const T& fill);                   // 0x00A7DB10 (out of line)
    void push_back(const T& value);
    T* erase(T* where);
    void Clear();
    void Allocate(unsigned long n);
    void Copy(const CSmallVector& other);
};
template <class T, int N> void CSmallVector<T, N>::push_back(const T& value)
{
    resize(Size + 1, T());
    Start[Size - 1] = value;
}
template class CSmallVector<const CANavQuadTreeNode*, 8>;