// behaviour test for 0x0075CFE0 CTCCameraPointScriptedSpline::GetKeyCamera
#include <cstdio>
struct CKeyCamera { unsigned char _body[0x38]; };
struct SS {
    CKeyCamera* first; CKeyCamera* last;
    unsigned int NumKeyCameras() const { return (unsigned int)(last - first); }
    long GetKeyCamera(float t) const
    {
        if (t >= 1.0f) return NumKeyCameras() - 1;
        return (long)((NumKeyCameras() - 1) * t);
    }
};
int main()
{
    CKeyCamera k[5];
    SS s; s.first = k; s.last = k + 5;
    bool ok = s.GetKeyCamera(1.0f) == 4 && s.GetKeyCamera(2.0f) == 4 && s.GetKeyCamera(0.0f) == 0
        && s.GetKeyCamera(0.5f) == 2 && s.GetKeyCamera(0.99f) == 3;
    if (!ok) { std::printf("0075cfe0_TEST FAIL\n"); return 1; }
    std::printf("0075cfe0_TEST PASS\n");
    return 0;
}