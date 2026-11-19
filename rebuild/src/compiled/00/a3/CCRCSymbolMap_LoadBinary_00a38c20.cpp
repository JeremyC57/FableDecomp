// CCRCSymbolMap::LoadBinary(const CWideString& filename) -- read the whole file into a byte buffer one longer than the file, read the entry count from it, size the CRC -> symbol map to that count and fill each slot with the next two longs (CRC, symbol)
// Leaf: ?LoadBinary@CCRCSymbolMap@@QAEXABVCWideString@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A38C20, 551 bytes, `ret 4`, esp frame, no EH frame.
// Shape: FableWin.pdb ?LoadBinary@CCRCSymbolMap@@QAEXABVCWideString@@@Z (FableWin 0x031DB670), statement for statement, with the
//   Ego_d.pdb locals (infile, buffer, os, no_entries, c1, var1, var2). FableWin's `xor reg,reg ; je` before the loop is the debug
//   build's for-scope guard and emits nothing here.
// Callees: CDiskFileWin32(const CWideString&, FileAccessType, unsigned long) 0x0099AD80 / Read 0x0099A960 / Close 0x0099A920 (the inline
//   destructor), ~CWideString 0x0099B510, ~CBaseClassNonCopyable 0x0099A300, malloc 0x00BFEA0E / free 0x00BFEA14 (CArray<unsigned char>),
//   CDataInputStream() 0x00993B20 / ~CDataStream 0x00993B10, CMemoryDataInputStream::OpenMemorySource 0x00994570,
//   CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the inline ReadULONG / ReadSLONG slow path),
//   vector<pair<unsigned long, long> >::resize 0x00A39AE0.
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_GETBYTESIZE
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CArray.h"
#include "engine/CVectorMap.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CMemoryDataInputStream.h"

// Stand-in (no shared header yet): the real class with its Ego_r.pdb members up to the one the row writes (struct_layouts_egor.tsv:
// vtable +0x00, SLONGMap +0x04; NamesUsed / CheckForDuplicateSymbols follow).
class CCRCSymbolMap {
public:
    virtual ~CCRCSymbolMap();
    void LoadBinary(const CWideString& filename);
    CVectorMap<unsigned long, long, CKeyPairCompareLess<unsigned long, long> > SLONGMap;   // +0x04
};

void CCRCSymbolMap::LoadBinary(const CWideString& filename)
{
    CDiskFileWin32 infile(filename, FILE_ACCESS_READ, 2);
    CArray<unsigned char> buffer(infile.GetLength() + 1);
    infile.Read(buffer.GetAsCArray(), infile.GetLength(), 0);
    CMemoryDataInputStream os;
    os.OpenMemorySource(buffer.GetAsCArray(), buffer.GetByteSize());
    long no_entries = os.ReadULONG();
    SLONGMap.resize(no_entries);
    for (long c1 = 0; c1 < no_entries; ++c1) {
        long var1 = os.ReadSLONG();
        long var2 = os.ReadSLONG();
        SLONGMap[c1] = std::pair<long, long>(var1, var2);
    }
}