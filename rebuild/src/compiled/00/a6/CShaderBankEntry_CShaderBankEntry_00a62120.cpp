// CShaderBankEntry::CShaderBankEntry(unsigned long type, unsigned long index, CShaderDataBank* bank) -- read the bank entry's
//   data into a zeroed char buffer, wrap it in a memory stream and read the shader: the code size in bytes and the code
//   dwords, the constant-name count and each null-terminated name, then the vertex-shader constant layout name.
// Leaf: ??0CShaderBankEntry@@QAE@KKPAVCShaderDataBank@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A62120, 688 bytes (678 + int3 padding), `ret 0Ch`; vtable 0x0129D698.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0 (base), CCharString() 0x0099E4B0,
//   CBankFile::GetEntryDataSize 0x009CC240, malloc 0x00BFEA0E, CBankFile::ReadEntryData 0x009CC2A0,
//   CMemoryDataInputStream(const void*, ulong) 0x00994930, CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0,
//   CArray<unsigned long>::resize(n, const T&) 0x00464931, CMemoryDataInputStream::ReadNullTerminatedString 0x00995D30
//   (devirtualised on the local stream), CCharString(const CCharString&) 0x0099EC30, vector<CCharString>::
//   _M_insert_overflow 0x00433530, ~CCharString 0x0099EAE0, CCharString::operator= 0x0099EFB0,
//   ~CMemoryDataInputStream 0x00993B10, free 0x00BFEA14.
// Layout: Ego_r.pdb CShaderBankEntry (44 bytes, 16-byte CArrays) -> retail 0x24 bytes (12-byte STLport vectors, as the
//   landed CShaderDataBank::NotifyEntryAdded 0x00A623D0): ShaderType +0x4, ShaderCode +0x8, ShaderConstants +0x14,
//   VSConstantLayout +0x20. CMemoryDataInputStream as the landed CWoundMorphs::TransferBinaryIn 0x00757990 (0x1c bytes on
//   the frame here).
// Shape: the counts are read through a by-value inline reader (the value lives in the dead `index` slot only on the
//   overflow path); the char buffer is STLport vector(n) (finish = uninitialized_fill_n's `first + n`); the vectors'
//   end-of-storage is the allocator proxy with its own ctor (member-init store order); CArray::GetData is
//   `size() > 0 ? _M_start : this`; push_back is inline with a `__false_type()` temporary passed to a same-TU member-template
//   _M_insert_overflow body (the empty-tag rule: the tag shares the name's slot, as retail).
// NF134 (2026-11-20, q6182): the local CMemoryDataInputStream constructor declaration respelled in place to the PDB
//   ??0CMemoryDataInputStream@@QAE@PBXK@Z (Ego_r.pdb 0x00AFCA20 + FableWin.pdb 0x030849C0; engine/CMemoryDataInputStream.h); was PAXK.
// NF188 (2026-11-20, q4251): the row-local CDataInputStream / CMemoryDataInputStream models (a user-declared destructor that
//   bound a CMemoryDataInputStream dtor spelling at 0x00993B10) are replaced by the shared engine/CDataInputStream.h /
//   engine/CMemoryDataInputStream.h classes, whose compiler-generated destructor calls ~CDataStream 0x00993B10 directly
//   (??1CDataStream@@UAE@XZ, Ego_r.pdb 0x00AFBBB0 + FableWin.pdb 0x03080440; engine/CDataInputStream.h retry11a note).
#include "fable_compiler.h"
// Naming (NF129, 2026-11-20, q4249): ReadNullTerminatedString is virtual in both PDBs (?ReadNullTerminatedString@CMemoryDataInputStream@@UAE?AVCCharString@@XZ,
//   Ego_r 0x00AFDBE0 / FableWin 0x03084F50), overriding CDataInputStream's virtual; the local model's vtable data member becomes the
//   base's vptr (in-place respell, base subobject still at +0), and VC7.1 still calls 0x00995D30 directly on the local stream.
// NF48 (round 7): the three arrays are the shared engine/CArray.h (public STLport 4.6.2 std::vector<T>; a local CArray template with a
// same-TU _M_insert_overflow body and a CCharBuffer stand-in before): CArray<char>(n) is STLport vector(n) with the char fill as memset
// (FABLE_RETAIL_CARRAY, FABLE_RETAIL_STLP_FILL_CHAR), GetData is CArray::GetAsCArray, resize(n, x) is the out-of-line
// vector<unsigned long>::resize 0x00464931 (FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE), push_back the header's inline body over
// vector<CCharString>::_M_insert_overflow 0x00433530 (its stl/_vector.c body visible, called out of line).
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_FILL_CHAR
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#include "stlport/fable_containers.h"
#include "engine/CArray.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
extern "C" void* __cdecl memset(void* dest, int value, unsigned int bytes);
#pragma intrinsic(memcpy, memset)
extern "C" void* __cdecl malloc(unsigned int bytes);
extern "C" void __cdecl free(void* p);


class CCharString { public:
    void* PStringData;
    CCharString();                                           // 0x0099E4B0
    CCharString(const CCharString& other);                   // 0x0099EC30
    ~CCharString();                                          // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);        // 0x0099EFB0
};




#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#include "engine/CMemoryDataInputStream.h"

class CBankFile { public:
    unsigned long GetEntryDataSize(unsigned long index) const;                                   // 0x009CC240 ?GetEntryDataSize@CBankFile@@QBEKK@Z (NF72, q10882: Ego_r.pdb const)
    void ReadEntryData(unsigned long index, unsigned long offset, void* buffer, unsigned long size);   // 0x009CC2A0
};
class CShaderDataBank : public CBankFile { public:};


class CShaderBankEntry : public CBaseClassNonCopyable { public:
    unsigned long          ShaderType;                       // +0x04
    CArray<unsigned long>  ShaderCode;                       // +0x08
    CArray<CCharString>    ShaderConstants;                  // +0x14
    CCharString            VSConstantLayout;                 // +0x20
    CShaderBankEntry(unsigned long type, unsigned long index, CShaderDataBank* bank);
    virtual ~CShaderBankEntry();
};

CShaderBankEntry::CShaderBankEntry(unsigned long type, unsigned long index, CShaderDataBank* bank)
    : ShaderType(type)
{
    unsigned long size = bank->GetEntryDataSize(index);
    CArray<char> data(size);
    bank->ReadEntryData(index, 0, data.GetAsCArray(), size);
    CMemoryDataInputStream stream(data.GetAsCArray(), size);
    unsigned long codeCount = stream.ReadULONG() / 4;
    ShaderCode.resize(codeCount, 0);
    unsigned long* code = ShaderCode.GetAsCArray();
    long bytes = codeCount * sizeof(unsigned long);
    if (bytes > 0)
        stream.Read(code, bytes);
    unsigned long count = stream.ReadULONG();
    for (unsigned long i = 0; i < count; ++i) {
        CCharString name = stream.ReadNullTerminatedString();
        ShaderConstants.push_back(name);
    }
    VSConstantLayout = stream.ReadNullTerminatedString();
}