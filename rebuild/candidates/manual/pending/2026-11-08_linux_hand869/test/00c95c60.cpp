// Behaviour test for libvorbis codebook.c vorbis_book_codeword (retail 0x00C95C60): a copy of the encode-side accessor -- the
// codeword of an entry is book->codelist[entry] while the book still carries its static codebook, and -1 on a decode-only book.
#include <cstdio>
#include <cstring>
typedef unsigned int ogg_uint32_t;
typedef struct encode_aux_threshmatch { float* quantthresh; long* quantmap; int quantvals; int threshvals; } encode_aux_threshmatch;
typedef struct static_codebook {
    long dim; long entries; long* lengthlist; int maptype; long q_min; long q_delta; int q_quant; int q_sequencep; long* quantlist;
    struct encode_aux_nearestmatch* nearest_tree; encode_aux_threshmatch* thresh_tree; struct encode_aux_pigeonhole* pigeon_tree; int allocedp;
} static_codebook;
typedef struct codebook {
    long dim; long entries; long used_entries; const static_codebook* c; float* valuelist; ogg_uint32_t* codelist; int* dec_index;
    char* dec_codelengths; ogg_uint32_t* dec_firsttable; int dec_firsttablen; int dec_maxlength;
} codebook;

static float _dist(int el,float *ref, float *b,int step){
  int i;
  float acc=0.f;
  for(i=0;i<el;i++){
    float val=(ref[i]-b[i*step]);
    acc+=val*val;
  }
  return(acc);
}

static int _best(codebook *book, float *a, int step){
  encode_aux_threshmatch *tt=book->c->thresh_tree;

  int dim=book->dim;
  int k,o;

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
	float dist=_dist(dim,e,a,step);
	if(besti==-1 || dist<best){
	  best=dist;
	  besti=i;
	}
      }
      e+=dim;
    }

    return(besti);
  }
}

static long vorbis_book_codeword(codebook *book,int entry){
  if(book->c) /* only use with encode; decode optimizations are
                 allowed to break this */
    return book->codelist[entry];
  return -1;
}

static long vorbis_book_codelen(codebook *book,int entry){
  if(book->c) /* only use with encode; decode optimizations are
                 allowed to break this */
    return book->c->lengthlist[entry];
  return -1;
}

/* a 2-dimensional book over the values {-1, 0, 1}: 9 entries, entry = v0 + 3*v1 with v in {0,1,2} for {-1,0,1}
   (the first dimension is the least significant lattice digit, as _book_unquantize's maptype 1 and _best's hint agree) */
static long lengths[9] = { 3, 3, 3, 3, 1, 3, 3, 3, 3 };
static float values[18] = { -1,-1, 0,-1, 1,-1, -1,0, 0,0, 1,0, -1,1, 0,1, 1,1 };
static ogg_uint32_t codes[9] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8 };
static float thresh[2] = { -0.5f, 0.5f };
static long quantmap[3] = { 0, 1, 2 };
static encode_aux_threshmatch tm = { thresh, quantmap, 3, 3 };
static static_codebook sc = { 2, 9, lengths, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static codebook book = { 2, 9, 9, &sc, values, codes, 0, 0, 0, 0, 0 };

static int fail(const char* what) { std::printf("%s\n00c95c60_TEST FAIL\n", what); return 1; }

int main()
{
    int i;
    for (i = 0; i < 9; i++) if (vorbis_book_codeword(&book, i) != (long)codes[i]) return fail("codeword");
    codes[4] = 0x1234u;
    if (vorbis_book_codeword(&book, 4) != 0x1234) return fail("codeword reads the list");
    book.c = 0;
    if (vorbis_book_codeword(&book, 0) != -1) return fail("decode-only book");
    book.c = &sc;
    std::printf("00c95c60_TEST PASS\n");
    return 0;
}
