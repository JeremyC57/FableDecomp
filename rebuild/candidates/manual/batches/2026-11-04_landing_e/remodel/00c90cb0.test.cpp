// Behaviour test for libvorbis vorbis_analysis (0x00C90CB0): the 1.0.1 analysis.c body over scripted callees -- bit counters zeroed,
// the packer reset, mapping 0's forward() result passed through, OV_EINVAL for a bitrate-managed stream when a packet is asked
// for, and otherwise the packet filled from the packer and the block.
#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef __int64 ogg_int64_t;
#else
typedef long long ogg_int64_t;
#endif
#define OV_EINVAL -131
typedef struct { long endbyte; unsigned char* buffer; int resets; } oggpack_buffer;
typedef struct { unsigned char* packet; long bytes; long b_o_s; long e_o_s; ogg_int64_t granulepos; ogg_int64_t packetno; } ogg_packet;
typedef struct vorbis_block { oggpack_buffer opb; int eofflag; ogg_int64_t granulepos; ogg_int64_t sequence;
  long glue_bits, time_bits, floor_bits, res_bits; } vorbis_block;
typedef struct { int (*forward)(struct vorbis_block *vb); } vorbis_func_mapping;
static int g_forwardRet = 0, g_managed = 0, g_forwardCalls = 0;
static int fwd(vorbis_block *vb){ g_forwardCalls++; return g_forwardRet; }
static vorbis_func_mapping g_map0 = { fwd };
static vorbis_func_mapping *_mapping_P[] = { &g_map0 };
static void oggpack_reset(oggpack_buffer *b){ b->resets++; b->endbyte=0; }
static unsigned char *oggpack_get_buffer(oggpack_buffer *b){ return b->buffer; }
static long oggpack_bytes(oggpack_buffer *b){ return b->endbyte + 7; }
static int vorbis_bitrate_managed(vorbis_block *vb){ return g_managed; }
int vorbis_analysis(vorbis_block *vb, ogg_packet *op){
  int ret;
  vb->glue_bits=0; vb->time_bits=0; vb->floor_bits=0; vb->res_bits=0;
  oggpack_reset(&vb->opb);
  if((ret=_mapping_P[0]->forward(vb))) return(ret);
  if(op){
    if(vorbis_bitrate_managed(vb)) return(OV_EINVAL);
    op->packet=oggpack_get_buffer(&vb->opb);
    op->bytes=oggpack_bytes(&vb->opb);
    op->b_o_s=0;
    op->e_o_s=vb->eofflag;
    op->granulepos=vb->granulepos;
    op->packetno=vb->sequence;
  }
  return(0);
}
int main(){
  unsigned char buf[4]; vorbis_block vb; ogg_packet op; int ok=1;
  memset(&vb,0,sizeof vb); memset(&op,0,sizeof op);
  vb.opb.buffer=buf; vb.opb.endbyte=99; vb.glue_bits=vb.time_bits=vb.floor_bits=vb.res_bits=5;
  vb.eofflag=1; vb.granulepos=1234; vb.sequence=42;
  if(vorbis_analysis(&vb,&op)!=0) ok=0;
  if(vb.glue_bits||vb.time_bits||vb.floor_bits||vb.res_bits) ok=0;
  if(vb.opb.resets!=1 || g_forwardCalls!=1) ok=0;
  if(op.packet!=buf || op.bytes!=7 || op.b_o_s!=0 || op.e_o_s!=1 || op.granulepos!=1234 || op.packetno!=42) ok=0;
  g_forwardRet=-3;                                      /* forward() error passes straight through */
  if(vorbis_analysis(&vb,&op)!=-3) ok=0;
  g_forwardRet=0; g_managed=1;                          /* bitrate-managed + packet requested: OV_EINVAL */
  if(vorbis_analysis(&vb,&op)!=OV_EINVAL) ok=0;
  if(vorbis_analysis(&vb,0)!=0) ok=0;                   /* no packet: managed mode is fine */
  printf("00c90cb0_TEST %s\n", ok ? "PASS" : "FAIL");
  return ok ? 0 : 1;
}
