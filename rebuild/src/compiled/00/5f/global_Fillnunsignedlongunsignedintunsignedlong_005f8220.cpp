#pragma optimize("s",on)
// Leaf: _Fill_n<unsigned_long*,unsigned_int,unsigned_long>
// std::_Fill_n<unsigned long*, unsigned int, unsigned long>
unsigned long * __fastcall Fill_n_impl(unsigned long *dest, unsigned int count, unsigned long *val)
{
    for (; 0 < count; --count, ++dest)
    {
        if (dest)
            *dest = *val;
    }
    return dest;
}