// libvorbis::_book_maptype1_quantvals(const static_codebook* b) -- largest integer `vals` with vals^dim <= entries (lattice codebook column size), seeded by floor(pow(entries, 1/dim)) and corrected with integer math
// Leaf: _book_maptype1_quantvals
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C95140, 103 bytes. libvorbis 1.0.1 lib/sharedbook.c, compiled as C with
//   /Gr: `__fastcall`, b in ecx, plain `ret`.
// Callees: the pow intrinsic _CIpow 0x00BFEB40, floor (MSVCR71 import [0x01440100]), __ftol2 0x00BFEA70.
// Data: the float 1.0f @ 0x0122DED8.
// Layout: libvorbis 1.0.1 static_codebook (+0 dim, +4 entries).
// Shape: the shipped source; entries is loaded before `1.f / b->dim` (`fild ; fild ; fdivr`), the (float)
//   cast emits nothing (no /Op), and the endless loop keeps vals in eax with the two products in ecx/edx.
extern "C" double __cdecl pow(double x, double y);
#pragma intrinsic(pow)
extern "C" __declspec(dllimport) double __cdecl floor(double x);   // [0x01440100]

typedef struct static_codebook {
    long  dim;
    long  entries;
    long* lengthlist;
    int   maptype;
    long  q_min;
    long  q_delta;
    int   q_quant;
    int   q_sequencep;
    long* quantlist;
    void* nearest_tree;
    void* thresh_tree;
    void* pigeon_tree;
    int   allocedp;
} static_codebook;

extern "C" long __fastcall _book_maptype1_quantvals(const static_codebook* b)
{
    long vals = floor(pow((float)b->entries, 1.f / b->dim));

    /* verify via integer means that vals really is the greatest value of dim for which
       vals^b->bim <= b->entries; treat the above as an initial guess */
    while (1) {
        long acc = 1;
        long acc1 = 1;
        int i;
        for (i = 0; i < b->dim; i++) {
            acc *= vals;
            acc1 *= vals + 1;
        }
        if (acc <= b->entries && acc1 > b->entries) {
            return vals;
        } else {
            if (acc > b->entries) {
                vals--;
            } else {
                vals++;
            }
        }
    }
}