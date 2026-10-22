// CPersistContext_TransferableVector<CDefString>::TransferOut(CPersistContext& context, CCharString& out, long index) -- writes element `index` of the CDefString vector through an empty CPersistTraits<CDefString> (tail-jumps to 0x007E3930)
// Leaf: ?TransferOut@?$CPersistContext_TransferableVector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F334A0, a slot of the no-RTTI vtable at 0x01301C1C (slots: Size
// 0x00F33470, Resize, TransferOut 0x00F334A0, TransferIn 0x00F33480 -- the PDB method set of
// CPersistContext_TransferableVector<CDefString>, `?TransferOut@?$CPersistContext_TransferableVector@VCDefString@@@@...`).
// The wrapper is built by CPersistContext::Transfer<CDefString>(name, vector<CDefString>&) 0x00F333B0 for its
// text-load (0x009BB110) and text-save (0x009BAC80) arms. CDefString is one long (4-byte elements, `lea eax,[ecx+edx*4]`).
// The manifest label `CPersistContext_TransferableVector::vfunc_N` is propagated.
// Shape: retail TAIL-JUMPS (`mov [esp+0Ch],eax ; lea ecx,[esp+0Ch] ; jmp`): the empty traits object is homed in the
// dead index slot, which also becomes the callee's third argument. VC7.1 only emits that tail jump when the traits
// writer's body sits EARLIER in the same TU (an out-of-line `#pragma auto_inline(off)` stand-in that never touches
// `this`); with the writer only declared it keeps `push ; push ; push ; call ; ret 0Ch` (35 bytes v 25).
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

// Same-TU stand-in for the retail writer 0x007E3930 (it formats the def name of the index); the lander binds the
// call to the retail address.
void FormatDefStringValue(CPersistContext& context, CCharString& out, CDefString* value);
#pragma auto_inline(off)
void CPersistTraits<CDefString>::TransferOut(CPersistContext& context, CCharString& out, CDefString* value)
{
    FormatDefStringValue(context, out, value);
}
#pragma auto_inline(on)

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
void CPersistContext_TransferableVector<T>::TransferOut(CPersistContext& context, CCharString& out, long index)
{
    CPersistTraits<T> traits;
    traits.TransferOut(context, out, &(*TransVector)[index]);
}

template void CPersistContext_TransferableVector<CDefString>::TransferOut(CPersistContext& context, CCharString& out, long index);
