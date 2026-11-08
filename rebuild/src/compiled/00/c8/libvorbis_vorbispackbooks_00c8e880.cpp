// libvorbis::_vorbis_pack_books(oggpack_buffer* opb, vorbis_info* vi) -- info.c: write the third (setup) header packet body -- type 5 + "vorbis", every static codebook through vorbis_staticbook_pack, the empty time-domain placeholders, then the floors (type + the backend's pack, failing when a floor type has none), residues, mappings and modes (blockflag, windowtype, transformtype, mapping) with the framing bit; OV_EFAULT without a codec setup, -1 on a pack failure
// Leaf: __vorbis_pack_books@8
// Flags: /TC /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C8E880, 483 bytes. libvorbis info.c `static int _vorbis_pack_books(...)` ("libVorbis I
//   20030909" = the 1.0.1 text), kept out of line with a private register convention (opb in eax, vi on the stack, `ret 4`).
//   Caller: vorbis_analysis_headerout (info.c; the third packet). Callees: oggpack_write 0x00C90FC0 (`@oggpack_write@12`),
//   _v_writestring 0x00C8DB80 (info.c static: o in ebx, s in ecx, bytes in eax; same-TU body as in the landed
//   _vorbis_pack_info 0x00C8E6E0), vorbis_staticbook_pack 0x00C95CA0 (`@vorbis_staticbook_pack@8`), and the backend tables
//   _floor_P 0x013A3DC8 / _residue_P 0x013A3DD0 / _mapping_P 0x013A3DDC (bound by the landed vorbis_info_clear 0x00C8DFB0) through
//   their `pack` slots (floor: null-tested `call eax`; residue / mapping: `call [eax]`, mapping with vi in ecx).
// Identity (two sources): (1) the body is info.c _vorbis_pack_books verbatim -- 0x05 then "vorbis" (0x012BF238, 6), `books-1` in 8
//   bits over book_param (ci+0x720), the 6/16-bit time placeholders, `floors-1` / `residues-1` / `maps-1` / `modes-1` in 6 bits
//   with the 16-bit type fields (floor_type ci+0x320, floor_param +0x420, residue_type +0x520, residue_param +0x620, map_type
//   +0x120, map_param +0x220, mode_param +0x20 with the 1/16/16/8-bit mode fields), the framing 1 and the -1 / OV_EFAULT (-129)
//   exits; (2) info.c file order: _vorbis_pack_info 0x00C8E6E0, _vorbis_pack_comment 0x00C8E790, _vorbis_pack_books 0x00C8E880,
//   vorbis_commentheader_out 0x00C8EA70 (all landed under these names). The catalog name "OggVorbis_EncodeSetupPacket" is a
//   descriptive guess.
// Shape: the landed _vorbis_pack_info / _vorbis_pack_comment TUs: info.c statics as `static __declspec(noinline)` bodies with a
//   same-TU caller, so VC7.1 /O2 assigns retail's private conventions itself (no /GL). Compiled as C (/TC /Gr, like the other
//   landed rows that include it) so the shared engine/Vorbis.h supplies the Xiph layouts (codec_setup_info, static_codebook,
//   the vorbis_func_* backend tables, whose residue slot is named `class`).
// Original libvorbis info.c; BSD-style terms in reference COPYING.
#include "engine/Vorbis.h"
#define OV_EFAULT -129

void oggpack_write(oggpack_buffer* b, unsigned long value, int bits);          // 0x00C90FC0
int vorbis_staticbook_pack(const static_codebook* c, oggpack_buffer* opb);     // 0x00C95CA0
extern vorbis_func_floor*   _floor_P[2];                                                         // 0x013A3DC8
extern vorbis_func_residue* _residue_P[3];                                                       // 0x013A3DD0
extern vorbis_func_mapping* _mapping_P[1];                                                       // 0x013A3DDC

static __declspec(noinline) void _v_writestring(oggpack_buffer* o, const char* s, int bytes)   // 0x00C8DB80
{
    while (bytes--) {
        oggpack_write(o, *s++, 8);
    }
}

static __declspec(noinline) int __stdcall _vorbis_pack_books(oggpack_buffer *opb,vorbis_info *vi){
  codec_setup_info     *ci=vi->codec_setup;
  int i;
  if(!ci)return(OV_EFAULT);

  oggpack_write(opb,0x05,8);
  _v_writestring(opb,"vorbis", 6);

  /* books */
  oggpack_write(opb,ci->books-1,8);
  for(i=0;i<ci->books;i++)
    if(vorbis_staticbook_pack(ci->book_param[i],opb))goto err_out;

  /* times; hook placeholders */
  oggpack_write(opb,0,6);
  oggpack_write(opb,0,16);

  /* floors */
  oggpack_write(opb,ci->floors-1,6);
  for(i=0;i<ci->floors;i++){
    oggpack_write(opb,ci->floor_type[i],16);
    if(_floor_P[ci->floor_type[i]]->pack)
      _floor_P[ci->floor_type[i]]->pack(ci->floor_param[i],opb);
    else
      goto err_out;
  }

  /* residues */
  oggpack_write(opb,ci->residues-1,6);
  for(i=0;i<ci->residues;i++){
    oggpack_write(opb,ci->residue_type[i],16);
    _residue_P[ci->residue_type[i]]->pack(ci->residue_param[i],opb);
  }

  /* maps */
  oggpack_write(opb,ci->maps-1,6);
  for(i=0;i<ci->maps;i++){
    oggpack_write(opb,ci->map_type[i],16);
    _mapping_P[ci->map_type[i]]->pack(vi,ci->map_param[i],opb);
  }

  /* modes */
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

// Same-TU caller (the _vorbis_pack_books call of info.c vorbis_analysis_headerout), present so the static gets retail's
// private convention. Not graded.
int __cdecl _vorbis_pack_books_caller(oggpack_buffer* opb, vorbis_info* vi) { return _vorbis_pack_books(opb, vi); }