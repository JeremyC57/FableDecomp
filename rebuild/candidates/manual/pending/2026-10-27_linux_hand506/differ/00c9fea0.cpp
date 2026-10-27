// libvorbis::res0_pack(vorbis_info_residue* vr, oggpack_buffer* opb) -- libvorbis res0.c: write a residue 0/1/2 setup header: begin and end (24 bits), grouping-1 (24), partitions-1 (6), groupbook (8); per partition its second-stage cascade bitmap (3 low bits + flag + 5 high bits when ilog > 3, else 4 bits) counting the books with icount; then each book number in 8 bits
// Grade: DIFFER(206v215) -- LTCG: retail loads secondstages[j] into edx BEFORE the ilog call and reuses it after (mov edx,[ebx] ; mov ecx,edx ; call ilog ; ... push 3/4 ; call oggpack_write with edx not reloaded): the link-time register allocator knew ilog keeps edx; VC7.1 alone reloads ([ebx] after the call) and drops retail's loop-alignment padding
// Leaf: @res0_pack@8
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C9FEA0, 215 bytes. libpk's statically linked Xiph libvorbis 20030909 (lib/res0.c), vr in ecx,
// opb in edx (LTCG __fastcall), plain `ret`.
// Identity: res0.c's pack -- the 24/24/24/6/8 header widths, the ilog(secondstages[j]) > 3 split, icount() accumulation and the
// booklist (+0x114) loop are exactly res0_pack; the ilog 0x00C9FE70 / icount 0x00C9FE80 statics it calls are landed as res0.c's.
// Callees: oggpack_write @ 0x00C90FC0 (__fastcall), ilog @ 0x00C9FE70, icount @ 0x00C9FE80.
typedef struct {
    long endbyte;
    int endbit;
    unsigned char* buffer;
    unsigned char* ptr;
    long storage;
} oggpack_buffer;

typedef void vorbis_info_residue;

typedef struct vorbis_info_residue0 {
    /* block-partitioned VQ coded straight residue */
    long begin;
    long end;

    /* first stage (lossless partitioning) */
    int grouping;                       /* group n vectors per partition */
    int partitions;                     /* possible codebooks for a partition */
    int groupbook;                      /* huffbook for partitioning */
    int secondstages[64];               /* expanded out to pointers in lookup */
    int booklist[256];                  /* list of second stage books */

    float classmetric1[64];
    float classmetric2[64];
} vorbis_info_residue0;

extern "C" void __fastcall oggpack_write(oggpack_buffer* b, unsigned long value, int bits);   // 0x00C90FC0
extern "C" int __fastcall ilog(unsigned int v);     // 0x00C9FE70
extern "C" int __fastcall icount(unsigned int v);   // 0x00C9FE80

extern "C" void __fastcall res0_pack(vorbis_info_residue* vr, oggpack_buffer* opb)
{
    vorbis_info_residue0* info = (vorbis_info_residue0*)vr;
    int j, acc = 0;
    oggpack_write(opb, info->begin, 24);
    oggpack_write(opb, info->end, 24);

    oggpack_write(opb, info->grouping - 1, 24);   /* residue vectors to group and
                                                     code with a partitioned book */
    oggpack_write(opb, info->partitions - 1, 6);  /* possible partition choices */
    oggpack_write(opb, info->groupbook, 8);       /* group huffman book */

    /* secondstages is a bitmask; as encoding progresses pass by pass, a
       bitmask of one indicates this partition class has bits to write
       this pass */
    for (j = 0; j < info->partitions; j++) {
        if (ilog(info->secondstages[j]) > 3) {
            /* yes, this is a minor hack due to not thinking ahead */
            oggpack_write(opb, info->secondstages[j], 3);
            oggpack_write(opb, 1, 1);
            oggpack_write(opb, info->secondstages[j] >> 3, 5);
        } else
            oggpack_write(opb, info->secondstages[j], 4); /* trailing zero */
        acc += icount(info->secondstages[j]);
    }
    for (j = 0; j < acc; j++)
        oggpack_write(opb, info->booklist[j], 8);
}
