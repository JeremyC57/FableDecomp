#include <stdio.h>
#include <stdlib.h>
// 00c8d170 ogg_stream_flush
// Leaf: @ogg_stream_flush@8
// Flags: /Gr
// Original libogg1.1 framing algorithm; 32bit API and observed fastcall translation-unit convention.
#ifdef _MSC_VER
typedef __int64 ogg_int64_t;
#pragma intrinsic(memcpy)
#else
#include <stdint.h>
typedef int64_t ogg_int64_t;
#define __fastcall
#define __cdecl
#define __declspec(x)
#endif
extern "C" void*__cdecl memcpy(void*,const void*,unsigned int);
extern "C" __declspec(dllimport) void*__cdecl memmove(void*,const void*,unsigned int);
typedef struct {
  unsigned char *header;
  long header_len;
  unsigned char *body;
  long body_len;
} ogg_page;
typedef struct {
  unsigned char   *body_data;    /* bytes from packet bodies */
  long    body_storage;          /* storage elements allocated */
  long    body_fill;             /* elements stored; fill mark */
  long    body_returned;         /* elements of fill returned */


  int     *lacing_vals;      /* The values that will go to the segment table */
  ogg_int64_t *granule_vals; /* granulepos values for headers. Not compact
				this way, but it is simple coupled to the
				lacing fifo */
  long    lacing_storage;
  long    lacing_fill;
  long    lacing_packet;
  long    lacing_returned;

  unsigned char    header[282];      /* working space for header encode */
  int              header_fill;

  int     e_o_s;          /* set when we have buffered the last packet in the
                             logical bitstream */
  int     b_o_s;          /* set after we've written the initial page
                             of a logical bitstream */
  long    serialno;
  long    pageno;
  ogg_int64_t  packetno;      /* sequence number for decode; the framing
                             knows where there's a hole in the data,
                             but we need coupling so that the codec
                             (which is in a seperate abstraction
                             layer) also knows about the gap */
  ogg_int64_t   granulepos;

} ogg_stream_state;
extern "C" void __fastcall ogg_page_checksum_set(ogg_page*);
extern "C" int __fastcall ogg_stream_flush(ogg_stream_state *os,ogg_page *og){
  int i;
  int vals=0;
  int maxvals=(os->lacing_fill>255?255:os->lacing_fill);
  int bytes=0;
  long acc=0;
  ogg_int64_t granule_pos=os->granule_vals[0];

  if(maxvals==0)return(0);
  
  /* construct a page */
  /* decide how many segments to include */
  
  /* If this is the initial header case, the first page must only include
     the initial header packet */
  if(os->b_o_s==0){  /* 'initial header page' case */
    granule_pos=0;
    for(vals=0;vals<maxvals;vals++){
      if((os->lacing_vals[vals]&0x0ff)<255){
	vals++;
	break;
      }
    }
  }else{
    for(vals=0;vals<maxvals;vals++){
      if(acc>4096)break;
      acc+=os->lacing_vals[vals]&0x0ff;
      granule_pos=os->granule_vals[vals];
    }
  }
  
  /* construct the header in temp storage */
  memcpy(os->header,"OggS",4);
  
  /* stream structure version */
  os->header[4]=0x00;
  
  /* continued packet flag? */
  os->header[5]=0x00;
  if((os->lacing_vals[0]&0x100)==0)os->header[5]|=0x01;
  /* first page flag? */
  if(os->b_o_s==0)os->header[5]|=0x02;
  /* last page flag? */
  if(os->e_o_s && os->lacing_fill==vals)os->header[5]|=0x04;
  os->b_o_s=1;

  /* 64 bits of PCM position */
  for(i=6;i<14;i++){
    os->header[i]=(granule_pos&0xff);
    granule_pos>>=8;
  }

  /* 32 bits of stream serial number */
  {
    long serialno=os->serialno;
    for(i=14;i<18;i++){
      os->header[i]=(serialno&0xff);
      serialno>>=8;
    }
  }

  /* 32 bits of page counter (we have both counter and page header
     because this val can roll over) */
  if(os->pageno==-1)os->pageno=0; /* because someone called
				     stream_reset; this would be a
				     strange thing to do in an
				     encode stream, but it has
				     plausible uses */
  {
    long pageno=os->pageno++;
    for(i=18;i<22;i++){
      os->header[i]=(pageno&0xff);
      pageno>>=8;
    }
  }
  
  /* zero for computation; filled in later */
  os->header[22]=0;
  os->header[23]=0;
  os->header[24]=0;
  os->header[25]=0;
  
  /* segment table */
  os->header[26]=vals&0xff;
  for(i=0;i<vals;i++)
    bytes+=os->header[i+27]=(os->lacing_vals[i]&0xff);
  
  /* set pointers in the ogg_page struct */
  og->header=os->header;
  og->header_len=os->header_fill=vals+27;
  og->body=os->body_data+os->body_returned;
  og->body_len=bytes;
  
  /* advance the lacing data and set the body_returned pointer */
  
  os->lacing_fill-=vals;
  memmove(os->lacing_vals,os->lacing_vals+vals,os->lacing_fill*sizeof(*os->lacing_vals));
  memmove(os->granule_vals,os->granule_vals+vals,os->lacing_fill*sizeof(*os->granule_vals));
  os->body_returned+=bytes;
  
  /* calculate the checksum */
  
  ogg_page_checksum_set(og);

  /* done */
  return(1);
}
typedef char page_size[sizeof(ogg_page)==16?1:-1];
typedef char stream_size[sizeof(ogg_stream_state)==360?1:-1];

static void check(bool p){if(!p)abort();}
static int checksumCalls;
extern "C" void __fastcall ogg_page_checksum_set(ogg_page*page){++checksumCalls;check(page->header_len>=27);for(int i=22;i<26;++i)check(page->header[i]==0);page->header[22]=0x12;page->header[23]=0x34;page->header[24]=0x56;page->header[25]=0x78;}
struct Case {int fill,kind,bos,selected;};
static const Case config[]={{0,0,0,0},{5,0,0,1},{3,1,0,3},{300,2,0,255},{3,1,1,3},{300,2,1,17},{300,3,1,255},{300,4,1,241},{20,5,1,20}};
static unsigned char body[80000];static int laces[320],originalLaces[320];static ogg_int64_t granules[320],originalGranules[320];
int main(){check(sizeof(ogg_stream_state)==360&&sizeof(ogg_page)==16);for(unsigned i=0;i<sizeof body;++i)body[i]=(unsigned char)(i*31+7);unsigned casesTested=0;for(unsigned c=0;c<sizeof(config)/sizeof(config[0]);++c)for(int eos=0;eos<2;++eos)for(int seq=0;seq<3;++seq)for(int marked=0;marked<2;++marked){const Case&v=config[c];ogg_stream_state os;unsigned char*raw=(unsigned char*)&os;for(unsigned i=0;i<sizeof os;++i)raw[i]=0;os.body_data=body;os.body_storage=sizeof body;os.body_fill=77000;os.body_returned=11;os.lacing_vals=laces;os.granule_vals=granules;os.lacing_storage=320;os.lacing_fill=v.fill;os.b_o_s=v.bos;os.e_o_s=eos;os.serialno=-12345;os.pageno=seq==0?-1:(seq==1?0:73);os.packetno=91;os.granulepos=87;int bytes=0;for(int i=0;i<320;++i){int amount=v.kind==0?7:(v.kind==1?(i==2?7:255):(v.kind==2?255:(v.kind==3?16:(v.kind==4?17:0))));laces[i]=amount;granules[i]=seq==2?-ogg_int64_t(i+1):((ogg_int64_t(0x01020304)<<32)|ogg_int64_t(i));if(i<v.selected)bytes+=amount;}if(marked)laces[0]|=0x100;for(int i=0;i<320;++i){originalLaces[i]=laces[i];originalGranules[i]=granules[i];}ogg_page page;page.header=(unsigned char*)1;page.header_len=7;page.body=(unsigned char*)2;page.body_len=9;checksumCalls=0;int result=ogg_stream_flush(&os,&page);if(v.fill==0){check(result==0&&checksumCalls==0&&os.lacing_fill==0&&os.body_returned==11&&page.header==(unsigned char*)1);continue;}check(result==1&&checksumCalls==1);check(page.header==os.header&&page.header_len==v.selected+27&&os.header_fill==v.selected+27&&page.body==body+11&&page.body_len==bytes);check(page.header[0]=='O'&&page.header[1]=='g'&&page.header[2]=='g'&&page.header[3]=='S'&&page.header[4]==0);int flags=(!marked?1:0)|(!v.bos?2:0)|((eos&&v.fill==v.selected)?4:0);check(page.header[5]==flags&&page.header[26]==v.selected);ogg_int64_t position=v.bos?originalGranules[v.selected-1]:0;for(int i=0;i<8;++i){check(page.header[6+i]==(unsigned char)(position&255));position>>=8;}unsigned long serial=(unsigned long)(-12345);unsigned long pageno=seq==0?0:(seq==1?0:73);for(int i=0;i<4;++i){check(page.header[14+i]==(unsigned char)((serial>>(i*8))&255));check(page.header[18+i]==(unsigned char)((pageno>>(i*8))&255));}check(page.header[22]==0x12&&page.header[23]==0x34&&page.header[24]==0x56&&page.header[25]==0x78);for(int i=0;i<v.selected;++i)check(page.header[27+i]==(originalLaces[i]&255));check(os.b_o_s==1&&os.pageno==long(pageno+1)&&os.body_returned==11+bytes&&os.lacing_fill==v.fill-v.selected&&os.packetno==91&&os.granulepos==87);for(int i=0;i<os.lacing_fill;++i)check(laces[i]==originalLaces[i+v.selected]&&granules[i]==originalGranules[i+v.selected]);++casesTested;}check(casesTested==96);puts("00c8d170_TEST PASS");return 0;}