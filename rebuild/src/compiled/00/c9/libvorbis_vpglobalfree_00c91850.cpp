// libvorbis::_vp_global_free(vorbis_look_psy_global* look) -- libvorbis psy.c: wipe the global psychoacoustic look and free it (null-safe)
// Leaf: @_vp_global_free@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C91850, 43 bytes (+5 int3). libvorbis lib/psy.c inside libpk's statically linked
//   Xiph libvorbis ("Xiph.Org libVorbis I 20030909"); LTCG `__fastcall` (look in ecx, plain `ret`).
// Identity: it directly follows _vp_global_look 0x00C91820 (agent447: calloc(1, 0x24)), and frees exactly that 0x24-byte
//   vorbis_look_psy_global {ampmax, channels, gi, coupling_pointlimit[2][P_NOISECURVES=3]} -- nine stores.
// Callee: free (MSVCR71 import [0x01440154]) through _ogg_free.
// Shape: the shipped source; memset(look,0,0x24) is the intrinsic (`xor eax,eax` + nine stores through edx, the copy
//   of look), the free's `push ecx` scheduled before the last store; the result is dropped (`pop ecx`).
// Label: manifest "allocator<std::vector<std::pair<long...>>>::construct" is propagated; the real name is the psy.c
//   function `_vp_global_free`.
extern "C" __declspec(dllimport) void __cdecl free(void* block);   // [0x01440154]
extern "C" void* __cdecl memset(void* dst, int c, unsigned int n);
#pragma intrinsic(memset)
#define _ogg_free free

#define VE_BANDS 7
#define P_NOISECURVES 3
#define PACKETBLOBS 15

typedef struct vorbis_info_psy_global vorbis_info_psy_global;

typedef struct {
    float ampmax;                                   // +0x00
    int channels;                                   // +0x04
    vorbis_info_psy_global* gi;                     // +0x08
    int coupling_pointlimit[2][P_NOISECURVES];      // +0x0c
} vorbis_look_psy_global;

extern "C" void __fastcall _vp_global_free(vorbis_look_psy_global* look)
{
    if (look) {
        memset(look, 0, sizeof(*look));
        _ogg_free(look);
    }
}