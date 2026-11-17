// CRangeCompressor::CalcCompressionScript(CArray<CScriptEntry>& script, const unsigned char* block_ptr, long element_count, long element_size, long block_size) -- choose the cheapest compression script for element_count values of block_size bytes: wider than 4 bytes in 4-byte columns, 3 as 1 + 2, otherwise one whole-width block against two half-width scripts
// Grade: DIFFER(605v605) -- 1 placement hunk: the column loop counter `left` (NOT in Ego_d; the only non-Ego_d local) is initialised (`mov ebx,edi`) before the `n < block_size` guard, retail after it (0x00F3AC0C). Without `left` (Ego_d: only n) the placement is right but `this` takes ebx (retail ebp): 19 hunks.
// Leaf: ?CalcCompressionScript@CRangeCompressor@@AAEJAAV?$CArray@UCScriptEntry@CRangeCompressor@@@@PBEJJJ@Z
// Retail: Fable.exe 0x00F3ABF0, 605 bytes, thiscall `ret 14h`.
#define FABLE_RETAIL_CARRAY_SIZE_CTOR_OUTOFLINE
#define FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE
#include "engine/CArray.h"
#include "engine/CRangeCompressor.h"

long CRangeCompressor::CalcCompressionScript(CArray<CScriptEntry>& script, const unsigned char* block_ptr, long element_count, long element_size, long block_size)
{
    long count = 0;
    if (block_size > 4) {
        long left = block_size;
        for (long n = 0; n < block_size; n += 4) {
            count += CalcCompressionScript(script, block_ptr + n, element_count, element_size, left > 4 ? 4 : left);
            left -= 4;
        }
        return count;
    }
    if (block_size == 3)
        return CalcCompressionScript(script, block_ptr + 2, element_count, element_size, 1)
             + CalcCompressionScript(script, block_ptr, element_count, element_size, 2);
    CArray<unsigned long> block_buffer(element_count);
    for (long n = 0; n < element_count; n++) {
        block_buffer[n] = block_size == 4 ? *(const unsigned long*)(block_ptr + n * element_size)
                        : block_size == 2 ? *(const unsigned short*)(block_ptr + n * element_size)
                        : *(block_ptr + n * element_size);
    }
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
