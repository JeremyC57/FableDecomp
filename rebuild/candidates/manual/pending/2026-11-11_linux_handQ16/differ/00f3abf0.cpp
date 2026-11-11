// CRangeCompressor::CalcCompressionScript(CArray<CScriptEntry>& script, const unsigned char* data, long count, long stride, long bytes) -- choose the cheapest compression script for `count` values of `bytes` width: wider than 4 bytes in 4-byte columns, 3 as 1+2, otherwise one whole-width block against two half-width scripts
// Grade: DIFFER(605v605) -- one instruction placed early: the split loop's `remaining` init (`mov ebx,edi`) sits before the `offset < bytes` guard, retail after it (0x00F3AC0C). Strength-reduced `bytes - offset` forms place it right but drop the counter (prio 56) under this (58), so this takes ebx; a guarded do-while (`if (bytes > 0) { long offset = 0, remaining = bytes; do ... while (offset < bytes); }`, work/v10/g.cpp) gets every register and the placement right but extends the const-0 esi piece to the values-loop guard (`cmp ebx,esi` v retail `test ebx,ebx`, 0x00F3ACBB).
// Leaf: ?CalcCompressionScript@CRangeCompressor@@AAEJAAV?$CArray@UCScriptEntry@CRangeCompressor@@@@PBEJJJ@Z
// Retail: Fable.exe 0x00F3ABF0, 605 bytes, thiscall `ret 14h` (Ego_r.pdb / FableWin.pdb name above).
// Callees: itself (recursive), CArray<unsigned long>(unsigned int) 0x00A96100 (out of line), CalcBestCompressionForBlock 0x00F3A890,
//   CArray<CScriptEntry>::push_back 0x00F3B420 (out of line), free 0x00BFEA14 (the inline array destructors).
#define FABLE_RETAIL_CARRAY_SIZE_CTOR_OUTOFLINE
#define FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE
#include "engine/CArray.h"
#include "engine/CRangeCompressor.h"

long CRangeCompressor::CalcCompressionScript(CArray<CScriptEntry>& script, const unsigned char* data, long count, long stride, long bytes)
{
    long total = 0;
    if (bytes > 4) {
        for (long offset = 0, remaining = bytes; offset < bytes; offset += 4, remaining -= 4)
            total += CalcCompressionScript(script, data + offset, count, stride, remaining > 4 ? 4 : remaining);
        return total;
    }
    if (bytes == 3)
        return CalcCompressionScript(script, data + 2, count, stride, 1)
             + CalcCompressionScript(script, data, count, stride, 2);
    CArray<unsigned long> values(count);
    for (long i = 0; i < count; ++i) {
        const unsigned char* source = data + i * stride;
        unsigned long value;
        if (bytes == 4)
            value = *(const unsigned long*)source;
        else if (bytes == 2)
            value = *(const unsigned short*)source;
        else
            value = *source;
        values[i] = value;
    }
    CScriptEntry entry;
    long whole = CalcBestCompressionForBlock(values, bytes, entry);
    if (bytes == 1) {
        script.push_back(entry);
        return whole;
    }
    CArray<CScriptEntry> split;
    long half = bytes / 2;
    long split_cost = CalcCompressionScript(split, data, count, stride, half);
    split_cost += CalcCompressionScript(split, data + half, count, stride, half);
    if (whole <= split_cost) {
        script.push_back(entry);
        return whole;
    }
    for (unsigned long j = 0; j < split.size(); ++j)
        script.push_back(split[j]);
    return split_cost;
}
