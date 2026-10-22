// CPersistContext_TransferableVector<CDefString>::Size() -- the element count `TransVector->size()` ((last - first) >> 2)
// Leaf: ?Size@?$CPersistContext_TransferableVector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F33470, a slot of the no-RTTI vtable at 0x01301C1C (slots: Size
// 0x00F33470, Resize, TransferOut 0x00F334A0, TransferIn 0x00F33480 -- the PDB method set of
// CPersistContext_TransferableVector<CDefString>, `?Size@?$CPersistContext_TransferableVector@VCDefString@@@@...`).
// The wrapper is built by CPersistContext::Transfer<CDefString>(name, vector<CDefString>&) 0x00F333B0 for its
// text-load (0x009BB110) and text-save (0x009BAC80) arms. CDefString is one long (4-byte elements, `lea eax,[ecx+edx*4]`).
// The manifest label `CPersistContext_TransferableVector::vfunc_N` is propagated.
// Shape: `mov ecx,[ecx+4] ; mov eax,[ecx+4] ; sub eax,[ecx] ; sar eax,2`.
struct CPersistContext;
struct CStringParser;
struct CCharString;

class CDefString {
public:
    long Index;
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CDefString> {      // no members: retail never builds a DefaultVal for it here
    void TransferIn(CPersistContext& context, CStringParser& parser, CDefString* value);   // 0x007E3820
    void TransferOut(CPersistContext& context, CCharString& out, CDefString* value);       // 0x007E3930
};

template <class T> struct vector {                   // STLport, 12 bytes in retail
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    T& operator[](long index) { return _M_start[index]; }
};

struct CPersistContext_TransferableVectorGenericType {
    virtual long Size() = 0;
    virtual void Resize(long count) = 0;
    virtual void TransferOut(CPersistContext& context, CCharString& out, long index) = 0;
    virtual void TransferIn(CPersistContext& context, CStringParser& parser, long index) = 0;
};

template <class T> struct CPersistContext_TransferableVector : CPersistContext_TransferableVectorGenericType {
    vector<T>* TransVector;                          // +0x4
    virtual long Size();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext& context, CCharString& out, long index);
    virtual void TransferIn(CPersistContext& context, CStringParser& parser, long index);
};

template <class T>
long CPersistContext_TransferableVector<T>::Size()
{
    return TransVector->size();
}

template long CPersistContext_TransferableVector<CDefString>::Size();
