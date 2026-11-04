// Behaviour test for libvorbis vorbis_bitrate_managed (0x00C9CFD0): 1 exactly when the bitrate manager's queue_binned is set.
#include <cstdio>
typedef unsigned int ogg_uint32_t;
typedef struct { ogg_uint32_t *queue_binned; } bitrate_manager_state;
typedef struct { bitrate_manager_state bms; } private_state;
typedef struct { void *backend_state; } vorbis_dsp_state;
typedef struct { vorbis_dsp_state *vd; } vorbis_block;
int vorbis_bitrate_managed(vorbis_block *vb){
  vorbis_dsp_state      *vd=vb->vd;
  private_state         *b=(private_state*)vd->backend_state;
  bitrate_manager_state *bm=&b->bms;
  if(bm->queue_binned)return(1);
  return(0);
}
int main(){
  ogg_uint32_t q[2]; private_state ps; vorbis_dsp_state vd; vorbis_block vb; int ok=1;
  vd.backend_state=&ps; vb.vd=&vd;
  ps.bms.queue_binned=0; if(vorbis_bitrate_managed(&vb)!=0) ok=0;
  ps.bms.queue_binned=q; if(vorbis_bitrate_managed(&vb)!=1) ok=0;
  printf("00c9cfd0_TEST %s\n", ok ? "PASS" : "FAIL");
  return ok ? 0 : 1;
}