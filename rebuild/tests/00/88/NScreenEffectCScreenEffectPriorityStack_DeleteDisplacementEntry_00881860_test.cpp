#include <cstdio>
// Behaviour model of the NScreenEffect::CScreenEffectPriorityStack key-set functions (0x00880F60 .. 0x00881860):
// look the entry up in the stack's key set and touch it only when it is still registered; the set (find / erase)
// is a small array here.
struct C3DVector { float X, Y, Z; };
struct CRGBFloatColour { float R, G, B, A; };
struct CRadialBlurParams { CRGBFloatColour ThresholdColour, IntensityColour; float Length, Fade, Range, Twist; CRadialBlurParams() {} };
enum ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION { TRACK_NONE = 0, TRACK_HERO = 1, TRACK_CAMERA = 2 };
struct CRadialBlurPositionParams { C3DVector AreaCenter, BlurCenter; ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION AreaTrackPos, BlurTrackPos; };
static long g_framesPerSecond = 15;
struct CStackEntry { virtual ~CStackEntry() {} long curdurationcount; long durationcount; bool fadingout; };
struct CRadialBlurEntry : CStackEntry { CRadialBlurParams param; CRadialBlurPositionParams posparam; };
struct CDisplacementEntry : CStackEntry { CRGBFloatColour paramcol; };
struct CKeySet {
    CStackEntry* keys[8]; int count;
    CKeySet() : count(0) {}
    int find(CStackEntry* const& key) const { for (int i = 0; i < count; ++i) if (keys[i] == key) return i; return -1; }
    unsigned long erase(CStackEntry* const& key) { int i = find(key); if (i < 0) return 0; keys[i] = keys[--count]; return 1; }
    void insert(CStackEntry* key) { keys[count++] = key; }
};
struct CScreenEffectPriorityStack {
    CKeySet RadialBlurKeys; CKeySet DisplacementKeys;
    CRadialBlurParams GetRadialBlurParam(CRadialBlurEntry* entry)
    {
        int it = RadialBlurKeys.find(entry);
        if (it >= 0) return static_cast<CRadialBlurEntry*>(RadialBlurKeys.keys[it])->param;
        return CRadialBlurParams();
    }
    void SetAreaCenter(CRadialBlurEntry* entry, const C3DVector& c) { int it = RadialBlurKeys.find(entry); if (it >= 0) static_cast<CRadialBlurEntry*>(RadialBlurKeys.keys[it])->posparam.AreaCenter = c; }
    void SetBlurCenter(CRadialBlurEntry* entry, const C3DVector& c) { int it = RadialBlurKeys.find(entry); if (it >= 0) static_cast<CRadialBlurEntry*>(RadialBlurKeys.keys[it])->posparam.BlurCenter = c; }
    void SetAreaTrackPos(CRadialBlurEntry* entry, ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION t) { int it = RadialBlurKeys.find(entry); if (it >= 0) static_cast<CRadialBlurEntry*>(RadialBlurKeys.keys[it])->posparam.AreaTrackPos = t; }
    void SetBlurTrackPos(CRadialBlurEntry* entry, ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION t) { int it = RadialBlurKeys.find(entry); if (it >= 0) static_cast<CRadialBlurEntry*>(RadialBlurKeys.keys[it])->posparam.BlurTrackPos = t; }
    void DeleteRadialBlurEntry(CRadialBlurEntry* entry, float fadeTime)
    {
        if (RadialBlurKeys.erase(entry) > 0) { entry->fadingout = true; entry->curdurationcount = 0; entry->durationcount = (long)(fadeTime * g_framesPerSecond); }
    }
    void DeleteDisplacementEntry(CDisplacementEntry* entry, float fadeTime)
    {
        if (DisplacementKeys.erase(entry) > 0) { entry->fadingout = true; entry->curdurationcount = 0; entry->durationcount = (long)(fadeTime * g_framesPerSecond); }
    }
};
static void Reset(CRadialBlurEntry& e) { e.posparam.AreaCenter.X = e.posparam.BlurCenter.X = -1.0f; e.posparam.AreaTrackPos = e.posparam.BlurTrackPos = TRACK_NONE; e.param.Length = 3.0f; e.param.Twist = 0.5f; e.curdurationcount = 9; e.durationcount = 9; e.fadingout = false; }
int main()
{
    CScreenEffectPriorityStack s; CRadialBlurEntry in, out; Reset(in); Reset(out); s.RadialBlurKeys.insert(&in);
    C3DVector c = { 4.0f, 5.0f, 6.0f };
    int ok = 1;
    CDisplacementEntry d; d.fadingout = false; d.curdurationcount = 5; d.durationcount = 5; s.DisplacementKeys.insert(&d);
    s.DeleteDisplacementEntry(&d, 0.5f);
    ok = d.fadingout && d.curdurationcount == 0 && d.durationcount == 7 && s.DisplacementKeys.count == 0;
    d.fadingout = false; s.DeleteDisplacementEntry(&d, 1.0f);
    ok = ok && !d.fadingout && d.durationcount == 7;
    if (!ok) { std::printf("00881860_TEST FAIL\n"); return 1; }
    std::printf("00881860_TEST PASS\n");
    return 0;
}