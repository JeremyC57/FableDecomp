// libogg::ogg_page_checksum_set(ogg_page* og) -- libogg framing: zero the CRC field (header bytes 22..25), run the table CRC-32 over the header then the body, and store the result little-endian back into bytes 22..25
// Leaf: @ogg_page_checksum_set@4
// Module (sweep2, 2026-10-25): owner decision 1c unifies the statically linked Xiph code on libogg / libvorbis (vorbisfile.c
//   ov_* with libvorbis); was module _global. The extern "C" __fastcall symbol is unchanged.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8CF60, length 162 bytes.
// Library: libpk's statically linked Xiph libogg (framing.c, libVorbis I 20030909 era). No PDB/RTTI: libogg API name.
// Convention: LTCG __fastcall (og in ecx). No callees. Data: the static `crc_lookup[256]` table @ 0x0139D9B0
// (polynomial 0x04C11DB7, unreflected).
// Shape: the libogg body verbatim; every `og->header[n] = ...` byte store re-reads og->header (a char store may alias
// the pointer), the zero register is the `i` counter reset (`xor edx,edx`) shared with the null test.
typedef unsigned int ogg_uint32_t;

struct ogg_page {
    unsigned char* header;              // +0x00
    long           header_len;          // +0x04
    unsigned char* body;                // +0x08
    long           body_len;            // +0x0c
};

extern const ogg_uint32_t crc_lookup[256];   // 0x0139D9B0

extern "C" void __fastcall ogg_page_checksum_set(ogg_page* og)
{
    if (og) {
        ogg_uint32_t crc_reg = 0;
        int i;

        /* safety; needed for API behavior, but not framing code */
        og->header[22] = 0;
        og->header[23] = 0;
        og->header[24] = 0;
        og->header[25] = 0;

        for (i = 0; i < og->header_len; i++)
            crc_reg = (crc_reg << 8) ^ crc_lookup[((crc_reg >> 24) & 0xff) ^ og->header[i]];
        for (i = 0; i < og->body_len; i++)
            crc_reg = (crc_reg << 8) ^ crc_lookup[((crc_reg >> 24) & 0xff) ^ og->body[i]];

        og->header[22] = (unsigned char)(crc_reg & 0xff);
        og->header[23] = (unsigned char)((crc_reg >> 8) & 0xff);
        og->header[24] = (unsigned char)((crc_reg >> 16) & 0xff);
        og->header[25] = (unsigned char)((crc_reg >> 24) & 0xff);
    }
}