// Behaviour test for libvorbis _2class @ 0x00CA0890 (copy of the body over a fake block allocator).
#include <cstdio>
#include <cstring>
#include <cmath>
typedef struct vorbis_block { char pool[4096]; int used; } vorbis_block;
typedef void vorbis_look_residue;
typedef struct vorbis_info_residue0 { long begin; long end; int grouping; int partitions; int groupbook; int secondstages[64]; int booklist[256]; float classmetric1[64]; float classmetric2[64]; } vorbis_info_residue0;
typedef struct { vorbis_info_residue0* info; int parts; int stages; void* fullbooks; void* phrasebook; void*** partbooks; int partvals; int** decodemap; long postbits; long phrasebits; long frames; } vorbis_look_residue0;
static void* _vorbis_block_alloc(vorbis_block* vb, long bytes) { void* p = vb->pool + vb->used; vb->used += (bytes + 7) & ~7; return p; }
static long** _2class(vorbis_block* vb, vorbis_look_residue* vl, float** in, int ch)
{
    long i, j, k, l;
    vorbis_look_residue0* look = (vorbis_look_residue0*)vl;
    vorbis_info_residue0* info = look->info;
    int samples_per_partition = info->grouping;
    int possible_partitions = info->partitions;
    int n = info->end - info->begin;
    int partvals = n / samples_per_partition;
    long** partword = (long**)_vorbis_block_alloc(vb, sizeof(*partword));
    float magmax = 0.f;
    float angmax = 0.f;
    partword[0] = (long*)_vorbis_block_alloc(vb, n * ch / samples_per_partition * sizeof(*partword[0]));
    memset(partword[0], 0, n * ch / samples_per_partition * sizeof(*partword[0]));
    for (i = 0, l = info->begin / ch; i < partvals; i++) {
        magmax = 0.f; angmax = 0.f;
        for (j = 0; j < samples_per_partition; j += ch) {
            if (fabs(in[0][l]) > magmax) magmax = fabs(in[0][l]);
            for (k = 1; k < ch; k++) if (fabs(in[k][l]) > angmax) angmax = fabs(in[k][l]);
            l++;
        }
        for (j = 0; j < possible_partitions - 1; j++)
            if (magmax <= info->classmetric1[j] && angmax <= info->classmetric2[j]) break;
        partword[0][i] = j;
    }
    look->frames++;
    return (partword);
}
int main()
{
    static vorbis_block vb; vb.used = 0;
    vorbis_info_residue0 info; memset(&info, 0, sizeof(info));
    info.begin = 0; info.end = 8; info.grouping = 4; info.partitions = 3;
    info.classmetric1[0] = 1.0f; info.classmetric2[0] = 1.0f;   // class 0: both maxima <= 1
    info.classmetric1[1] = 3.0f; info.classmetric2[1] = 3.0f;   // class 1: both <= 3, else class 2
    vorbis_look_residue0 look; memset(&look, 0, sizeof(look)); look.info = &info; look.frames = 5;
    // ch = 2: samples l = 0..3 (two per partition), partition 0 -> small, partition 1 -> magnitude 2.5 and angle -3.5
    float mag[4] = {0.5f, -0.75f, 2.5f, 1.0f};
    float ang[4] = {0.25f, 0.5f, -3.5f, 0.0f};
    float* in[2] = {mag, ang};
    long** pw = _2class(&vb, &look, in, 2);
    if (pw == 0 || pw[0] == 0) { std::printf("00ca0890_TEST FAIL\n"); return 1; }
    if (pw[0][0] != 0 || pw[0][1] != 2 || look.frames != 6) { std::printf("00ca0890_TEST FAIL\n"); return 1; }
    std::printf("00ca0890_TEST PASS\n");
    return 0;
}