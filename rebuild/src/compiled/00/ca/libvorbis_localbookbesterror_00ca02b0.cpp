// libvorbis::local_book_besterror(codebook* book, float* a) -- residue encoder helper (res0.c): find the codebook entry nearest to the vector a (threshold search per scalar, brute-force search when that entry has no codeword), subtract the entry's values from a and return the entry index
// Vendor-proof: libvorbis-1.0.1(lib/res0.c) @local_book_besterror@8 -- 879 bytes verified against Fable.exe 0x00CA02B0
// Leaf: _local_book_besterror@8
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA02B0, 879 bytes, `ret 8` (a file static called with two stack arguments and
//   callee-pop: `static __declspec(noinline) int __stdcall` inside extern "C", the chunk 675/676 libvorbis-static recipe).
//   libvorbis lib/res0.c (Xiph.Org libVorbis I 20030909 @ 0x012BF240, the 1.0.1 text: `best=(best*tt->quantvals)+
//   tt->quantmap[i]`, the final subtraction unguarded). Sole caller: _encodepart 0x00CA0620 (landed, `@_encodepart@20`),
//   whose body sits below so VC7.1 emits the static next to its real caller. Catalog label "Audio_ComputeTimeFrequencyMap"
//   is a descriptive stand-in. Callees: none.
// Layout: codebook (codebook.h, 0x2c): dim +0, entries +4, c +0xc, valuelist +0x10; static_codebook: lengthlist +8,
//   thresh_tree +0x28; encode_aux_threshmatch: quantthresh +0, quantmap +4, quantvals +8, threshvals +0xc.
typedef struct oggpack_buffer {
    long           endbyte;
    int            endbit;
    unsigned char* buffer;
    unsigned char* ptr;
    long           storage;
} oggpack_buffer;
typedef unsigned int ogg_uint32_t;
typedef struct encode_aux_threshmatch {
    float* quantthresh;                            // +0x00
    long*  quantmap;                               // +0x04
    int    quantvals;                              // +0x08
    int    threshvals;                             // +0x0c
} encode_aux_threshmatch;
typedef struct static_codebook {
    long   dim;                                    // +0x00
    long   entries;                                // +0x04
    long*  lengthlist;                             // +0x08
    int    maptype;                                // +0x0c
    long   q_min;                                  // +0x10
    long   q_delta;                                // +0x14
    int    q_quant;                                // +0x18
    int    q_sequencep;                            // +0x1c
    long*  quantlist;                              // +0x20
    struct encode_aux_nearestmatch* nearest_tree;  // +0x24
    struct encode_aux_threshmatch*  thresh_tree;   // +0x28
    struct encode_aux_pigeonhole*   pigeon_tree;   // +0x2c
    int    allocedp;                               // +0x30
} static_codebook;
typedef struct codebook {                          // libvorbis codebook.h, 0x2c bytes
    long                          dim;             // +0x00
    long                          entries;         // +0x04
    long                          used_entries;    // +0x08
    const static_codebook*        c;               // +0x0c
    float*                        valuelist;       // +0x10
    ogg_uint32_t*                 codelist;        // +0x14
    int*                          dec_index;       // +0x18
    char*                         dec_codelengths; // +0x1c
    ogg_uint32_t*                 dec_firsttable;  // +0x20
    int                           dec_firsttablen; // +0x24
    int                           dec_maxlength;   // +0x28
} codebook;

extern "C" int __fastcall vorbis_book_encode(codebook* book, int a, oggpack_buffer* b);  // 0x00C96140

extern "C" {
static __declspec(noinline) int __stdcall local_book_besterror(codebook *book,float *a){
  int dim=book->dim,i,k,o;
  int best=0;
  encode_aux_threshmatch *tt=book->c->thresh_tree;

  /* find the quant val of each scalar */
  for(k=0,o=dim;k<dim;++k){
    float val=a[--o];
    i=tt->threshvals>>1;

    if(val<tt->quantthresh[i]){
      if(val<tt->quantthresh[i-1]){
        for(--i;i>0;--i)
          if(val>=tt->quantthresh[i-1])
            break;
      }
    }else{

      for(++i;i<tt->threshvals-1;++i)
        if(val<tt->quantthresh[i])break;

    }

    best=(best*tt->quantvals)+tt->quantmap[i];
  }
  /* regular lattices are easy :-) */

  if(book->c->lengthlist[best]<=0){
    const static_codebook *c=book->c;
    int i,j;
    float bestf=0.f;
    int dim=book->dim;
    float *e=book->valuelist;
    best=-1;
    for(i=0;i<book->entries;i++){
      if(c->lengthlist[i]>0){
        float thisf=0.f;
        for(j=0;j<dim;j++){
          float val=(e[j]-a[j]);
          thisf+=val*val;
        }
        if(best==-1 || thisf<bestf){
          bestf=thisf;
          best=i;
        }
      }
      e+=dim;
    }
  }

  {
    float *ptr=book->valuelist+best*dim;
    for(i=0;i<dim;i++)
      *a++ -= *ptr++;
  }

  return(best);
}
}

extern "C" int __fastcall _encodepart(oggpack_buffer* opb, float* vec, int n, codebook* book, long* acc)   // 0x00CA0620 (landed)
{
    int i, bits = 0;
    int dim = book->dim;
    int step = n / dim;

    for (i = 0; i < step; i++) {
        int entry = local_book_besterror(book, vec + i * dim);
        bits += vorbis_book_encode(book, entry, opb);
    }

    return (bits);
}