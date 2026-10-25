// Behaviour test for 0x00C9FDF0 libvorbis res0_free_look (copy of the model + logic, counting frees).
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct vorbis_look_residue0 {
    void* info; int parts; int stages; void* fullbooks; void* phrasebook; void*** partbooks; int partvals;
    int** decodemap; long postbits, phrasebits, frames;
};
static int g_frees;
static void count_free(void* p) { g_frees++; std::free(p); }
static void free_look(void* i)
{
    int j;
    if (i) {
        vorbis_look_residue0* look = (vorbis_look_residue0*)i;
        for (j = 0; j < look->parts; j++)
            if (look->partbooks[j]) count_free(look->partbooks[j]);
        count_free(look->partbooks);
        for (j = 0; j < look->partvals; j++) count_free(look->decodemap[j]);
        count_free(look->decodemap);
        std::memset(look, 0, sizeof(*look));
        count_free(look);
    }
}
int main()
{
    int j;
    vorbis_look_residue0* look = (vorbis_look_residue0*)std::calloc(1, sizeof(vorbis_look_residue0));
    look->parts = 4; look->partvals = 3;
    look->partbooks = (void***)std::calloc(4, sizeof(void**));
    look->partbooks[1] = (void**)std::malloc(8); look->partbooks[3] = (void**)std::malloc(8);   /* 0 and 2 empty */
    look->decodemap = (int**)std::calloc(3, sizeof(int*));
    for (j = 0; j < 3; j++) look->decodemap[j] = (int*)std::malloc(8);
    free_look(look);
    int ok = g_frees == 2 + 1 + 3 + 1 + 1;
    free_look(0);
    if (g_frees != 8) ok = 0;
    if (!ok) { std::printf("00c9fdf0_TEST FAIL\n"); return 1; }
    std::printf("00c9fdf0_TEST PASS\n");
    return 0;
}