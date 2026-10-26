// _global::UninitCopy_CStreamInfo_00ab4450(const CStreamInfo* first, const CStreamInfo* last, CStreamInfo* result, const __false_type&) -- STLport __uninitialized_copy<CVertexShaderInput::CStreamInfo*>: placement copy-construct stream infos (index, then the vertex stream format with its element array), return the advanced result
// Leaf: UninitCopy_CStreamInfo_00ab4450
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB4450, length 61 bytes (+3 int3 pad), `ret 8`.
// Callers: 0x00AB459A / 0x00AB45F6 in the landed vector<CVertexShaderInput::CStreamInfo>::_M_insert_overflow 0x00AB4540
//   (its same-TU stand-in "UninitializedCopy" is this body).
// Callees: CArray<CVertexStreamFormat::CDataElement>::CArray(const CArray&) @ 0x00A5E0A0.
// Identity: the catalog label `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, ...>` is propagated. Vtable
//   0x0129D578 is CVertexStreamFormat (rtti-vtables.tsv, slot 0 = deleting dtor 0x00A5E670). Layout as the landed
//   caller: CStreamInfo (20 bytes) = StreamIndex +0, StreamFormat +4; CVertexStreamFormat (16) = vptr +0, DataElements
//   (12-byte retail CArray) +4. Byte twin of 0x00A5E7F0 (the same instance in the skeletal-morph TU).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx -> edi,
//   last in edx -> ebx, result + the tag reference on the stack, result -> esi). The inline copy constructors store the
//   index, the derived vtable (the CBaseClass vtable store is dead), then call the array's out-of-line copy
//   constructor on `this+8`; placement-new null test on the destination. The named `cur = result` copy makes VC7.1 load
//   result into esi before the empty-range test and return it from one shared epilogue (GOTCHAS 009f6c00); looping on
//   `result` itself gives a second epilogue that reloads the argument.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

class CBaseClass {
public:
    virtual ~CBaseClass();
};

template <class T> class CArray { public:              // retail 12-byte array: start, finish, end of storage
    T* start;
    T* finish;
    T* end;
    CArray(const CArray& other);                // 0x00A5E0A0 (for CDataElement)
};

class CVertexStreamFormat : public CBaseClass { // vtable 0x0129D578, 16 bytes
public:
    class CDataElement { public:                       // 8 bytes
        unsigned int Register;
        int          DataType;
    };
    CArray<CDataElement> DataElements;          // +0x04
    __forceinline CVertexStreamFormat(const CVertexStreamFormat& o) : DataElements(o.DataElements) { }
    virtual ~CVertexStreamFormat() { }
};

struct CStreamInfo {                            // CVertexShaderInput::CStreamInfo, 20 bytes
    unsigned int        StreamIndex;            // +0x00
    CVertexStreamFormat StreamFormat;           // +0x04
    __forceinline CStreamInfo(const CStreamInfo& o) : StreamIndex(o.StreamIndex), StreamFormat(o.StreamFormat) { }
};

CStreamInfo* __fastcall UninitCopy_CStreamInfo_00ab4450(const CStreamInfo* first, const CStreamInfo* last, CStreamInfo* result, const __false_type&)
{
    CStreamInfo* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) CStreamInfo(*first);
    return cur;
}