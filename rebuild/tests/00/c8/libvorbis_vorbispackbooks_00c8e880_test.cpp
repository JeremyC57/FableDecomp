// Behaviour test for libvorbis info.c _vorbis_pack_books (retail 0x00C8E880): a copy of the packer over a model codec setup,
// with libogg's oggpack_write and a bit reader to check the stream -- type 5, "vorbis", books-1 (8 bits) + one static book pack
// per book, the 6/16-bit time placeholders, floors / residues / maps / modes with their 16-bit types and backend packs, the
// mode fields (1/16/16/8 bits), the framing bit; OV_EFAULT without a codec setup and -1 when a floor type cannot pack.
#include <cstdio>
#include <cstring>
#include <cstdlib>
typedef struct { long endbyte; int endbit; unsigned char *buffer; unsigned char *ptr; long storage; } oggpack_buffer;
typedef struct vorbis_info { int version; int channels; long rate; long bitrate_upper; long bitrate_nominal; long bitrate_lower; long bitrate_window; void *codec_setup; } vorbis_info;
typedef struct { long dim; long entries; } static_codebook;   /* only the fields this test needs */
typedef void vorbis_info_floor; typedef void vorbis_info_residue; typedef void vorbis_info_mapping;
typedef struct { int blockflag; int windowtype; int transformtype; int mapping; } vorbis_info_mode;
typedef struct { void (*pack)(vorbis_info_floor *,oggpack_buffer *); } vorbis_func_floor;
typedef struct { void (*pack)(vorbis_info_residue *,oggpack_buffer *); } vorbis_func_residue;
typedef struct { void (*pack)(vorbis_info *,vorbis_info_mapping *,oggpack_buffer *); } vorbis_func_mapping;
typedef struct codec_setup_info {
  long blocksizes[2]; int modes; int maps; int floors; int residues; int books; int psys;
  vorbis_info_mode *mode_param[64]; int map_type[64]; vorbis_info_mapping *map_param[64];
  int floor_type[64]; vorbis_info_floor *floor_param[64]; int residue_type[64]; vorbis_info_residue *residue_param[64];
  static_codebook *book_param[256];
} codec_setup_info;
#define OV_EFAULT -129

/* libogg bitwise.c writer (LSb first) */
static const unsigned long mask[]={0x00000000,0x00000001,0x00000003,0x00000007,0x0000000f,0x0000001f,0x0000003f,0x0000007f,0x000000ff,0x000001ff,0x000003ff,0x000007ff,0x00000fff,0x00001fff,0x00003fff,0x00007fff,0x0000ffff,0x0001ffff,0x0003ffff,0x0007ffff,0x000fffff,0x001fffff,0x003fffff,0x007fffff,0x00ffffff,0x01ffffff,0x03ffffff,0x07ffffff,0x0fffffff,0x1fffffff,0x3fffffff,0x7fffffff,0xffffffff};
static void oggpack_write(oggpack_buffer *b,unsigned long value,int bits){
  if(b->endbyte+4>=b->storage){ b->buffer=(unsigned char*)realloc(b->buffer,b->storage+256); b->storage+=256; b->ptr=b->buffer+b->endbyte; }
  value&=mask[bits]; bits+=b->endbit;
  b->ptr[0]|=value<<b->endbit;
  if(bits>=8){ b->ptr[1]=(unsigned char)(value>>(8-b->endbit));
    if(bits>=16){ b->ptr[2]=(unsigned char)(value>>(16-b->endbit));
      if(bits>=24){ b->ptr[3]=(unsigned char)(value>>(24-b->endbit));
        if(bits>=32){ if(b->endbit) b->ptr[4]=(unsigned char)(value>>(32-b->endbit)); else b->ptr[4]=0; } } } }
  b->endbyte+=bits/8; b->ptr+=bits/8; b->endbit=bits&7;
}
static int gBookPacks = 0, gFloorPacks = 0, gResPacks = 0, gMapPacks = 0; static vorbis_info *gMapVi = 0;
static int vorbis_staticbook_pack(const static_codebook *c,oggpack_buffer *opb){ gBookPacks++; oggpack_write(opb,c->dim,16); oggpack_write(opb,c->entries,24); return 0; }
static void floor_pack(vorbis_info_floor *i,oggpack_buffer *opb){ gFloorPacks++; oggpack_write(opb,*(int*)i,8); }
static void res_pack(vorbis_info_residue *i,oggpack_buffer *opb){ gResPacks++; oggpack_write(opb,*(int*)i,8); }
static void map_pack(vorbis_info *vi,vorbis_info_mapping *i,oggpack_buffer *opb){ gMapPacks++; gMapVi=vi; oggpack_write(opb,*(int*)i,8); }
static vorbis_func_floor floorA = { floor_pack }, floorNone = { 0 };
static vorbis_func_residue resA = { res_pack };
static vorbis_func_mapping mapA = { map_pack };
static vorbis_func_floor*   _floor_P[2]   = { &floorA, &floorNone };
static vorbis_func_residue* _residue_P[3] = { &resA, &resA, &resA };
static vorbis_func_mapping* _mapping_P[1] = { &mapA };

static void _v_writestring(oggpack_buffer *o, const char *s, int bytes){ while(bytes--){ oggpack_write(o,*s++,8); } }

static int _vorbis_pack_books(oggpack_buffer *opb,vorbis_info *vi){
  codec_setup_info     *ci=(codec_setup_info *)vi->codec_setup;
  int i;
  if(!ci)return(OV_EFAULT);
  oggpack_write(opb,0x05,8);
  _v_writestring(opb,"vorbis", 6);
  oggpack_write(opb,ci->books-1,8);
  for(i=0;i<ci->books;i++)
    if(vorbis_staticbook_pack(ci->book_param[i],opb))goto err_out;
  oggpack_write(opb,0,6);
  oggpack_write(opb,0,16);
  oggpack_write(opb,ci->floors-1,6);
  for(i=0;i<ci->floors;i++){
    oggpack_write(opb,ci->floor_type[i],16);
    if(_floor_P[ci->floor_type[i]]->pack)
      _floor_P[ci->floor_type[i]]->pack(ci->floor_param[i],opb);
    else
      goto err_out;
  }
  oggpack_write(opb,ci->residues-1,6);
  for(i=0;i<ci->residues;i++){
    oggpack_write(opb,ci->residue_type[i],16);
    _residue_P[ci->residue_type[i]]->pack(ci->residue_param[i],opb);
  }
  oggpack_write(opb,ci->maps-1,6);
  for(i=0;i<ci->maps;i++){
    oggpack_write(opb,ci->map_type[i],16);
    _mapping_P[ci->map_type[i]]->pack(vi,ci->map_param[i],opb);
  }
  oggpack_write(opb,ci->modes-1,6);
  for(i=0;i<ci->modes;i++){
    oggpack_write(opb,ci->mode_param[i]->blockflag,1);
    oggpack_write(opb,ci->mode_param[i]->windowtype,16);
    oggpack_write(opb,ci->mode_param[i]->transformtype,16);
    oggpack_write(opb,ci->mode_param[i]->mapping,8);
  }
  oggpack_write(opb,1,1);
  return(0);
err_out:
  return(-1);
}

/* bit reader for the check */
static const unsigned char *rbuf; static long rpos;
static unsigned long rd(int bits){ unsigned long v=0; int k; for(k=0;k<bits;k++){ v|=(unsigned long)((rbuf[rpos>>3]>>(rpos&7))&1)<<k; rpos++; } return v; }
static int fail(const char* w){ std::printf("%s\n00c8e880_TEST FAIL\n", w); return 1; }

int main()
{
    codec_setup_info ci; vorbis_info vi; oggpack_buffer opb;
    static_codebook b0 = { 2, 16 }, b1 = { 4, 256 };
    int fl0 = 0x11, fl1 = 0x22, rs0 = 0x33, mp0 = 0x44;
    vorbis_info_mode m0 = { 0, 0, 0, 0 }, m1 = { 1, 0, 0, 0 };
    std::memset(&ci, 0, sizeof(ci)); std::memset(&vi, 0, sizeof(vi)); std::memset(&opb, 0, sizeof(opb));
    vi.codec_setup = 0;
    if (_vorbis_pack_books(&opb, &vi) != OV_EFAULT) return fail("efault");
    vi.codec_setup = &ci;
    ci.books = 2; ci.book_param[0] = &b0; ci.book_param[1] = &b1;
    ci.floors = 2; ci.floor_type[0] = 0; ci.floor_param[0] = &fl0; ci.floor_type[1] = 0; ci.floor_param[1] = &fl1;
    ci.residues = 1; ci.residue_type[0] = 2; ci.residue_param[0] = &rs0;
    ci.maps = 1; ci.map_type[0] = 0; ci.map_param[0] = &mp0;
    ci.modes = 2; ci.mode_param[0] = &m0; ci.mode_param[1] = &m1;
    opb.buffer = (unsigned char*)calloc(1, 256); opb.ptr = opb.buffer; opb.storage = 256;
    if (_vorbis_pack_books(&opb, &vi) != 0) return fail("pack");
    if (gBookPacks != 2 || gFloorPacks != 2 || gResPacks != 1 || gMapPacks != 1 || gMapVi != &vi) return fail("callbacks");
    rbuf = opb.buffer; rpos = 0;
    if (rd(8) != 5) return fail("type");
    { const char *s = "vorbis"; int k; for (k = 0; k < 6; k++) if (rd(8) != (unsigned char)s[k]) return fail("vorbis"); }
    if (rd(8) != 1) return fail("books-1");
    if (rd(16) != 2 || rd(24) != 16 || rd(16) != 4 || rd(24) != 256) return fail("book bodies");
    if (rd(6) != 0 || rd(16) != 0) return fail("times");
    if (rd(6) != 1) return fail("floors-1");
    if (rd(16) != 0 || rd(8) != 0x11 || rd(16) != 0 || rd(8) != 0x22) return fail("floors");
    if (rd(6) != 0 || rd(16) != 2 || rd(8) != 0x33) return fail("residues");
    if (rd(6) != 0 || rd(16) != 0 || rd(8) != 0x44) return fail("maps");
    if (rd(6) != 1) return fail("modes-1");
    if (rd(1) != 0 || rd(16) != 0 || rd(16) != 0 || rd(8) != 0) return fail("mode 0");
    if (rd(1) != 1 || rd(16) != 0 || rd(16) != 0 || rd(8) != 0) return fail("mode 1");
    if (rd(1) != 1) return fail("framing");
    if (rpos != opb.endbyte * 8 + opb.endbit) return fail("length");
    /* a floor type without a pack routine fails the header */
    ci.floor_type[1] = 1;
    std::memset(opb.buffer, 0, 256); opb.ptr = opb.buffer; opb.endbyte = 0; opb.endbit = 0;
    if (_vorbis_pack_books(&opb, &vi) != -1) return fail("floor without pack");
    free(opb.buffer);
    std::printf("00c8e880_TEST PASS\n");
    return 0;
}