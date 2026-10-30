// Behaviour test for 00c8db20 (adapted from the landed host row 0x00c8db10's test, whose TU exercises this function through its real caller).
// Behaviour test for 0x00c8db10: the candidate source below is embedded verbatim, followed by stand-ins for its callees
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int g_ok = 1;
#define CHECK(c) do { if (!(c)) { g_ok = 0; printf("check failed line %d\n", __LINE__); } } while (0)
// ---- embedded candidate source ----
// ogg_stream_packetout(ogg_stream_state* os, ogg_packet* op) -- libogg framing.c: take the next whole packet out of the stream (advance)
// Leaf: @ogg_stream_packetout@8
// Flags: /Gr
// Retail: Fable.exe 0x00C8DB10, 12 bytes (`mov eax,ecx; push 1; mov ecx,edx; call 0x00C8D9D0; ret`).
// Library: libpk statically links Xiph libogg 1.1 / libvorbis (post-1.0.1 CVS), compiled as C with /Gr (extern "C" __fastcall).
// Identity: framing.c `int ogg_stream_packetout(os,op){ return _packetout(os,op,1); }` and its twin ogg_stream_packetpeek
//   0x00C8DB20 (`push 0`) right after it, both forwarding to the file-static _packetout 0x00C8D9D0 (lacing_returned +0x24 vs
//   lacing_packet +0x20, the 0x400 gap bit, packetno +0x158, granule_vals +0x14 -- framing.c's body). Callers: vorbisfile
//   _fetch_and_process_packet etc. (`lea ecx,[vf+0x78]` = &vf->os). The catalog label `std::_Dest_val<...CFlash>` is propagated.
// Shape: _packetout is a same-TU static, so VC7.1 gives it a private register convention (os in eax, op in ecx, adv on the
//   stack, callee-pop) -- the `mov eax,ecx` is the caller moving os into that register.
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

extern "C" int __fastcall ogg_stream_packetout(ogg_stream_state *os,ogg_packet *op){
  return _packetout(os,op,1);
}

/* 0x00C8DB20 */
extern "C" int __fastcall ogg_stream_packetpeek(ogg_stream_state *os,ogg_packet *op){
  return _packetout(os,op,0);
}

// ---- end of embedded source ----
int main()
{
    ogg_stream_state os;
    memset(&os, 0, sizeof(os));
    unsigned char body[1024];
    int lacing[8];
    ogg_int64_t granules[8];
    for (int i = 0; i < 1024; ++i) body[i] = (unsigned char)i;
    lacing[0] = 0x100 | 255;            // bos, continued packet
    lacing[1] = 10;                     // 255 + 10 = 265 bytes
    lacing[2] = 0x400;                  // gap marker
    lacing[3] = 0x200 | 7;              // eos packet of 7 bytes
    for (int g = 0; g < 8; ++g) granules[g] = 1000 + g;
    os.body_data = body; os.lacing_vals = lacing; os.granule_vals = granules;
    os.lacing_fill = 4; os.lacing_packet = 4; os.packetno = 5;
    ogg_packet op;
    memset(&op, 0, sizeof(op));
    CHECK(ogg_stream_packetpeek(&os, 0) == 1);          // a whole packet waits, peek does not advance
    CHECK(os.lacing_returned == 0);
    CHECK(ogg_stream_packetout(&os, &op) == 1);
    CHECK(op.bytes == 265 && op.b_o_s == 0x100 && op.e_o_s == 0 && op.packet == body && op.packetno == 5);
    CHECK(op.granulepos == 1001);
    CHECK(os.body_returned == 265 && os.lacing_returned == 2 && os.packetno == 6);
    CHECK(ogg_stream_packetout(&os, &op) == -1);         // the gap is reported once
    CHECK(os.lacing_returned == 3 && os.packetno == 7);
    CHECK(ogg_stream_packetout(&os, &op) == 1);
    CHECK(op.bytes == 7 && op.e_o_s == 0x200 && op.packet == body + 265 && op.granulepos == 1003);
    CHECK(ogg_stream_packetout(&os, &op) == 0);          // nothing left
    if (g_ok) printf("00c8db20_TEST PASS\n");
    return g_ok ? 0 : 1;
}