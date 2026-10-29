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