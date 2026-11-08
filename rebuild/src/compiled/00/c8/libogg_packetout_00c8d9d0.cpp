// libogg::_packetout(ogg_stream_state* os, ogg_packet* op, int adv) -- framing.c: the last part of decode -- group the lacing segments after lacing_returned into one packet: return 0 when none is complete, -1 (bumping lacing_returned and packetno) at a 0x400 hole marker, 1 as a cheap "whole packet waiting" probe when op and adv are both zero; otherwise gather the 255-chained segment sizes, fill op (e_o_s / b_o_s flags, body pointer, packetno, granulepos, bytes) and, when adv is set, advance body_returned, lacing_returned and packetno
// Vendor-proof: libogg-1.1.1(src/framing.c) @_packetout@12 -- 311 bytes verified against Fable.exe 0x00C8D9D0
// Leaf: @_packetout@12
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C8D9D0, 311 bytes. libogg 1.1 framing.c `static int _packetout(...)`, kept out of line
//   with a private register convention (os in eax, op in ecx, adv pushed, `ret 4`; under /Gr the C symbol is `@_packetout@12`). Callers: ogg_stream_packetout 0x00C8DB10
//   (`push 1`) and ogg_stream_packetpeek 0x00C8DB20 (`push 0`), both landed under their Xiph names with this body in their TU.
// Identity (two sources): (1) the body is framing.c _packetout verbatim -- lacing_returned (+0x24) vs lacing_packet (+0x20), the
//   0x400 gap marker with the packetno (+0x158) bump and -1, the `!op && !adv` probe, the 0xff / 0x200 / 0x100 size / eos / bos
//   masks and the `size==255` chain, the op stores from body_data+body_returned and granule_vals (+0x14); (2) its two callers are
//   the framing.c API pair right after it in file order. The catalog name `_packetout` is already the Xiph one.
// Shape: the landed ogg_stream_packetout TU (0x00C8DB10) verbatim: framing.c compiled as C with /Gr (extern "C" __fastcall API),
//   the file-static _packetout beside its two callers so VC7.1 /O2 assigns retail's private convention itself (no /GL).
//   Only `__packetout` is this row.
// Original libogg framing.c; BSD-style terms in reference COPYING.
#ifdef _MSC_VER
typedef __int64 ogg_int64_t;
#else
#include <stdint.h>
typedef int64_t ogg_int64_t;
#define __fastcall
#define __declspec(x)
#endif
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
typedef struct {
  unsigned char *packet;
  long  bytes;
  long  b_o_s;
  long  e_o_s;

  ogg_int64_t  granulepos;

  ogg_int64_t  packetno;     /* sequence number for decode; the framing
				knows where there's a hole in the data,
				but we need coupling so that the codec
				(which is in a seperate abstraction
				layer) also knows about the gap */
} ogg_packet;

/* 0x00C8D9D0 */
extern "C" {
static int _packetout(ogg_stream_state *os,ogg_packet *op,int adv){

  /* The last part of decode. We have the stream broken into packet
     segments.  Now we need to group them into packets (or return the
     out of sync markers) */

  int ptr=os->lacing_returned;

  if(os->lacing_packet<=ptr)return(0);

  if(os->lacing_vals[ptr]&0x400){
    /* we need to tell the codec there's a gap; it might need to
       handle previous packet dependencies. */
    os->lacing_returned++;
    os->packetno++;
    return(-1);
  }

  if(!op && !adv)return(1); /* just using peek as an inexpensive way
                               to ask if there's a whole packet
                               waiting */

  /* Gather the whole packet. We'll have no holes or a partial packet */
  {
    int size=os->lacing_vals[ptr]&0xff;
    int bytes=size;
    int eos=os->lacing_vals[ptr]&0x200; /* last packet of the stream? */
    int bos=os->lacing_vals[ptr]&0x100; /* first packet of the stream? */

    while(size==255){
      int val=os->lacing_vals[++ptr];
      size=val&0xff;
      if(val&0x200)eos=0x200;
      bytes+=size;
    }

    if(op){
      op->e_o_s=eos;
      op->b_o_s=bos;
      op->packet=os->body_data+os->body_returned;
      op->packetno=os->packetno;
      op->granulepos=os->granule_vals[ptr];
      op->bytes=bytes;
    }

    if(adv){
      os->body_returned+=bytes;
      os->lacing_returned=ptr+1;
      os->packetno++;
    }
  }
  return(1);
}
}   /* extern "C" */

extern "C" int __fastcall ogg_stream_packetout(ogg_stream_state *os,ogg_packet *op){
  return _packetout(os,op,1);
}

/* 0x00C8DB20 */
extern "C" int __fastcall ogg_stream_packetpeek(ogg_stream_state *os,ogg_packet *op){
  return _packetout(os,op,0);
}