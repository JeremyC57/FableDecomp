#ifndef FABLE_PASS22_RVBVECTOR_H
#define FABLE_PASS22_RVBVECTOR_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
struct _RVB_VECTOR {
public:
 float x; // +0x0
 float y; // +0x4
 float z; // +0x8
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char RVB_VECTORSizePass22[(sizeof(_RVB_VECTOR)==12)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
