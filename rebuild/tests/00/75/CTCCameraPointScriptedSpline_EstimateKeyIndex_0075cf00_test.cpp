// Behaviour test for CTCCameraPointScriptedSpline::EstimateKeyIndex (0x0075CF00).
#include <cstdio>

struct CKeyCamera { float DeltaIndex; float Pad[9]; float Duration; float Rest[3]; };
template <class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
class CTCCameraPointScriptedSpline {
public:
    CRetailVector<CKeyCamera> KeyCameras;
    void EstimateKeyIndex()
    {
        if (KeyCameras.size() > 1) {
            float spacing = 1.0f / (float)(KeyCameras.size() - 1);
            for (unsigned long i = 0; i != KeyCameras.size(); ++i) {
                CKeyCamera& key = KeyCameras[i];
                float frames = key.Duration * 15.0f;
                if (frames == 0.0f) key.DeltaIndex = spacing;
                else key.DeltaIndex = spacing / frames;
            }
        }
    }
};

int main()
{
    CKeyCamera keys[5];
    for (int i = 0; i < 5; ++i) { keys[i].DeltaIndex = -1.0f; keys[i].Duration = 0.0f; }
    keys[1].Duration = 2.0f;
    CTCCameraPointScriptedSpline s; s.KeyCameras._M_start = keys; s.KeyCameras._M_finish = keys + 5; s.KeyCameras._M_end_of_storage = keys + 5;
    s.EstimateKeyIndex();
    float spacing = 1.0f / 4.0f; float slow = spacing / 30.0f;
    bool ok = keys[0].DeltaIndex == spacing && keys[1].DeltaIndex == slow && keys[4].DeltaIndex == spacing;
    keys[0].DeltaIndex = -1.0f; s.KeyCameras._M_finish = keys + 1;      // one key: untouched
    s.EstimateKeyIndex();
    ok = ok && keys[0].DeltaIndex == -1.0f;
    if (!ok) { std::printf("0075cf00_TEST FAIL\n"); return 1; }
    std::printf("0075cf00_TEST PASS\n");
    return 0;
}