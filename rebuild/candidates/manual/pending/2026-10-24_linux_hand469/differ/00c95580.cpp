// vorbis_book_init_decode(codebook* c, const static_codebook* s) -- libvorbis sharedbook: build a decode codebook from a static one: collapse to the used entries, sort the bit-reversed codewords (treeless decode), unquantize the values, fill the sparse index / code-length lists and the first-level lookup table with direct hits and hi/lo search hints; 0, or -1 (book cleared) when the codewords cannot be built
// Grade: DIFFER(923v910) -- lane gap: LTCG custom convention. Retail passes bitreverse's argument in eax (`mov eax,[ecx] ; call 0x00C954F0`, result in eax, three call sites); ours must pick a VC7.1 convention (__fastcall: `mov ecx,[ecx]`), which re-colours the code around every call (the firsttable fill loop keeps the code length in ebx and drops one reload in retail; ours re-reads dec_codelengths) and shifts the frame words (s at [ebp-14h] vs ours [ebp-18h], the codep/sortindex alloca pointers one word over). The prologue, the used-entry count, the memset, the _make_words / qsort / _book_unquantize / _ilog / calloc calls and the hint encoding (`or eax,0FFFF0000h ; shl eax,0Fh`) already agree. 1 grade.
// Leaf: vorbis_book_init_decode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C95580, 910 bytes (912 with int3 padding), plain `ret` (callee-clean __fastcall,
//   both arguments in registers).
// Library: libpk's statically linked Xiph libVorbis I 20030909 (sharedbook.c). The catalog's "CTCWound::CWound::Initialize" is a
//   propagated label. Neighbours: _ilog 0x00C94EE0, _make_words 0x00C94FD0, _book_unquantize 0x00C951B0, vorbis_book_clear
//   0x00C95440, vorbis_book_init_encode 0x00C954A0, bitreverse 0x00C954F0, sort32a 0x00C95560.
// Convention: the LTCG link made the C API __fastcall (c in ecx, s in edx; _make_words / _book_unquantize take two register
//   arguments and one stack argument, callee-clean), but the private static bitreverse takes its argument in eax
//   (`mov eax,[ecx] ; call 0x00C954F0`) -- an LTCG custom convention VC7.1 without /GL cannot emit (lane gap). It is
//   declared __fastcall here so the call stays out of line. CRT calls through the MSVCR71 imports: qsort [0x01440060],
//   malloc [0x01440158], free [0x01440154], calloc [0x01440058]; alloca is the _chkstk probe 0x00BFEA30.
// Shape: the sharedbook.c body verbatim (alloca'd codep / sortindex, EBP frame).
extern "C" {
typedef unsigned int ogg_uint32_t;

struct encode_aux_nearestmatch;
struct encode_aux_threshmatch;
struct encode_aux_pigeonhole;
typedef struct static_codebook {
    long  dim;                          // +0x00
    long  entries;                      // +0x04
    long* lengthlist;                   // +0x08
    int   maptype;                      // +0x0c
    long  q_min;
    long  q_delta;
    int   q_quant;
    int   q_sequencep;
    long* quantlist;
    struct encode_aux_nearestmatch* nearest_tree;
    struct encode_aux_threshmatch*  thresh_tree;
    struct encode_aux_pigeonhole*   pigeon_tree;
    int   allocedp;
} static_codebook;

typedef struct codebook {               // 0x2c bytes
    long dim;                           // +0x00
    long entries;                       // +0x04
    long used_entries;                  // +0x08
    const static_codebook* c;           // +0x0c
    float*        valuelist;            // +0x10
    ogg_uint32_t* codelist;             // +0x14
    int*          dec_index;            // +0x18
    char*         dec_codelengths;      // +0x1c
    ogg_uint32_t* dec_firsttable;       // +0x20
    int           dec_firsttablen;      // +0x24
    int           dec_maxlength;        // +0x28
} codebook;

__declspec(dllimport) void* __cdecl malloc(unsigned int size);
__declspec(dllimport) void* __cdecl calloc(unsigned int count, unsigned int size);
__declspec(dllimport) void __cdecl free(void* block);
__declspec(dllimport) void __cdecl qsort(void* base, unsigned int count, unsigned int width,
                                         int (__cdecl* compare)(const void*, const void*));
void* __cdecl memset(void* dest, int value, unsigned int count);
#pragma intrinsic(memset)
void* __cdecl _alloca(unsigned int size);
#pragma intrinsic(_alloca)

int __fastcall _ilog(unsigned int v);                                               // 0x00C94EE0
ogg_uint32_t* __fastcall _make_words(long* l, long n, long sparsecount);            // 0x00C94FD0
float* __fastcall _book_unquantize(const static_codebook* b, int n, int* sparsemap);   // 0x00C951B0
void __fastcall vorbis_book_clear(codebook* b);                                     // 0x00C95440
ogg_uint32_t __fastcall bitreverse(ogg_uint32_t x);                                 // 0x00C954F0 (retail: x in eax)
int __cdecl sort32a(const void* a, const void* b);                                  // 0x00C95560

#define _ogg_malloc malloc
#define _ogg_calloc calloc
#define _ogg_free   free
#define alloca      _alloca

int __fastcall vorbis_book_init_decode(codebook* c, const static_codebook* s)
{
    int i, j, n = 0, tabn;
    int* sortindex;
    memset(c, 0, sizeof(*c));

    /* count actually used entries */
    for (i = 0; i < s->entries; i++)
        if (s->lengthlist[i] > 0)
            n++;

    c->entries = s->entries;
    c->used_entries = n;
    c->dim = s->dim;

    /* two different remappings go on here: collapse to the used entries, then reorder by sorted bit-reversed codeword */
    {
        /* perform sort */
        ogg_uint32_t* codes = _make_words(s->lengthlist, s->entries, c->used_entries);
        ogg_uint32_t** codep = (ogg_uint32_t**)alloca(sizeof(*codep) * n);

        if (codes == 0) goto err_out;

        for (i = 0; i < n; i++) {
            codes[i] = bitreverse(codes[i]);
            codep[i] = codes + i;
        }

        qsort(codep, n, sizeof(*codep), sort32a);

        sortindex = (int*)alloca(n * sizeof(*sortindex));
        c->codelist = (ogg_uint32_t*)_ogg_malloc(n * sizeof(*c->codelist));
        /* the index is a reverse index */
        for (i = 0; i < n; i++) {
            int position = codep[i] - codes;
            sortindex[position] = i;
        }

        for (i = 0; i < n; i++)
            c->codelist[sortindex[i]] = codes[i];
        _ogg_free(codes);
    }

    c->valuelist = _book_unquantize(s, n, sortindex);
    c->dec_index = (int*)_ogg_malloc(n * sizeof(*c->dec_index));

    for (n = 0, i = 0; i < s->entries; i++)
        if (s->lengthlist[i] > 0)
            c->dec_index[sortindex[n++]] = i;

    c->dec_codelengths = (char*)_ogg_malloc(n * sizeof(*c->dec_codelengths));
    for (n = 0, i = 0; i < s->entries; i++)
        if (s->lengthlist[i] > 0)
            c->dec_codelengths[sortindex[n++]] = (char)s->lengthlist[i];

    c->dec_firsttablen = _ilog(c->used_entries) - 4; /* this is magic */
    if (c->dec_firsttablen < 5) c->dec_firsttablen = 5;
    if (c->dec_firsttablen > 8) c->dec_firsttablen = 8;

    tabn = 1 << c->dec_firsttablen;
    c->dec_firsttable = (ogg_uint32_t*)_ogg_calloc(tabn, sizeof(*c->dec_firsttable));
    c->dec_maxlength = 0;

    for (i = 0; i < n; i++) {
        if (c->dec_maxlength < c->dec_codelengths[i])
            c->dec_maxlength = c->dec_codelengths[i];
        if (c->dec_codelengths[i] <= c->dec_firsttablen) {
            ogg_uint32_t orig = bitreverse(c->codelist[i]);
            for (j = 0; j < (1 << (c->dec_firsttablen - c->dec_codelengths[i])); j++)
                c->dec_firsttable[orig | (j << c->dec_codelengths[i])] = i + 1;
        }
    }

    /* now fill in 'unused' entries in the firsttable with hi/lo search hints for the non-direct-hits */
    {
        ogg_uint32_t mask = 0xfffffffeUL << (31 - c->dec_firsttablen);
        long lo = 0, hi = 0;

        for (i = 0; i < tabn; i++) {
            ogg_uint32_t word = i << (32 - c->dec_firsttablen);
            if (c->dec_firsttable[bitreverse(word)] == 0) {
                while ((lo + 1) < n && c->codelist[lo + 1] <= word) lo++;
                while (hi < n && word >= (c->codelist[hi] & mask)) hi++;

                /* only 15 bits per hint: encode as the difference from the extremes */
                {
                    unsigned long loval = lo;
                    unsigned long hival = n - hi;

                    if (loval > 0x7fff) loval = 0x7fff;
                    if (hival > 0x7fff) hival = 0x7fff;
                    c->dec_firsttable[bitreverse(word)] = 0x80000000UL | (loval << 15) | hival;
                }
            }
        }
    }

    return (0);
err_out:
    vorbis_book_clear(c);
    return (-1);
}
}
