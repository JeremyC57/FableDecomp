// Behaviour test for 0x004861DF std::vector<unsigned char>::resize(size_type) (NF104 q4767): links the row's object. The one-argument
// resize is declared only here (FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL), so the call binds the row's explicit instantiation; the row
// also emits the out-of-line resize(size_type, const unsigned char&) it calls (0x00486189 in retail). Grows with zero bytes, keeps the
// existing bytes, then shrinks.
// The row keeps _M_fill_insert out of line (retail's own COMDAT, not part of this row); the harness supplies STLport's body
// (FABLE_RETAIL_STLP_VECTOR, stl/_vector.c) by explicit instantiation so the link resolves.
#include <cstdio>
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#include "stlport/fable_containers.h"
template void std::vector<unsigned char>::_M_fill_insert(unsigned char*, std::vector<unsigned char>::size_type, const unsigned char&);

int main()
{
    std::vector<unsigned char> v;
    v.resize(5);
    if (v.size() != 5) { std::printf("004861df_TEST FAIL grow %u\n", (unsigned)v.size()); return 1; }
    for (unsigned i = 0; i < 5; ++i) if (v[i] != 0) { std::printf("004861df_TEST FAIL zero\n"); return 1; }
    v[0] = 7; v[4] = 9;
    v.resize(8);
    if (v.size() != 8 || v[0] != 7 || v[4] != 9 || v[7] != 0) { std::printf("004861df_TEST FAIL keep\n"); return 1; }
    v.resize(2);
    if (v.size() != 2 || v[0] != 7) { std::printf("004861df_TEST FAIL shrink\n"); return 1; }
    std::printf("004861df_TEST PASS\n");
    return 0;
}
