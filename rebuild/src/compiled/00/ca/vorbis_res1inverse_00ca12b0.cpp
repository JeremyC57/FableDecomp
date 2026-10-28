// vorbis::res1_inverse(...) -- Compact active residue channels and decode their partitions.
// Leaf: @res1_inverse@20
// Identity: Xiph Vorbis1.0.1 res0.c original res1_inverse packs active channels then _01inverse with vorbis_book_decodev_add. Own helper00ca0df0 matches setup/partition decode algorithm and returns20 stack bytes; callback00c965c0 receives codebook/float in ecx/edx with remaining two stack args. Own wrapper uses normal fastcall. No owner layout accessed.
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#endif
struct vorbis_block;typedef void vorbis_look_residue;struct codebook;struct oggpack_buffer;
extern "C" long __fastcall vorbis_book_decodev_add(codebook*,float*,oggpack_buffer*,int);
extern "C" int __stdcall _01inverse(vorbis_block*,vorbis_look_residue*,float**,int,long(__fastcall*)(codebook*,float*,oggpack_buffer*,int));
extern "C" int __fastcall res1_inverse(vorbis_block* vb,vorbis_look_residue* vl,float** in,int* nonzero,int ch){int i,used=0;for(i=0;i<ch;i++)if(nonzero[i])in[used++]=in[i];if(used)return _01inverse(vb,vl,in,used,vorbis_book_decodev_add);else return 0;}