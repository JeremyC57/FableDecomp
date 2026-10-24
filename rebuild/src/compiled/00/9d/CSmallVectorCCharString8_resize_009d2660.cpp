// CSmallVector<CCharString,8>::resize(long n, const CCharString& fill) -- grow or shrink the string vector to n entries: copy the kept prefix into a fresh array, fill new slots with `fill`, release the old block
// Leaf: ?resize@
// Retail: Fable.exe @ 0x009D2660, 210 bytes, `ret 8`. FableWin.pdb ?resize@?$CSmallVector@VCCharString@@$07@@QAEXJABVCCharString@@@Z; the manifest label
//   (CANavQuadTreeNode const*) is propagated -- the body builds CCharString elements (`new T[n]` with a count cookie and the
//   `vector constructor iterator` 0x004038C0 over CCharString::CCharString 0x0099E4B0) and copies with CCharString::operator=
//   0x0099EFB0, and its Clear is 0x009D2110 (the CCharString destroy loop).
// Layout: Ego_r.pdb CSmallVector<T,8> = Start (T*, +0) and Size (+4, signed char); the shared model below is the
//   one all twelve CSmallVector rows of this batch match with (resize/push_back/erase public, Clear/Allocate/Copy protected).
// Shape: Clear() is declared only (it is the out-of-line 0x009D2110): with its body visible VC7.1 inlines the destroy loop in one
//   of the two call sites and the register allocation changes.
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
    void Clear();                                         // 0x009D2110 (out of line)
    void Allocate(unsigned long n);
    void Copy(const CSmallVector& other);
};
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
template class CSmallVector<CCharString, 8>;