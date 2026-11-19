// CPackedUIntArray::CalculateBias(const void* src_array, unsigned long src_bits, long src_stride, unsigned long count) const -- the smallest masked value among count source entries ((src_bits + 7) / 8 little-endian bytes each, src_stride bytes apart); 0 when count is 0
// Leaf: ?CalculateBias@CPackedUIntArray@@ABEKPBXKJK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A62AE0, 130 bytes, `ret 10h`, no frame (`this` unused; called by Initialise 0x00A62D80).
// Locals: Ego_d.pdb names (src_ptr, src_bytes, min_value, src_val, input_bit_mask; loop block c1). Their declaration order is the one
//   that homes src_val / c1 / input_bit_mask in the dead src_array / src_bits / count argument slots as retail does (measured over all
//   24 orders after src_ptr). ReadNBytes / BitMasks: Ego_r.pdb private statics (engine/CPackedUIntArray.h opt-in).
#define FABLE_RETAIL_CPACKEDUINTARRAY_CALCULATEBIAS
#include "engine/CPackedUIntArray.h"

unsigned long CPackedUIntArray::CalculateBias(const void* src_array, unsigned long src_bits, long src_stride, unsigned long count) const
{
    const unsigned char* src_ptr = (const unsigned char*)src_array;
    unsigned long src_bytes = (src_bits + 7) >> 3;
    unsigned long min_value = count ? 0xffffffff : 0;
    unsigned long src_val = 0;
    unsigned long input_bit_mask = BitMasks[src_bits];
    for (unsigned long c1 = 0; c1 < count; ++c1) {
        ReadNBytes(src_val, src_ptr, src_bytes);
        src_val &= input_bit_mask;
        if (min_value > src_val)
            min_value = src_val;
        src_ptr += src_stride;
    }
    return min_value;
}