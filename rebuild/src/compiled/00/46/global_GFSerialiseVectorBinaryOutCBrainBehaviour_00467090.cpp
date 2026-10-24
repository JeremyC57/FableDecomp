// GFSerialiseVectorBinaryOut<CBrainBehaviour>(CDataOutputStream& stream, vector<CBrainBehaviour>& value) -- write the element count, then each behaviour's name index and two priorities through CPersistTraits<CBrainBehaviour>
// Leaf: ??$GFSerialiseVectorBinaryOut@VCBrainBehaviour@@@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00467090, 101 bytes, __fastcall (stream in ecx, vector in edx), plain `ret`; size TU
//         with an EBP frame (`and [ebp-4],0`, `leave`), y-off + s like the landed traits constructor it calls.
// Callees: CPersistTraits<CBrainBehaviour>::CPersistTraits() @ 0x004670F5 (landed: DefaultVal("") -> CDefString::SetString,
//          zero priorities), CDataOutputStream::WriteLong(long) @ 0x00993EB0 (the count and the three members).
// Identity: Ego_r.pdb name set ??$GFSerialiseVectorBinaryOut@VCBrainBehaviour@@@@YI...; PDB CBrainBehaviour (12 bytes) =
//   { CDefString Name (long TablePos); long Priority; long ScriptAIPriority; }. Its only caller is the landed binary transfer
//   0x00467062 (call site 0x0046707D), whose load arm calls GFSerialiseVectorBinaryIn<CBrainBehaviour> 0x0046712B. The catalog
//   label <CActionInputControl> is propagated.
// Layout: engine/CDataOutputStream.h with a methods view; retail STLport vector {start, finish, end_of_storage}.
// Shape: the landed 0x005C12A1 template (traits first, so its out-of-line constructor runs before the count): count =
//   size() (`idiv` by 12), WriteLong, then the inline traits transfer per element: the name writes its table position
//   (CDefString::TransferBinaryOut inline) and the two priorities follow. The traits keeps its 12-byte frame slot (its
//   constructor takes its address); the byte offset and the down-counter live in [ebp-4] / [ebp-8].
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CDataOutputStream.h"

struct CDataOutputStreamWriter : CDataOutputStream {
    void WriteLong(long value);                             // 0x00993EB0
};

class CDefString {
public:
    long TablePos;                                          // +0x00
    __forceinline void TransferBinaryOut(CDataOutputStreamWriter& stream) const { stream.WriteLong(TablePos); }
};

class CBrainBehaviour {                                    // 12 bytes
public:
    CDefString Name;                                        // +0x00
    long       Priority;                                    // +0x04
    long       ScriptAIPriority;                            // +0x08
};

namespace std {
template <class _Tp> class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Tp* begin() { return _M_start; }
    size_type size() const { return size_type(_M_finish - _M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
};
}

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CBrainBehaviour> {
    CBrainBehaviour DefaultVal;
    CPersistTraits();                                       // 0x004670F5
    __forceinline void TransferBinaryOut(CDataOutputStreamWriter& stream, const CBrainBehaviour* value) const
    {
        value->Name.TransferBinaryOut(stream);
        stream.WriteLong(value->Priority);
        stream.WriteLong(value->ScriptAIPriority);
    }
};

template <class T>
void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStreamWriter& stream, std::vector<T>& value)
{
    CPersistTraits<T> traits;
    long count = value.size();
    stream.WriteLong(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryOut(stream, &value[i]);
}

template void __fastcall GFSerialiseVectorBinaryOut<CBrainBehaviour>(CDataOutputStreamWriter& stream, std::vector<CBrainBehaviour>& value);