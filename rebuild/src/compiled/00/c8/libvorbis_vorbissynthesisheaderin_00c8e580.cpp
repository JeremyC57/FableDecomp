// libvorbis::vorbis_synthesis_headerin(vorbis_info* vi, vorbis_comment* vc, ogg_packet* op) -- libvorbis info.c: classify one of the three Vorbis header packets (type byte + "vorbis") and unpack it into the info (1), comment (3) or setup/codebook (5) structures; OV_ENOTVORBIS when the signature is wrong, OV_EBADHEADER for an out-of-order or malformed header
// Leaf: @vorbis_synthesis_headerin@12
// Flags: /Gr /GS
// Grades: 1 (RELOCATION_MATCH; the four info.c statics as same-TU noinline bodies, the two callee-pop ones `__stdcall`)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C8E580, 347 bytes, LTCG __fastcall (vi in ecx, vc in edx, op on the stack, `ret 4`), /GS.
// Library: libpk's statically linked Xiph libvorbis ("Xiph.Org libVorbis I 20030909", 1.0.1 info.c). No PDB/RTTI: the
//   libvorbis API name (the catalog label "CreatureHitNotification_Construct" is propagated). Evidence: the body is info.c's
//   switch verbatim (packtype 1/3/5, OV_ENOTVORBIS -132 / OV_EBADHEADER -133, memcmp(buffer, "vorbis", 6) at 0x012BF238),
//   and its four static callees sit right above it in info.c order (_v_readstring 0x00C8DBB0, _vorbis_unpack_info
//   0x00C8E120, _vorbis_unpack_comment 0x00C8E200, _vorbis_unpack_books 0x00C8E2E0).
// Callees: oggpack_readinit 0x00C91360 / oggpack_read 0x00C915A0 (libogg bitwise.c, landed `@..@` fastcall), the four
//   info.c statics (private register conventions: same-TU bodies), __security_check_cookie 0x00BFE9F9.
// Shape: the statics are TU-local `static __declspec(noinline)` bodies (info.c 1.0.1) so VC7.1 gives them retail's private
//   register conventions; their own callees (vorbis_info_clear 0x00C8DFB0, vorbis_comment_clear 0x00C8DEF0,
//   vorbis_staticbook_unpack 0x00C95EE0, the _floor_P / _residue_P / _mapping_P tables, MSVCR71 calloc) are declarations.
extern "C" {
typedef struct {
    long           endbyte;
    int            endbit;
    unsigned char* buffer;
    unsigned char* ptr;
    long           storage;
} oggpack_buffer;
typedef struct {
    unsigned char* packet;
    long           bytes;
    long           b_o_s;
    long           e_o_s;
    __int64        granulepos;
    __int64        packetno;
} ogg_packet;
typedef struct vorbis_info {
    int   version;
    int   channels;
    long  rate;
    long  bitrate_upper;
    long  bitrate_nominal;
    long  bitrate_lower;
    long  bitrate_window;
    void* codec_setup;
} vorbis_info;
typedef struct vorbis_comment {
    char** user_comments;
    int*   comment_lengths;
    int    comments;
    char*  vendor;
} vorbis_comment;
typedef struct static_codebook static_codebook;
typedef void vorbis_info_floor;
typedef void vorbis_info_residue;
typedef void vorbis_info_mapping;
typedef struct {
    int blockflag;
    int windowtype;
    int transformtype;
    int mapping;
} vorbis_info_mode;
typedef struct codec_setup_info {
    long                 blocksizes[2];
    int                  modes;
    int                  maps;
    int                  floors;
    int                  residues;
    int                  books;
    int                  psys;
    vorbis_info_mode*    mode_param[64];
    int                  map_type[64];
    vorbis_info_mapping* map_param[64];
    int                  floor_type[64];
    vorbis_info_floor*   floor_param[64];
    int                  residue_type[64];
    vorbis_info_residue* residue_param[64];
    static_codebook*     book_param[256];
} codec_setup_info;
typedef struct {
    void                (__fastcall* pack)(vorbis_info_floor*, oggpack_buffer*);
    vorbis_info_floor*  (__fastcall* unpack)(vorbis_info*, oggpack_buffer*);
} vorbis_func_floor;
typedef struct {
    void                (__fastcall* pack)(vorbis_info_residue*, oggpack_buffer*);
    vorbis_info_residue* (__fastcall* unpack)(vorbis_info*, oggpack_buffer*);
} vorbis_func_residue;
typedef struct {
    void                (__fastcall* pack)(vorbis_info*, vorbis_info_mapping*, oggpack_buffer*);
    vorbis_info_mapping* (__fastcall* unpack)(vorbis_info*, oggpack_buffer*);
} vorbis_func_mapping;

#define VI_TIMEB 1
#define VI_FLOORB 2
#define VI_RESB 3
#define VI_MAPB 1
#define VI_WINDOWB 1
#define OV_EFAULT -129
#define OV_ENOTVORBIS -132
#define OV_EBADHEADER -133
#define OV_EVERSION -134

__declspec(dllimport) void* __cdecl calloc(unsigned int num, unsigned int size);   // IAT 0x01440058
#define _ogg_calloc calloc
int __cdecl memcmp(const void* a, const void* b, unsigned int n);
#pragma intrinsic(memcmp, memset)
void* __cdecl memset(void* d, int c, unsigned int n);

void __fastcall oggpack_readinit(oggpack_buffer* b, unsigned char* buf, int bytes);   // 0x00C91360
long __fastcall oggpack_read(oggpack_buffer* b, int bits);                           // 0x00C915A0
void __fastcall vorbis_info_clear(vorbis_info* vi);                                  // 0x00C8DFB0
void __fastcall vorbis_comment_clear(vorbis_comment* vc);                            // 0x00C8DEF0
int __fastcall vorbis_staticbook_unpack(oggpack_buffer* opb, static_codebook* s);    // 0x00C95EE0
extern vorbis_func_floor*   _floor_P[];                                              // 0x013A3DC8
extern vorbis_func_residue* _residue_P[];                                            // 0x013A3DD0
extern vorbis_func_mapping* _mapping_P[];                                            // 0x013A3DDC

static __declspec(noinline) void _v_readstring(oggpack_buffer* o, char* buf, int bytes)       // 0x00C8DBB0
{
    while (bytes--) {
        *buf++ = (char)oggpack_read(o, 8);
    }
}

static __declspec(noinline) int _vorbis_unpack_info(vorbis_info* vi, oggpack_buffer* opb)     // 0x00C8E120
{
    codec_setup_info* ci = (codec_setup_info*)vi->codec_setup;
    if (!ci) return (OV_EFAULT);

    vi->version = oggpack_read(opb, 32);
    if (vi->version != 0) return (OV_EVERSION);

    vi->channels = oggpack_read(opb, 8);
    vi->rate = oggpack_read(opb, 32);

    vi->bitrate_upper = oggpack_read(opb, 32);
    vi->bitrate_nominal = oggpack_read(opb, 32);
    vi->bitrate_lower = oggpack_read(opb, 32);

    ci->blocksizes[0] = 1 << oggpack_read(opb, 4);
    ci->blocksizes[1] = 1 << oggpack_read(opb, 4);

    if (vi->rate < 1) goto err_out;
    if (vi->channels < 1) goto err_out;
    if (ci->blocksizes[0] < 8) goto err_out;
    if (ci->blocksizes[1] < ci->blocksizes[0]) goto err_out;

    if (oggpack_read(opb, 1) != 1) goto err_out; /* EOP check */

    return (0);
 err_out:
    vorbis_info_clear(vi);
    return (OV_EBADHEADER);
}

static __declspec(noinline) int __stdcall _vorbis_unpack_comment(vorbis_comment* vc, oggpack_buffer* opb)   // 0x00C8E200
{
    int i;
    int vendorlen = oggpack_read(opb, 32);
    if (vendorlen < 0) goto err_out;
    vc->vendor = (char*)_ogg_calloc(vendorlen + 1, 1);
    _v_readstring(opb, vc->vendor, vendorlen);
    vc->comments = oggpack_read(opb, 32);
    if (vc->comments < 0) goto err_out;
    vc->user_comments = (char**)_ogg_calloc(vc->comments + 1, sizeof(*vc->user_comments));
    vc->comment_lengths = (int*)_ogg_calloc(vc->comments + 1, sizeof(*vc->comment_lengths));

    for (i = 0; i < vc->comments; i++) {
        int len = oggpack_read(opb, 32);
        if (len < 0) goto err_out;
        vc->comment_lengths[i] = len;
        vc->user_comments[i] = (char*)_ogg_calloc(len + 1, 1);
        _v_readstring(opb, vc->user_comments[i], len);
    }
    if (oggpack_read(opb, 1) != 1) goto err_out; /* EOP check */

    return (0);
 err_out:
    vorbis_comment_clear(vc);
    return (OV_EBADHEADER);
}

static __declspec(noinline) int __stdcall _vorbis_unpack_books(vorbis_info* vi, oggpack_buffer* opb)   // 0x00C8E2E0
{
    codec_setup_info* ci = (codec_setup_info*)vi->codec_setup;
    int i;
    if (!ci) return (OV_EFAULT);

    /* codebooks */
    ci->books = oggpack_read(opb, 8) + 1;
    for (i = 0; i < ci->books; i++) {
        ci->book_param[i] = (static_codebook*)_ogg_calloc(1, 0x34);
        if (vorbis_staticbook_unpack(opb, ci->book_param[i])) goto err_out;
    }

    /* time backend settings; hooks are unused */
    {
        int times = oggpack_read(opb, 6) + 1;
        for (i = 0; i < times; i++) {
            int test = oggpack_read(opb, 16);
            if (test < 0 || test >= VI_TIMEB) goto err_out;
        }
    }

    /* floor backend settings */
    ci->floors = oggpack_read(opb, 6) + 1;
    for (i = 0; i < ci->floors; i++) {
        ci->floor_type[i] = oggpack_read(opb, 16);
        if (ci->floor_type[i] < 0 || ci->floor_type[i] >= VI_FLOORB) goto err_out;
        ci->floor_param[i] = _floor_P[ci->floor_type[i]]->unpack(vi, opb);
        if (!ci->floor_param[i]) goto err_out;
    }

    /* residue backend settings */
    ci->residues = oggpack_read(opb, 6) + 1;
    for (i = 0; i < ci->residues; i++) {
        ci->residue_type[i] = oggpack_read(opb, 16);
        if (ci->residue_type[i] < 0 || ci->residue_type[i] >= VI_RESB) goto err_out;
        ci->residue_param[i] = _residue_P[ci->residue_type[i]]->unpack(vi, opb);
        if (!ci->residue_param[i]) goto err_out;
    }

    /* map backend settings */
    ci->maps = oggpack_read(opb, 6) + 1;
    for (i = 0; i < ci->maps; i++) {
        ci->map_type[i] = oggpack_read(opb, 16);
        if (ci->map_type[i] < 0 || ci->map_type[i] >= VI_MAPB) goto err_out;
        ci->map_param[i] = _mapping_P[ci->map_type[i]]->unpack(vi, opb);
        if (!ci->map_param[i]) goto err_out;
    }

    /* mode settings */
    ci->modes = oggpack_read(opb, 6) + 1;
    for (i = 0; i < ci->modes; i++) {
        ci->mode_param[i] = (vorbis_info_mode*)_ogg_calloc(1, sizeof(*ci->mode_param[i]));
        ci->mode_param[i]->blockflag = oggpack_read(opb, 1);
        ci->mode_param[i]->windowtype = oggpack_read(opb, 16);
        ci->mode_param[i]->transformtype = oggpack_read(opb, 16);
        ci->mode_param[i]->mapping = oggpack_read(opb, 8);

        if (ci->mode_param[i]->windowtype >= VI_WINDOWB) goto err_out;
        if (ci->mode_param[i]->transformtype >= VI_WINDOWB) goto err_out;
        if (ci->mode_param[i]->mapping >= ci->maps) goto err_out;
    }

    if (oggpack_read(opb, 1) != 1) goto err_out; /* top level EOP check */

    return (0);
 err_out:
    vorbis_info_clear(vi);
    return (OV_EBADHEADER);
}

int __fastcall vorbis_synthesis_headerin(vorbis_info* vi, vorbis_comment* vc, ogg_packet* op)
{
    oggpack_buffer opb;

    if (op) {
        oggpack_readinit(&opb, op->packet, op->bytes);

        /* Which of the three types of header is this? */
        /* Also verify header-ness, vorbis */
        {
            char buffer[6];
            int packtype = oggpack_read(&opb, 8);
            memset(buffer, 0, 6);
            _v_readstring(&opb, buffer, 6);
            if (memcmp(buffer, "vorbis", 6)) {
                /* not a vorbis header */
                return (OV_ENOTVORBIS);
            }
            switch (packtype) {
            case 0x01: /* least significant *bit* is read first */
                if (!op->b_o_s) {
                    /* Not the initial packet */
                    return (OV_EBADHEADER);
                }
                if (vi->rate != 0) {
                    /* previously initialized info header */
                    return (OV_EBADHEADER);
                }

                return (_vorbis_unpack_info(vi, &opb));

            case 0x03: /* least significant *bit* is read first */
                if (vi->rate == 0) {
                    /* um... we didn't get the initial header */
                    return (OV_EBADHEADER);
                }

                return (_vorbis_unpack_comment(vc, &opb));

            case 0x05: /* least significant *bit* is read first */
                if (vi->rate == 0 || vc->vendor == 0) {
                    /* um... we didn't get the initial header or comments yet */
                    return (OV_EBADHEADER);
                }

                return (_vorbis_unpack_books(vi, &opb));

            default:
                /* Not a valid vorbis header type */
                return (OV_EBADHEADER);
                break;
            }
        }
    }
    return (OV_EBADHEADER);
}
}