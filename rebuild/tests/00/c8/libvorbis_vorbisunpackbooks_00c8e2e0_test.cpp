// Behaviour test for libvorbis info.c _vorbis_unpack_books (retail 0x00C8E2E0): a copy of the setup-header body reader over a model
// codec setup, fed by libogg's bit writer and reader -- books-1 (8 bits) + one static book unpack per book, the 6/16-bit time
// placeholders (type must be < VI_TIMEB), floors / residues / maps (type 16 bits, bounds-checked, then the backend unpack), the
// modes (1/16/16/8 bits, range-checked) and the framing bit; OV_EFAULT without a codec setup, OV_EBADHEADER (after
// vorbis_info_clear) on a bad time type, a floor type out of range, a mode whose mapping index is too large, or a missing framing bit.
#include <cstdio>
#include <cstring>
#include <cstdlib>
typedef struct { long endbyte; int endbit; unsigned char *buffer; unsigned char *ptr; long storage; } oggpack_buffer;
typedef struct vorbis_info { int version; int channels; long rate; long bitrate_upper; long bitrate_nominal; long bitrate_lower; long bitrate_window; void *codec_setup; } vorbis_info;
typedef struct { long dim; long entries; } static_codebook;   /* the model book: 2 fields of the 0x34-byte original */
typedef void vorbis_info_floor; typedef void vorbis_info_residue; typedef void vorbis_info_mapping;
typedef struct { int blockflag; int windowtype; int transformtype; int mapping; } vorbis_info_mode;
typedef struct { void (*pack)(vorbis_info_floor *,oggpack_buffer *); vorbis_info_floor *(*unpack)(vorbis_info *,oggpack_buffer *); } vorbis_func_floor;
typedef struct { void (*pack)(vorbis_info_residue *,oggpack_buffer *); vorbis_info_residue *(*unpack)(vorbis_info *,oggpack_buffer *); } vorbis_func_residue;
typedef struct { void (*pack)(vorbis_info *,vorbis_info_mapping *,oggpack_buffer *); vorbis_info_mapping *(*unpack)(vorbis_info *,oggpack_buffer *); } vorbis_func_mapping;
typedef struct codec_setup_info {
  long blocksizes[2]; int modes; int maps; int floors; int residues; int books; int psys;
  vorbis_info_mode *mode_param[64]; int map_type[64]; vorbis_info_mapping *map_param[64];
  int floor_type[64]; vorbis_info_floor *floor_param[64]; int residue_type[64]; vorbis_info_residue *residue_param[64];
  static_codebook *book_param[256];
} codec_setup_info;
#define OV_EFAULT -129
#define OV_EBADHEADER -133
#define VI_TIMEB 1
#define VI_FLOORB 2
#define VI_RESB 3
#define VI_MAPB 1
#define VI_WINDOWB 1
#define _ogg_calloc calloc

/* libogg bitwise.c (LSb first) */
static const unsigned long mask[]={0x00000000,0x00000001,0x00000003,0x00000007,0x0000000f,0x0000001f,0x0000003f,0x0000007f,0x000000ff,0x000001ff,0x000003ff,0x000007ff,0x00000fff,0x00001fff,0x00003fff,0x00007fff,0x0000ffff,0x0001ffff,0x0003ffff,0x0007ffff,0x000fffff,0x001fffff,0x003fffff,0x007fffff,0x00ffffff,0x01ffffff,0x03ffffff,0x07ffffff,0x0fffffff,0x1fffffff,0x3fffffff,0x7fffffff,0xffffffff};
static void oggpack_writeinit(oggpack_buffer *b){ memset(b,0,sizeof(*b)); b->ptr=b->buffer=(unsigned char*)calloc(256,1); b->storage=256; }
static void oggpack_write(oggpack_buffer *b,unsigned long value,int bits){
  if(b->endbyte+4>=b->storage){ b->buffer=(unsigned char*)realloc(b->buffer,b->storage+256); memset(b->buffer+b->storage,0,256); b->storage+=256; b->ptr=b->buffer+b->endbyte; }
  value&=mask[bits]; bits+=b->endbit;
  b->ptr[0]|=value<<b->endbit;
  if(bits>=8){ b->ptr[1]=(unsigned char)(value>>(8-b->endbit));
    if(bits>=16){ b->ptr[2]=(unsigned char)(value>>(16-b->endbit));
      if(bits>=24){ b->ptr[3]=(unsigned char)(value>>(24-b->endbit));
        if(bits>=32){ if(b->endbit) b->ptr[4]=(unsigned char)(value>>(32-b->endbit)); else b->ptr[4]=0; } } } }
  b->endbyte+=bits/8; b->ptr+=bits/8; b->endbit=bits&7;
}
static void oggpack_readinit(oggpack_buffer *b,unsigned char *buf,int bytes){ memset(b,0,sizeof(*b)); b->buffer=b->ptr=buf; b->storage=bytes; }
static long oggpack_read(oggpack_buffer *b,int bits){
  unsigned long ret; unsigned long m=mask[bits];
  bits+=b->endbit;
  if(b->endbyte+4>=b->storage){ ret=-1UL; if(b->endbyte*8+bits>b->storage*8)goto overflow; }
  ret=b->ptr[0]>>b->endbit;
  if(bits>8){ ret|=b->ptr[1]<<(8-b->endbit);
    if(bits>16){ ret|=b->ptr[2]<<(16-b->endbit);
      if(bits>24){ ret|=b->ptr[3]<<(24-b->endbit);
        if(bits>32 && b->endbit){ ret|=b->ptr[4]<<(32-b->endbit); } } } }
  ret&=m;
 overflow:
  b->ptr+=bits/8; b->endbyte+=bits/8; b->endbit=bits&7;
  return(ret);
}

/* model backends: each unpack reads one 8-bit value and hands back a tagged object; the floor returns NULL on tag 0xee */
static int gBooks = 0, gFloors = 0, gRes = 0, gMaps = 0, gClears = 0;
static int vorbis_staticbook_unpack(oggpack_buffer *opb,static_codebook *s){ gBooks++; s->dim=oggpack_read(opb,16); s->entries=oggpack_read(opb,24); return s->entries<0; }
static vorbis_info_floor *floor_unpack(vorbis_info *,oggpack_buffer *opb){ int *t=(int*)calloc(1,sizeof(int)); *t=oggpack_read(opb,8); gFloors++; if(*t==0xee){ free(t); return 0; } return t; }
static vorbis_info_residue *res_unpack(vorbis_info *,oggpack_buffer *opb){ int *t=(int*)calloc(1,sizeof(int)); *t=oggpack_read(opb,8); gRes++; return t; }
static vorbis_info_mapping *map_unpack(vorbis_info *,oggpack_buffer *opb){ int *t=(int*)calloc(1,sizeof(int)); *t=oggpack_read(opb,8); gMaps++; return t; }
static vorbis_func_floor floorA = { 0, floor_unpack }, floorB = { 0, floor_unpack };
static vorbis_func_residue resA = { 0, res_unpack };
static vorbis_func_mapping mapA = { 0, map_unpack };
static vorbis_func_floor*   _floor_P[2]   = { &floorA, &floorB };
static vorbis_func_residue* _residue_P[3] = { &resA, &resA, &resA };
static vorbis_func_mapping* _mapping_P[1] = { &mapA };
static void vorbis_info_clear(vorbis_info *vi){ gClears++; }

static int _vorbis_unpack_books(vorbis_info *vi,oggpack_buffer *opb){
  codec_setup_info     *ci=(codec_setup_info *)vi->codec_setup;
  int i;
  if(!ci)return(OV_EFAULT);

  /* codebooks */
  ci->books=oggpack_read(opb,8)+1;
  for(i=0;i<ci->books;i++){
    ci->book_param[i]=(static_codebook*)_ogg_calloc(1,sizeof(static_codebook));
    if(vorbis_staticbook_unpack(opb,ci->book_param[i]))goto err_out;
  }

  /* time backend settings; hooks are unused */
  {
    int times=oggpack_read(opb,6)+1;
    for(i=0;i<times;i++){
      int test=oggpack_read(opb,16);
      if(test<0 || test>=VI_TIMEB)goto err_out;
    }
  }

  /* floor backend settings */
  ci->floors=oggpack_read(opb,6)+1;
  for(i=0;i<ci->floors;i++){
    ci->floor_type[i]=oggpack_read(opb,16);
    if(ci->floor_type[i]<0 || ci->floor_type[i]>=VI_FLOORB)goto err_out;
    ci->floor_param[i]=_floor_P[ci->floor_type[i]]->unpack(vi,opb);
    if(!ci->floor_param[i])goto err_out;
  }

  /* residue backend settings */
  ci->residues=oggpack_read(opb,6)+1;
  for(i=0;i<ci->residues;i++){
    ci->residue_type[i]=oggpack_read(opb,16);
    if(ci->residue_type[i]<0 || ci->residue_type[i]>=VI_RESB)goto err_out;
    ci->residue_param[i]=_residue_P[ci->residue_type[i]]->unpack(vi,opb);
    if(!ci->residue_param[i])goto err_out;
  }

  /* map backend settings */
  ci->maps=oggpack_read(opb,6)+1;
  for(i=0;i<ci->maps;i++){
    ci->map_type[i]=oggpack_read(opb,16);
    if(ci->map_type[i]<0 || ci->map_type[i]>=VI_MAPB)goto err_out;
    ci->map_param[i]=_mapping_P[ci->map_type[i]]->unpack(vi,opb);
    if(!ci->map_param[i])goto err_out;
  }

  /* mode settings */
  ci->modes=oggpack_read(opb,6)+1;
  for(i=0;i<ci->modes;i++){
    ci->mode_param[i]=(vorbis_info_mode*)_ogg_calloc(1,sizeof(*ci->mode_param[i]));
    ci->mode_param[i]->blockflag=oggpack_read(opb,1);
    ci->mode_param[i]->windowtype=oggpack_read(opb,16);
    ci->mode_param[i]->transformtype=oggpack_read(opb,16);
    ci->mode_param[i]->mapping=oggpack_read(opb,8);

    if(ci->mode_param[i]->windowtype>=VI_WINDOWB)goto err_out;
    if(ci->mode_param[i]->transformtype>=VI_WINDOWB)goto err_out;
    if(ci->mode_param[i]->mapping>=ci->maps)goto err_out;
  }

  if(oggpack_read(opb,1)!=1)goto err_out; /* top level EOP check */

  return(0);
 err_out:
  vorbis_info_clear(vi);
  return(OV_EBADHEADER);
}

/* writes one setup body: 2 books, 1 time, 2 floors (types 1 and 0), 1 residue, 1 map, 2 modes; `variant` injects one fault */
static void build(oggpack_buffer *w, int variant){
  oggpack_writeinit(w);
  oggpack_write(w,2-1,8);
  oggpack_write(w,2,16); oggpack_write(w,16,24);           /* book 0: dim 2, 16 entries */
  oggpack_write(w,4,16); oggpack_write(w,256,24);          /* book 1 */
  oggpack_write(w,1-1,6); oggpack_write(w,variant==1?1:0,16);   /* one time placeholder (type 1 is out of range) */
  oggpack_write(w,2-1,6);
  oggpack_write(w,variant==2?2:1,16); oggpack_write(w,0x11,8);  /* floor 0: type 1 (type 2 is out of range) */
  oggpack_write(w,0,16); oggpack_write(w,variant==3?0xee:0x22,8);   /* floor 1: type 0 (0xee makes the unpack fail) */
  oggpack_write(w,1-1,6); oggpack_write(w,2,16); oggpack_write(w,0x33,8);     /* residue type 2 */
  oggpack_write(w,1-1,6); oggpack_write(w,0,16); oggpack_write(w,0x44,8);     /* map type 0 */
  oggpack_write(w,2-1,6);
  oggpack_write(w,0,1); oggpack_write(w,0,16); oggpack_write(w,0,16); oggpack_write(w,0,8);
  oggpack_write(w,1,1); oggpack_write(w,0,16); oggpack_write(w,0,16); oggpack_write(w,variant==4?1:0,8);  /* mapping 1 >= maps */
  oggpack_write(w,variant==5?0:1,1);                       /* framing bit */
}

static int fail(const char* what) { std::printf("%s\n00c8e2e0_TEST FAIL\n", what); return 1; }

int main()
{
    oggpack_buffer w, r; vorbis_info vi; codec_setup_info ci; int v;
    memset(&vi, 0, sizeof vi);
    if (_vorbis_unpack_books(&vi, &w) != OV_EFAULT) return fail("no codec setup");

    memset(&ci, 0, sizeof ci); vi.codec_setup = &ci;
    build(&w, 0); oggpack_readinit(&r, w.buffer, (int)w.endbyte + 1);
    if (_vorbis_unpack_books(&vi, &r) != 0) return fail("good header");
    if (ci.books != 2 || ci.book_param[0]->dim != 2 || ci.book_param[0]->entries != 16 || ci.book_param[1]->entries != 256) return fail("books");
    if (ci.floors != 2 || ci.floor_type[0] != 1 || ci.floor_type[1] != 0 || *(int*)ci.floor_param[0] != 0x11 || *(int*)ci.floor_param[1] != 0x22) return fail("floors");
    if (ci.residues != 1 || ci.residue_type[0] != 2 || *(int*)ci.residue_param[0] != 0x33) return fail("residues");
    if (ci.maps != 1 || ci.map_type[0] != 0 || *(int*)ci.map_param[0] != 0x44) return fail("maps");
    if (ci.modes != 2 || ci.mode_param[0]->blockflag != 0 || ci.mode_param[1]->blockflag != 1 || ci.mode_param[1]->mapping != 0) return fail("modes");
    if (gBooks != 2 || gFloors != 2 || gRes != 1 || gMaps != 1 || gClears != 0) return fail("backend calls");
    if (r.endbyte * 8 + r.endbit != w.endbyte * 8 + w.endbit) return fail("bit count");

    for (v = 1; v <= 5; v++) {
        memset(&ci, 0, sizeof ci);
        build(&w, v); oggpack_readinit(&r, w.buffer, (int)w.endbyte + 1);
        if (_vorbis_unpack_books(&vi, &r) != OV_EBADHEADER) return fail("bad header variant");
        if (gClears != v) return fail("vorbis_info_clear on error");
    }
    std::printf("00c8e2e0_TEST PASS\n");
    return 0;
}