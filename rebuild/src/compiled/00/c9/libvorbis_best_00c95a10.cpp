// libvorbis::_best(codebook* book, float* a, int step) -- sharedbook.c: find the codebook entry nearest the strided vector a: with a threshold-match hint (thresh_tree) quantise each scalar through quantthresh / quantmap into a lattice index and return it when that entry is coded; otherwise brute force the squared distance (_dist) over every coded entry
// Leaf: @_best@12
// Flags: /TC /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C95A10, 588 bytes (`ret 4` at 0x00C95C59), LTCG `extern "C" __fastcall` (book in ecx, a in edx,
//   step on the stack). Callee: _dist 0x00C95910 (sharedbook.c static, landed libvorbis_dist_00c95910.cpp: el / ref / b on the stack,
//   step in esi, `ret 0Ch`, float in st(0)). Caller: vorbis_book_errorv 0x00C96170 (landed, binds `@_best@12`).
// Extent note: the catalogued extent 0x00C95A10 + 642 holds THREE functions: _best (588 bytes, to 0x00C95C5C), then sharedbook.c's
//   vorbis_book_codeword 0x00C95C60 (18 bytes: `book->c ? book->codelist[entry] : -1`) and vorbis_book_codelen 0x00C95C80 (18 bytes:
//   `book->c ? book->c->lengthlist[entry] : -1`), each `@name@8` __fastcall. All three are in this TU (sharedbook.c order) and graded
//   against the split extents (trim_oracle.tsv); the chunk oracle's 642-byte row needs splitting at landing.
// Identity (two sources): (1) the body is Xiph libvorbis 1.0.1 lib/sharedbook.c _best verbatim with its `#if 0` blocks (the
//   pigeonhole and nearest-tree searches) removed by the preprocessor: the threshvals>>1 bisection start, the two scans of
//   quantthresh, `index = index*quantvals + quantmap[i]`, the `lengthlist[index] > 0` check, then the brute-force loop keeping
//   `besti == -1 || this < best`; (2) its callee is the landed _dist and its caller the landed vorbis_book_errorv, both sharedbook.c /
//   codebook.c neighbours ("Xiph.Org libVorbis I 20030909" at 0x012BF240). The catalog name is the Xiph one already.
// Shape: a C TU (/TC: sharedbook.c names a local `this`), the landed _dist TU convention (`static __declspec(noinline) __stdcall` static
//   beside its real caller, so VC7.1 /O2 picks step in esi itself); the public functions are __fastcall (C linkage under /TC).
// Original libvorbis sharedbook.c (libvorbis 1.0.1, unmodified function bodies); BSD-style terms in reference COPYING.
typedef unsigned int ogg_uint32_t;
typedef struct encode_aux_threshmatch {            // libvorbis 1.0.1 codebook.h
    float* quantthresh;                            // +0x00
    long*  quantmap;                               // +0x04
    int    quantvals;                              // +0x08
    int    threshvals;                             // +0x0c
} encode_aux_threshmatch;
typedef struct static_codebook {                   // libvorbis 1.0.1 codebook.h, 0x34 bytes
    long  dim;                                     // +0x00
    long  entries;                                 // +0x04
    long* lengthlist;                              // +0x08
    int   maptype;                                 // +0x0c
    long  q_min;                                   // +0x10
    long  q_delta;                                 // +0x14
    int   q_quant;                                 // +0x18
    int   q_sequencep;                             // +0x1c
    long* quantlist;                               // +0x20
    struct encode_aux_nearestmatch* nearest_tree;  // +0x24
    encode_aux_threshmatch*  thresh_tree;          // +0x28
    struct encode_aux_pigeonhole*   pigeon_tree;   // +0x2c
    int   allocedp;                                // +0x30
} static_codebook;
typedef struct codebook {                          // libvorbis 1.0.1 codebook.h, 0x2c bytes
    long                   dim;                    // +0x00
    long                   entries;                // +0x04
    long                   used_entries;           // +0x08
    const static_codebook* c;                      // +0x0c
    float*                 valuelist;              // +0x10
    ogg_uint32_t*          codelist;               // +0x14
    int*                   dec_index;              // +0x18
    char*                  dec_codelengths;        // +0x1c
    ogg_uint32_t*          dec_firsttable;         // +0x20
    int                    dec_firsttablen;        // +0x24
    int                    dec_maxlength;          // +0x28
} codebook;

static __declspec(noinline) float __stdcall _dist(int el,float *ref, float *b,int step){
  int i;
  float acc=0.f;
  for(i=0;i<el;i++){
    float val=(ref[i]-b[i*step]);
    acc+=val*val;
  }
  return(acc);
}

int __fastcall _best(codebook *book, float *a, int step){
  encode_aux_threshmatch *tt=book->c->thresh_tree;

  int dim=book->dim;
  int k,o;
  /*int savebest=-1;
    float saverr;*/

  /* do we have a threshhold encode hint? */
  if(tt){
    int index=0,i;
    /* find the quant val of each scalar */
    for(k=0,o=step*(dim-1);k<dim;k++,o-=step){

      i=tt->threshvals>>1;
      if(a[o]<tt->quantthresh[i]){

	for(;i>0;i--)
	  if(a[o]>=tt->quantthresh[i-1])
	    break;
	
      }else{

	for(i++;i<tt->threshvals-1;i++)
	  if(a[o]<tt->quantthresh[i])break;

      }

      index=(index*tt->quantvals)+tt->quantmap[i];
    }
    /* regular lattices are easy :-) */
    if(book->c->lengthlist[index]>0) /* is this unused?  If so, we'll
					use a decision tree after all
					and fall through*/
      return(index);
  }

  /* brute force it! */
  {
    const static_codebook *c=book->c;
    int i,besti=-1;
    float best=0.f;
    float *e=book->valuelist;
    for(i=0;i<book->entries;i++){
      if(c->lengthlist[i]>0){
	float this=_dist(dim,e,a,step);
	if(besti==-1 || this<best){
	  best=this;
	  besti=i;
	}
      }
      e+=dim;
    }

    return(besti);
  }
}

long __fastcall vorbis_book_codeword(codebook *book,int entry){
  if(book->c) /* only use with encode; decode optimizations are
                 allowed to break this */
    return book->codelist[entry];
  return -1;
}

long __fastcall vorbis_book_codelen(codebook *book,int entry){
  if(book->c) /* only use with encode; decode optimizations are
                 allowed to break this */
    return book->c->lengthlist[entry];
  return -1;
}