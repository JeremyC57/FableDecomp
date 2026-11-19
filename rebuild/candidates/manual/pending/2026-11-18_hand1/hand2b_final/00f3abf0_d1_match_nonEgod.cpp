// CRangeCompressor::CalcCompressionScript -- Choose and emit the least expensive compression script for a block.
// Leaf: ?CalcCompressionScript@CRangeCompressor@@AAEJAAV?$CArray@UCScriptEntry@CRangeCompressor@@@@PBEJJJ@Z
// Retail: 0x00F3ABF0, 605 bytes, thiscall.
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
#include "engine/CArray.h"
#include "engine/CRangeCompressor.h"

unsigned long CRangeCompressor::ReadVarSizedUInt(const unsigned char* buffer, long byte_count)
{
    if (byte_count == 4)
        return *(const unsigned long*)buffer;
    if (byte_count == 2)
        return *(const unsigned short*)buffer;
    FABLE_RETAIL_ASSERT(byte_count == 1);
    return *buffer;
}

long CRangeCompressor::CalcCompressionScript(CArray<CScriptEntry>& script, const unsigned char* block_ptr, long element_count, long element_size, long block_size)
{
    long count = 0;
    if (block_size > 4) {
        for (long n = 0; n < block_size; n += 4)
            count += CalcCompressionScript(script, block_ptr + n, element_count, element_size, block_size - n > 4 ? 4 : block_size - n);
        return count;
    }
    if (block_size == 3)
        return CalcCompressionScript(script, block_ptr + 2, element_count, element_size, 1)
             + CalcCompressionScript(script, block_ptr, element_count, element_size, 2);
    CArray<unsigned long> block_buffer(element_count);
    for (long n = 0; n < element_count; n++)
    {   unsigned long& slot = block_buffer[n];
        slot = ReadVarSizedUInt(block_ptr + n * element_size, block_size); }
    CScriptEntry script_entry;
    long count1 = CalcBestCompressionForBlock(block_buffer, block_size, script_entry);
    if (block_size == 1) {
        script.push_back(script_entry);
        return count1;
    }
    CArray<CScriptEntry> sub_script;
    long count2 = CalcCompressionScript(sub_script, block_ptr, element_count, element_size, block_size / 2);
    count2 += CalcCompressionScript(sub_script, block_ptr + block_size / 2, element_count, element_size, block_size / 2);
    if (count1 <= count2) {
        script.push_back(script_entry);
        return count1;
    }
    for (long n = 0; n < sub_script.size(); n++)
        script.push_back(sub_script[n]);
    return count2;
}
