// 009ae390 CDefinitionManager::CompressDefinitionBlock
// Flags:
// Leaf: ?CompressDefinitionBlock@CDefinitionManager@@IAEXAAVCMemoryDataOutputStream@@AAV?$CArray@G@@K@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_CTOR_SETSTRING
#define FABLE_RETAIL_CSYMBOLMAP
#define FABLE_RETAIL_CSYMBOLMAP_ADDSYMBOLDIRTY
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR_ADOPT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATEDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER_SAVEBINARY_PASS23
#define FABLE_RETAIL_CCOMPRESSORZLIB
#define FABLE_RETAIL_CACOMPRESSOR
#define FABLE_RETAIL_CCOMPRESSORZLIB_NATIVE_BUFFER_PASS27
#define FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM_RESERVE
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_GETBYTESIZE
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_STLP_VECTOR_ORIGINAL_RESIZE_INSERT_CHAIN_PASS22
#include <string.h>
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CMemoryDataOutputStream.h"
#include "engine/CCompressorZlib.h"

// Inline-budget: the speed-built enclosing leaf expands both resize overloads.
// The out-of-line copies at 00486189 and 004861DF use size code; the latter has a frame.
#if defined(_MSC_VER)
#pragma optimize("s",on)
#endif
template<> inline void std::vector<unsigned char>::resize(size_type n,const unsigned char& x) { if (n<size()) erase(begin()+n,end()); else insert(end(),n-size(),x); }
#if defined(_MSC_VER)
#pragma optimize("y",off)
#endif
template<> inline void std::vector<unsigned char>::resize(size_type n) { resize(n,value_type()); }
#if defined(_MSC_VER)
#pragma optimize("",on)
#endif
void CDefinitionManager::CompressDefinitionBlock(CMemoryDataOutputStream& uncompressed_buffer,
    CArray<unsigned short>& def_offsets, unsigned long first_compressed_def)
{
    CCompressorZlib compressor;
    CMemoryDataOutputStream buffer_with_offsets;
    unsigned long adjust_offsets = def_offsets.GetByteSize();
    for (unsigned long c_offset = 0; c_offset < def_offsets.size(); ++c_offset)
        def_offsets[c_offset] += adjust_offsets;
    buffer_with_offsets.Write(def_offsets.GetAsCArray(),def_offsets.GetByteSize());
    buffer_with_offsets.Write(uncompressed_buffer.PeekData(),uncompressed_buffer.GetLength());
    CArray<unsigned char> compressed_buffer(buffer_with_offsets.GetLength() + 1024);
    unsigned long compressed_size = compressor.InternalCompress(buffer_with_offsets.PeekData(),
        buffer_with_offsets.GetLength(),compressed_buffer.GetAsCArray(),compressed_buffer.size());
    unsigned long compressed_offset = CompressedDefData.size();
    CompressedDefData.resize(compressed_offset + compressed_size);
    memcpy(&CompressedDefData[compressed_offset],compressed_buffer.GetAsCArray(),compressed_size);
    CompressedOffsets.Insert(first_compressed_def,compressed_offset);
    uncompressed_buffer.Clear();
    def_offsets.clear();
}