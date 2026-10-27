#include <cstdio>
// _vorbis_window_get: the window table pointer for blocksize index n.
static float vwin64[32], vwin128[64], vwin256[128];
static float* vwin[3] = { vwin64, vwin128, vwin256 };
static float* _vorbis_window_get(int n) { return vwin[n]; }
int main()
{
    if (_vorbis_window_get(0) != vwin64 || _vorbis_window_get(1) != vwin128 || _vorbis_window_get(2) != vwin256) {
        std::printf("00c9ea70_TEST FAIL\n"); return 1;
    }
    std::printf("00c9ea70_TEST PASS\n");
    return 0;
}