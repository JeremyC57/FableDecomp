// CSmallVector<const CANavQuadTreeNode*,8>::resize(long n, const const CANavQuadTreeNode*& fill) -- grow or shrink the node-pointer vector to n entries (copy the kept prefix, fill new slots, free the old block)
// Leaf: ?resize@
// Retail: Fable.exe @ 0x00A7DB10, 179 bytes, `ret 8`. FableWin.pdb ?resize@?$CSmallVector@PBVCANavQuadTreeNode@@$07@@QAEXJABQBVCANavQuadTreeNode@@@Z.
// Layout: Ego_r.pdb CSmallVector<T,8> = Start (T*, +0) and Size (+4, signed char); the shared model below is the
//   one all twelve CSmallVector rows of this batch match with (resize/push_back/erase public, Clear/Allocate/Copy protected).
// Shape: Clear() is inline here (delete[] + the zero stores; the stores before the reassignment are dead and dropped).
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
template <class T, int N> void CSmallVector<T, N>::resize(long n, const T& fill)
{
    if (n > 0)
    {
        T* next = new T[n];
        for (int i = 0; i < Min(n, GetSize()); ++i)
            next[i] = Start[i];
        int extra = n - Size;
        if (extra <= 0x7fffffff)
        {
            if (extra > 0)
                for (int j = 0; j < n - Size; ++j)
                    next[Size + j] = fill;
            Clear();
            Start = next;
            Size = n;
        }
    }
    else
        Clear();
}
template class CSmallVector<const CANavQuadTreeNode*, 8>;