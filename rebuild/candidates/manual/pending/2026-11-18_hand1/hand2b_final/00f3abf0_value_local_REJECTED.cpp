// CRangeCompressor::CalcCompressionScript(CArray<CScriptEntry>& script, const unsigned char* block_ptr, long element_count, long element_size, long block_size) -- choose and emit the cheapest compression script for a block: split blocks wider than 4 bytes into 4-byte columns, split a 3-byte block into 2 + 1, else read the block's values, cost one entry for the whole block against the two half-width sub-scripts, and append the cheaper
// OWNER DECISION: the fill loop's `unsigned long& value = block_buffer[n];` reference local is NOT in the original source (Ego_d.pdb and
//   FableWin.pdb both record the row's locals as count, count2, block_buffer, count1, sub_script, script_entry and three n; their line
//   tables give line 749 as the single 39-byte statement `block_buffer[n] = ReadVarSizedUInt(block_ptr + n * element_size, block_size);`).
//   It is the only lever found: the named reference keeps the element address as a c2 candidate referenced in the fill-loop head and
//   join blocks, `this` drops from priority 58 to 54 and loses to the strength-reduced `block_size - n` temp (retail: this = ebp, temp =
//   ebx, 0x00F3AC00 / 0x00F3AC0C). With the original statement every instruction matches except that ebx/ebp swap (differ/00f3abf0.cpp).
// Shape: FableWin/Ego_d line tables (lines 727..777); ReadVarSizedUInt is the same-TU private member (lines 322..336, inline in retail:
//   0x00F3ACC3..0x00F3ACD6, the same cmp 4 / cmp 2 chain as its Decompress expansions 0x00F39FD4 / 0x00F3A006 / 0x00F3A030).
// Inferred: one FABLE_RETAIL_ASSERT dead arm (ReadVarSizedUInt `byte_count==1`, FableWin 0x03316D80 line 333 ASSERT, string
//   "byte_count==1" line 0x14d); emits no bytes, no measured effect.
// Callees: std::vector<unsigned long>::vector(unsigned int) 0x00A96100 (CArray<unsigned long>(n) forwards inline),
//   CalcBestCompressionForBlock 0x00F3A890, std::vector<CScriptEntry>::push_back 0x00F3B420 (catalog name; see NOTES: Ego_r keeps an
//   out-of-line CArray<CScriptEntry>::push_back in this object), free 0x00BFEA14.
// Leaf: ?CalcCompressionScript@CRangeCompressor@@AAEJAAV?$CArray@UCScriptEntry@CRangeCompressor@@@@PBEJJJ@Z
// Retail: 0x00F3ABF0, 605 bytes, thiscall `ret 14h`.
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
    FABLE_RETAIL_ASSERT(byte_count==1);
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
    for (long n = 0; n < element_count; n++) {
        unsigned long& value = block_buffer[n];
        value = ReadVarSizedUInt(block_ptr + n * element_size, block_size);
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
