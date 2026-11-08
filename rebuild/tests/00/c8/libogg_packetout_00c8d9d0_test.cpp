// Behaviour test for libogg framing.c _packetout (retail 0x00C8D9D0): a copy over a hand-built ogg_stream_state -- a
// 255-chained two-segment packet with the eos flag, the gap marker (-1 and a packetno bump), the cheap peek probe, peek vs
// advance, and 0 when no whole packet is buffered.
#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef __int64 ogg_int64_t;
#else
typedef long long ogg_int64_t;
#endif
typedef struct {
  unsigned char   *body_data; long body_storage; long body_fill; long body_returned;
  int     *lacing_vals; ogg_int64_t *granule_vals; long lacing_storage; long lacing_fill; long lacing_packet; long lacing_returned;
  unsigned char    header[282]; int header_fill; int e_o_s; int b_o_s; long serialno; long pageno;
  ogg_int64_t  packetno; ogg_int64_t   granulepos;
} ogg_stream_state;
typedef struct { unsigned char *packet; long bytes; long b_o_s; long e_o_s; ogg_int64_t granulepos; ogg_int64_t packetno; } ogg_packet;

static int _packetout(ogg_stream_state *os,ogg_packet *op,int adv){
  int ptr=os->lacing_returned;
  if(os->lacing_packet<=ptr)return(0);
  if(os->lacing_vals[ptr]&0x400){
    os->lacing_returned++;
    os->packetno++;
    return(-1);
  }
  if(!op && !adv)return(1);
  {
    int size=os->lacing_vals[ptr]&0xff;
    int bytes=size;
    int eos=os->lacing_vals[ptr]&0x200;
    int bos=os->lacing_vals[ptr]&0x100;
    while(size==255){
      int val=os->lacing_vals[++ptr];
      size=val&0xff;
      if(val&0x200)eos=0x200;
      bytes+=size;
    }
    if(op){
      op->e_o_s=eos; op->b_o_s=bos;
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

static int fail() { std::printf("00c8d9d0_TEST FAIL\n"); return 1; }

int main()
{
    ogg_stream_state os; ogg_packet op;
    unsigned char body[600];
    int lacing[8]; ogg_int64_t gran[8];
    std::memset(&os, 0, sizeof(os)); std::memset(body, 0, sizeof(body));
    os.body_data = body; os.lacing_vals = lacing; os.granule_vals = gran;
    /* packet A: 255 + 10 bytes, bos, eos on the second segment; then a gap marker; then packet B of 7 bytes */
    lacing[0] = 255 | 0x100; gran[0] = -1;
    lacing[1] = 10 | 0x200;  gran[1] = 1000;
    lacing[2] = 0x400;       gran[2] = -1;
    lacing[3] = 7;           gran[3] = 2000;
    os.lacing_packet = 4; os.lacing_returned = 0; os.body_returned = 0; os.packetno = 3;

    if (_packetout(&os, 0, 0) != 1) return fail();                     /* the cheap probe: a packet is waiting */
    if (_packetout(&os, &op, 0) != 1) return fail();                   /* peek */
    if (op.bytes != 265 || op.b_o_s != 0x100 || op.e_o_s != 0x200 || op.packet != body || op.granulepos != 1000 || op.packetno != 3) return fail();
    if (os.lacing_returned != 0 || os.body_returned != 0 || os.packetno != 3) return fail();   /* peek moved nothing */
    if (_packetout(&os, &op, 1) != 1) return fail();                   /* advance */
    if (os.lacing_returned != 2 || os.body_returned != 265 || os.packetno != 4) return fail();
    if (_packetout(&os, &op, 1) != -1) return fail();                  /* the gap */
    if (os.lacing_returned != 3 || os.packetno != 5) return fail();
    if (_packetout(&os, &op, 1) != 1) return fail();
    if (op.bytes != 7 || op.packet != body + 265 || op.granulepos != 2000 || op.packetno != 5 || op.e_o_s != 0 || op.b_o_s != 0) return fail();
    if (_packetout(&os, &op, 1) != 0) return fail();                   /* nothing left */
    if (_packetout(&os, 0, 0) != 0) return fail();
    std::printf("00c8d9d0_TEST PASS\n");
    return 0;
}