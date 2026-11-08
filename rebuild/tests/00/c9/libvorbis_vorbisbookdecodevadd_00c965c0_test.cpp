// Behaviour test for libvorbis codebook.c vorbis_book_decodev_add (retail 0x00C965C0): a copy of the vector-add decoder over a
// scripted entry reader -- n output samples are accumulated dim at a time (the switch fall-through arm for dim <= 8 and the loop
// arm for dim > 8), the decoded vector is book->valuelist + entry*dim, and -1 is returned the moment the reader hits end of packet.
#include <cstdio>
#include <cstring>
typedef unsigned int ogg_uint32_t;
struct static_codebook;
typedef struct codebook{
  long dim; long entries; long used_entries; const static_codebook *c;
  float *valuelist; ogg_uint32_t *codelist; int *dec_index; char *dec_codelengths; ogg_uint32_t *dec_firsttable;
  int dec_firsttablen; int dec_maxlength;
} codebook;
typedef struct { long endbyte; int endbit; unsigned char *buffer; unsigned char *ptr; long storage; } oggpack_buffer;

/* scripted packet reader: the "packet" is a list of entry numbers, -1 = end of packet */
static const long *gScript = 0; static int gPos = 0;
static long decode_packed_entry_number(codebook*,oggpack_buffer*){ return gScript[gPos++]; }

static long vorbis_book_decodev_add(codebook *book,float *a,oggpack_buffer *b,int n){
  int i,j,entry;
  float *t;

  if(book->dim>8){
    for(i=0;i<n;){
      entry = decode_packed_entry_number(book,b);
      if(entry==-1)return(-1);
      t     = book->valuelist+entry*book->dim;
      for (j=0;j<book->dim;)
	a[i++]+=t[j++];
    }
  }else{
    for(i=0;i<n;){
      entry = decode_packed_entry_number(book,b);
      if(entry==-1)return(-1);
      t     = book->valuelist+entry*book->dim;
      j=0;
      switch((int)book->dim){
      case 8:
	a[i++]+=t[j++];
      case 7:
	a[i++]+=t[j++];
      case 6:
	a[i++]+=t[j++];
      case 5:
	a[i++]+=t[j++];
      case 4:
	a[i++]+=t[j++];
      case 3:
	a[i++]+=t[j++];
      case 2:
	a[i++]+=t[j++];
      case 1:
	a[i++]+=t[j++];
      case 0:
	break;
      }
    }
  }
  return(0);
}

static int fail(const char* what) { std::printf("%s\n00c965c0_TEST FAIL\n", what); return 1; }

int main()
{
    float values[4 * 12]; float a[24]; codebook book; oggpack_buffer opb; int i, d;
    for (i = 0; i < 4 * 12; i++) values[i] = (float)(i + 1) * 0.5f;   /* entry e, element k = (e*dim + k + 1) / 2 */
    memset(&book, 0, sizeof book); book.valuelist = values; book.entries = 4;
    memset(&opb, 0, sizeof opb);

    /* dim 3 (switch arm): entries 2, 0, 3, 1 add their vectors to a[0..11] on top of a baseline of 1 */
    {
        static const long script[] = { 2, 0, 3, 1, -1 };
        book.dim = 3; gScript = script; gPos = 0;
        for (i = 0; i < 24; i++) a[i] = 1.f;
        if (vorbis_book_decodev_add(&book, a, &opb, 12) != 0) return fail("dim 3 return");
        if (gPos != 4) return fail("dim 3 reads");
        for (i = 0; i < 12; i++) {
            long e = script[i / 3]; float want = 1.f + values[e * 3 + i % 3];
            if (a[i] != want) return fail("dim 3 values");
        }
        for (i = 12; i < 24; i++) if (a[i] != 1.f) return fail("dim 3 overrun");
    }
    /* dim 12 (loop arm): two entries fill 24 samples */
    {
        static const long script[] = { 1, 3, -1 };
        book.dim = 12; gScript = script; gPos = 0;
        for (i = 0; i < 24; i++) a[i] = 0.f;
        if (vorbis_book_decodev_add(&book, a, &opb, 24) != 0) return fail("dim 12 return");
        for (i = 0; i < 24; i++) if (a[i] != values[script[i / 12] * 12 + i % 12]) return fail("dim 12 values");
    }
    /* every dim 1..8 takes the right number of entries */
    for (d = 1; d <= 8; d++) {
        static const long script[] = { 0, 0, 0, 0, 0, 0, 0, 0, -1 };
        book.dim = d; gScript = script; gPos = 0;
        for (i = 0; i < 24; i++) a[i] = 0.f;
        if (vorbis_book_decodev_add(&book, a, &opb, d * 2) != 0 || gPos != 2) return fail("dim count");
        for (i = 0; i < d * 2; i++) if (a[i] != values[i % d]) return fail("dim fallthrough");
    }
    /* end of packet in the middle: -1, the samples before it already added */
    {
        static const long script[] = { 1, -1 };
        book.dim = 2; gScript = script; gPos = 0;
        for (i = 0; i < 24; i++) a[i] = 0.f;
        if (vorbis_book_decodev_add(&book, a, &opb, 6) != -1) return fail("eop");
        if (a[0] != values[2] || a[1] != values[3] || a[2] != 0.f) return fail("eop partial");
    }
    std::printf("00c965c0_TEST PASS\n");
    return 0;
}