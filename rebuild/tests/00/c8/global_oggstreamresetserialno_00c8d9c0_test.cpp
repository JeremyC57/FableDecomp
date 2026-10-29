#include <stdio.h>
#include <stdlib.h>
// 00c8d9c0 ogg_stream_reset_serialno
// Leaf: @ogg_stream_reset_serialno@8
// Flags: /Gr
// Original libogg 1.1 public C API; same-file reset body preserves register knowledge.
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
extern "C" __declspec(noinline) int __fastcall ogg_stream_reset(ogg_stream_state *os){
  os->body_fill=0;
  os->body_returned=0;

  os->lacing_fill=0;
  os->lacing_packet=0;
  os->lacing_returned=0;

  os->header_fill=0;

  os->e_o_s=0;
  os->b_o_s=0;
  os->pageno=-1;
  os->packetno=0;
  os->granulepos=0;

  return(0);
}
extern "C" int __fastcall ogg_stream_reset_serialno(ogg_stream_state *os,int serialno){
  ogg_stream_reset(os);
  os->serialno=serialno;
  return(0);
}
typedef char stream_size[sizeof(ogg_stream_state)==360?1:-1];

static void check(bool value){if(!value)abort();}
static const int serials[]={0,1,-1,0x7fffffff,(-0x7fffffff-1),123456,-7654321};
int main(){check(sizeof(ogg_stream_state)==360);for(unsigned pattern=0;pattern<5;++pattern)for(unsigned n=0;n<sizeof(serials)/sizeof(serials[0]);++n){ogg_stream_state state;unsigned char *raw=(unsigned char*)&state;unsigned char expected[sizeof state];for(unsigned i=0;i<sizeof state;++i)raw[i]=expected[i]=(unsigned char)(pattern==0?0:(pattern==1?255:(i*(pattern*13)+17)));int result=ogg_stream_reset_serialno(&state,serials[n]);check(result==0);const unsigned zeroOffsets[]={8,12,28,32,36,324,328,332,344,348,352,356};for(unsigned j=0;j<sizeof(zeroOffsets)/sizeof(zeroOffsets[0]);++j)for(unsigned b=0;b<4;++b)expected[zeroOffsets[j]+b]=0;for(unsigned b=0;b<4;++b){expected[340+b]=255;expected[336+b]=(unsigned char)((unsigned int)serials[n]>>(8*b));}for(unsigned i=0;i<sizeof state;++i)check(raw[i]==expected[i]);check(state.serialno==serials[n]&&state.pageno==-1&&state.packetno==0&&state.granulepos==0);check(ogg_stream_reset_serialno(&state,serials[n])==0);for(unsigned i=0;i<sizeof state;++i)check(raw[i]==expected[i]);}puts("00c8d9c0_TEST PASS");return 0;}