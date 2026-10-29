#include <stdio.h>
#include <string.h>
// 00c95ca0 vorbis_staticbook_pack
// Leaf: @vorbis_staticbook_pack@8
// Flags: /Gr
// Original libvorbis codebook.c/headers; BSD-style terms in reference COPYING.
// C local this is renamed thisLength because it is a C++ keyword.
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#endif
typedef struct static_codebook{
  long   dim;            /* codebook dimensions (elements per vector) */
  long   entries;        /* codebook entries */
  long  *lengthlist;     /* codeword lengths in bits */

  /* mapping ***************************************************************/
  int    maptype;        /* 0=none
			    1=implicitly populated values from map column 
			    2=listed arbitrary values */

  /* The below does a linear, single monotonic sequence mapping. */
  long     q_min;       /* packed 32 bit float; quant value 0 maps to minval */
  long     q_delta;     /* packed 32 bit float; val 1 - val 0 == delta */
  int      q_quant;     /* bits: 0 < quant <= 16 */
  int      q_sequencep; /* bitflag */

  long     *quantlist;  /* map == 1: (int)(entries^(1/dim)) element column map
			   map == 2: list of dim*entries quantized entry vals
			*/

  /* encode helpers ********************************************************/
  struct encode_aux_nearestmatch *nearest_tree;
  struct encode_aux_threshmatch  *thresh_tree;
  struct encode_aux_pigeonhole  *pigeon_tree;

  int allocedp;
} static_codebook;
typedef struct {
  long endbyte;
  int  endbit;

  unsigned char *buffer;
  unsigned char *ptr;
  long storage;
} oggpack_buffer;
extern "C" void __fastcall oggpack_write(oggpack_buffer*,unsigned long,int);
extern "C" int __fastcall _ilog(unsigned int);
extern "C" long __fastcall _book_maptype1_quantvals(const static_codebook*);
extern "C" int __fastcall vorbis_staticbook_pack(const static_codebook *c,oggpack_buffer *opb){
  long i,j;
  int ordered=0;

  /* first the basic parameters */
  oggpack_write(opb,0x564342,24);
  oggpack_write(opb,c->dim,16);
  oggpack_write(opb,c->entries,24);

  /* pack the codewords.  There are two packings; length ordered and
     length random.  Decide between the two now. */
  
  for(i=1;i<c->entries;i++)
    if(c->lengthlist[i-1]==0 || c->lengthlist[i]<c->lengthlist[i-1])break;
  if(i==c->entries)ordered=1;
  
  if(ordered){
    /* length ordered.  We only need to say how many codewords of
       each length.  The actual codewords are generated
       deterministically */

    long count=0;
    oggpack_write(opb,1,1);  /* ordered */
    oggpack_write(opb,c->lengthlist[0]-1,5); /* 1 to 32 */

    for(i=1;i<c->entries;i++){
      long thisLength=c->lengthlist[i];
      long last=c->lengthlist[i-1];
      if(thisLength>last){
	for(j=last;j<thisLength;j++){
	  oggpack_write(opb,i-count,_ilog(c->entries-count));
	  count=i;
	}
      }
    }
    oggpack_write(opb,i-count,_ilog(c->entries-count));
    
  }else{
    /* length random.  Again, we don't code the codeword itself, just
       the length.  This time, though, we have to encode each length */
    oggpack_write(opb,0,1);   /* unordered */
    
    /* algortihmic mapping has use for 'unused entries', which we tag
       here.  The algorithmic mapping happens as usual, but the unused
       entry has no codeword. */
    for(i=0;i<c->entries;i++)
      if(c->lengthlist[i]==0)break;

    if(i==c->entries){
      oggpack_write(opb,0,1); /* no unused entries */
      for(i=0;i<c->entries;i++)
	oggpack_write(opb,c->lengthlist[i]-1,5);
    }else{
      oggpack_write(opb,1,1); /* we have unused entries; thus we tag */
      for(i=0;i<c->entries;i++){
	if(c->lengthlist[i]==0){
	  oggpack_write(opb,0,1);
	}else{
	  oggpack_write(opb,1,1);
	  oggpack_write(opb,c->lengthlist[i]-1,5);
	}
      }
    }
  }

  /* is the entry number the desired return value, or do we have a
     mapping? If we have a mapping, what type? */
  oggpack_write(opb,c->maptype,4);
  switch(c->maptype){
  case 0:
    /* no mapping */
    break;
  case 1:case 2:
    /* implicitly populated value mapping */
    /* explicitly populated value mapping */
    
    if(!c->quantlist){
      /* no quantlist?  error */
      return(-1);
    }
    
    /* values that define the dequantization */
    oggpack_write(opb,c->q_min,32);
    oggpack_write(opb,c->q_delta,32);
    oggpack_write(opb,c->q_quant-1,4);
    oggpack_write(opb,c->q_sequencep,1);
    
    {
      int quantvals;
      switch(c->maptype){
      case 1:
	/* a single column of (c->entries/c->dim) quantized values for
	   building a full value list algorithmically (square lattice) */
	quantvals=_book_maptype1_quantvals(c);
	break;
      case 2:
	/* every value (c->entries*c->dim total) specified explicitly */
	quantvals=c->entries*c->dim;
	break;
      default: /* NOT_REACHABLE */
	quantvals=-1;
      }

      /* quantized values */
      for(i=0;i<quantvals;i++)
	oggpack_write(opb,labs(c->quantlist[i]),c->q_quant);

    }
    break;
  default:
    /* error case; we don't have any other map types now */
    return(-1);
  }

  return(0);
}

static unsigned cases,bitCount,readBit;
static unsigned char bits[4098];
static void check(bool p){if(!p){fprintf(stderr,"book pack case %u failed\n",cases);abort();}}
// Dependency contract for the original LSB-first bit writer.
extern "C" void __fastcall oggpack_write(oggpack_buffer*b,unsigned long value,int count){check(count>=0&&count<=32&&bitCount+count<32768);for(int k=0;k<count;++k){unsigned mask=1U<<(bitCount%8);if((value>>k)&1U)bits[1+bitCount/8]|=(unsigned char)mask;else bits[1+bitCount/8]&=(unsigned char)~mask;++bitCount;}b->endbit=bitCount%8;b->endbyte=bitCount/8;}
extern "C" int __fastcall _ilog(unsigned value){int count=0;while(value){++count;value>>=1;}return count;}
extern "C" long __fastcall _book_maptype1_quantvals(const static_codebook*b){int q=1;for(;;){long n=1;for(int i=0;i<b->dim;++i)n*=q+1;if(n>b->entries)return q;++q;}}
static unsigned take(int count){check(count>=0&&count<=32&&readBit+count<=bitCount);unsigned result=0;for(int k=0;k<count;++k){result|=((bits[1+readBit/8]>>(readBit%8))&1U)<<k;++readBit;}return result;}
static void readHeader(const static_codebook*b){check(take(24)==0x564342&&take(16)==(unsigned)b->dim&&take(24)==(unsigned)b->entries);bool ordered=take(1)!=0;if(ordered){unsigned length=take(5)+1;int at=0;while(at<b->entries){check(length<=32);unsigned count=take(_ilog(b->entries-at));check(count<=(unsigned)(b->entries-at));for(unsigned k=0;k<count;++k)check(b->lengthlist[at++]==(long)length);++length;}}else{bool sparse=take(1)!=0;for(int i=0;i<b->entries;++i){bool used=!sparse||take(1);long len=used?(long)take(5)+1:0;check(len==b->lengthlist[i]);}}check(take(4)==(unsigned)b->maptype);}
int main(){const int sizes[]={1,2,4,9,16};for(int size=0;size<5;++size)for(int shape=0;shape<5;++shape)for(int dim=1;dim<=3;++dim)for(int kind=0;kind<4;++kind)for(int seq=0;seq<2;++seq)for(int present=0;present<2;++present){
 static_codebook book;oggpack_buffer opb;memset(&book,0,sizeof book);memset(&opb,0,sizeof opb);long lengths[16],oldLengths[16],quant[48],oldQuant[48];int n=sizes[size];for(int i=0;i<16;++i){lengths[i]=shape==0?3:(shape==1?1+i/3:(shape==2?1+(i*5+2)%7:(shape==3?(n>1&&i%3==0?0:4):32)));oldLengths[i]=lengths[i];}for(int i=0;i<48;++i)quant[i]=oldQuant[i]=(i*7)%29-14;book.dim=dim;book.entries=n;book.lengthlist=lengths;book.maptype=kind;book.q_min=0x61012345;book.q_delta=0x61234567;book.q_quant=5;book.q_sequencep=seq;book.quantlist=present?quant:0;static_codebook original=book;memset(bits,0,sizeof bits);bits[0]=0x5a;bits[4097]=0xa5;bitCount=readBit=0;
 int result=vorbis_staticbook_pack(&book,&opb);readHeader(&book);bool good=kind==0||((kind==1||kind==2)&&present);check(result==(good?0:-1));if(good&&kind){check(take(32)==(unsigned)book.q_min&&take(32)==(unsigned)book.q_delta);check(take(4)+1==(unsigned)book.q_quant&&take(1)==(unsigned)seq);int count=kind==1?_book_maptype1_quantvals(&book):n*dim;for(int i=0;i<count;++i){long value=quant[i]<0?-quant[i]:quant[i];check(take(book.q_quant)==(unsigned)value);}}check(readBit==bitCount&&opb.endbyte==(long)(bitCount/8)&&opb.endbit==(int)(bitCount%8));check(bits[0]==0x5a&&bits[4097]==0xa5);check(memcmp(&book,&original,sizeof book)==0&&memcmp(lengths,oldLengths,sizeof lengths)==0&&memcmp(quant,oldQuant,sizeof quant)==0);++cases;
 }check(cases==1200);puts("00c95ca0_TEST PASS");return 0;}