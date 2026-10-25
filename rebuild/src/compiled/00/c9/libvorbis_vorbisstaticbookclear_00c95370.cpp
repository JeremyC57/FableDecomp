// libvorbis::vorbis_staticbook_clear(static_codebook* b) -- free an allocated static codebook's quant/length lists and its encoder nearest-match / threshold trees, then wipe it
// Leaf: vorbis_staticbook_clear
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C95370, 175 bytes. libvorbis 1.0.1 lib/sharedbook.c, compiled as C with
//   /Gr: `__fastcall`, b in ecx, plain `ret`.
// Callees: free (MSVCR71 import [0x01440154], hoisted into esi) through _ogg_free.
// Layout: libvorbis 1.0.1 static_codebook (0x34 bytes; +0x08 lengthlist, +0x20 quantlist, +0x24 nearest_tree,
//   +0x28 thresh_tree, +0x30 allocedp), encode_aux_nearestmatch (24 bytes: ptr0, ptr1, p, q, aux, alloc) and
//   encode_aux_threshmatch (16 bytes: quantthresh, quantmap, quantvals, threshvals).
// Shape: the shipped source; the unconditional frees inside each tree share one deferred `add esp` with the
//   tree's own free; the small memsets expand to a zero register and 6 / 4 stores, the 0x34-byte one to
//   `rep stosd` x13.
extern "C" __declspec(dllimport) void __cdecl free(void* block);   // [0x01440154]
extern "C" void* __cdecl memset(void* dst, int c, unsigned int n);
#pragma intrinsic(memset)
#define _ogg_free free

typedef struct encode_aux_nearestmatch {
    long* ptr0;
    long* ptr1;
    long* p;
    long* q;
    long  aux;
    long  alloc;
} encode_aux_nearestmatch;

typedef struct encode_aux_threshmatch {
    float* quantthresh;
    long*  quantmap;
    int    quantvals;
    int    threshvals;
} encode_aux_threshmatch;

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
    encode_aux_nearestmatch* nearest_tree;
    encode_aux_threshmatch*  thresh_tree;
    void* pigeon_tree;
    int   allocedp;
} static_codebook;

extern "C" void __fastcall vorbis_staticbook_clear(static_codebook* b)
{
    if (b->allocedp) {
        if (b->quantlist) _ogg_free(b->quantlist);
        if (b->lengthlist) _ogg_free(b->lengthlist);
        if (b->nearest_tree) {
            _ogg_free(b->nearest_tree->ptr0);
            _ogg_free(b->nearest_tree->ptr1);
            _ogg_free(b->nearest_tree->p);
            _ogg_free(b->nearest_tree->q);
            memset(b->nearest_tree, 0, sizeof(*b->nearest_tree));
            _ogg_free(b->nearest_tree);
        }
        if (b->thresh_tree) {
            _ogg_free(b->thresh_tree->quantthresh);
            _ogg_free(b->thresh_tree->quantmap);
            memset(b->thresh_tree, 0, sizeof(*b->thresh_tree));
            _ogg_free(b->thresh_tree);
        }

        memset(b, 0, sizeof(*b));
    }
}