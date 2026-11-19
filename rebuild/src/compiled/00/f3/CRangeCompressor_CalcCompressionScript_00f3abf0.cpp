// CRangeCompressor::CalcCompressionScript(CArray<CScriptEntry>& script, const unsigned char* block_ptr, long element_count, long element_size, long block_size) -- choose and emit the cheapest compression script for a block: split blocks wider than 4 bytes into 4-byte columns, split a 3-byte block into 2 + 1, else read the block's values, cost one entry for the whole block against the two half-width sub-scripts, and append the cheaper
// Leaf: ?CalcCompressionScript@CRangeCompressor@@AAEJAAV?$CArray@UCScriptEntry@CRangeCompressor@@@@PBEJJJ@Z
// Flags: none
// Retail: 0x00F3ABF0, 605 bytes, thiscall `ret 14h`.
// Shape: FableWin.pdb / Ego_d.pdb line tables (lines 727..777): locals count, count2, block_buffer, count1, sub_script, script_entry
//   and three n; the 3-byte split sums the 2-byte half then the 1-byte half (line 742). ReadVarSizedUInt is the same-TU private member
//   (FableWin lines 322..336: if 4 / else if 2 / else { ASSERT(byte_count==1); return }), inline in retail at 0x00F3ACC3..0x00F3ACD6.
// Inferred: the fill loop's named reference `unsigned long& value = block_buffer[n];` is unsourced -- Ego_d.pdb and FableWin.pdb
//   record no such local, and their line tables give line 749 as the single 39-byte statement
//   `block_buffer[n] = ReadVarSizedUInt(...)`. Mechanism: the named reference keeps the element address a c2 candidate referenced in
//   the fill-loop head and join blocks, so `this` drops from register priority 58 to 54 and loses to the strength-reduced
//   `block_size - n` temp (56): retail this = ebp, temp = ebx (0x00F3AC00 / 0x00F3AC0C). With the donor's statement every
//   instruction matches except that ebx/ebp swap. Ruled out (74 grades): the donor read line by line (FableWin 0x03316D80 listing),
//   callee signatures (push_back / vector(n) / CalcBestCompressionForBlock by-reference parameters identical to the donor's;
//   out-of-line and visible-ctor variants), include order, a `long left` counter (fixes registers, moves a copy before the loop-entry
//   jle), the CArray indexer models (INDEX_ASSERT, STLP INDEX_START / ORIGINAL_FRONT), the VC6 `if (0) {} else for` loops, assert
//   placement, sizeof constants, the QFE-4035 compiler and TU flags. Owner review: advisor decision 2026-11-19.
// Inferred: one FABLE_RETAIL_ASSERT dead arm (ReadVarSizedUInt `byte_count==1`, FableWin line 333 ASSERT, string "byte_count==1"
//   line 0x14d); emits no bytes, no measured effect.
// Callees: std::vector<unsigned long>::vector(unsigned int) 0x00A96100 (CArray<unsigned long>(n) forwards inline),
//   CalcBestCompressionForBlock 0x00F3A890, std::vector<CScriptEntry>::push_back 0x00F3B420 (catalog name; NAME_FIXES proposes
//   CArray<CScriptEntry>::push_back), free 0x00BFEA14.
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
    else if (byte_count == 2)
        return *(const unsigned short*)buffer;
    else
    {
        FABLE_RETAIL_ASSERT(byte_count == 1);
        return *buffer;
    }
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
        return CalcCompressionScript(script, block_ptr, element_count, element_size, 2)
             + CalcCompressionScript(script, block_ptr + 2, element_count, element_size, 1);
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