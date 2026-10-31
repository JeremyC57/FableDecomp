// CEngine::LoadNewScene(const CEngineInterpolationInfo& info) -- PARTIAL (prefix only): flush the pending new-scene state, notify the camera sink, take the start time and queue the current camera as a preload camera; the scene-load loop after that is not reconstructed yet
// Grade: DIFFER(111v1446) 384 diff lines (parked skeleton 3v1446, 404 lines): PARTIAL -- the prefix (scene flush, video-options sink, GFGetTime, the inline vector<CCamera>::push_back of the current camera with its placement null test and _M_insert_overflow(…, 1, true) fallback) lines up with retail 0x00B277A0..0x00B27811; the 8-aligned 0x3f4 frame and everything from the interpolation-info setup (0x00B31700 / 0x00B2FC00) on are not reconstructed
// Leaf: ?LoadNewScene@CEngine@@AAEXABVCEngineInterpolationInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B277A0, length 1456 bytes, `ret 4`, 8-aligned ebp frame (0x3f4 locals).
// Callees (prefix): 0x00B3B4A0 on the object at 0x01436E98 (name not recovered), 0x00B2F740 on the object at 0x01436E58
//   (two spellings in reloc_bindings), GFGetTime 0x009E1BC0 (?GFGetTime@@YINXZ), CCamera(const CCamera&) 0x0049D380,
//   vector<CCamera>::_M_insert_overflow 0x00B29840.
// Layout: engine/CEngine.h (NewSceneFlag +0x18, CurrentCamera +0x14, PreloadCameras +0x3c as STLport vector<CCamera>, 0x38-byte CCamera).
#define CEngine CEngineLayout
#include "engine/CEngine.h"
#undef CEngine
inline void* operator new(unsigned int, void* p) { return p; }
class CEngineInterpolationInfo;
class CCamera { public:
    unsigned char _bytes[0x38];
    CCamera(const CCamera& other);                      // 0x0049D380
};
namespace std {
struct __false_type { };
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector { public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill, bool atEnd);   // 0x00B29840
    __forceinline void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
}
class CEngineSceneSystem { public: void FlushNewScene(); };     // 0x00B3B4A0 (descriptive)
extern CEngineSceneSystem* g_pEngineSceneSystem;               // 0x01436E98 (descriptive)
class CEngineVideoOptionsSink { public: void OnVideoOptionsChanged(); };   // 0x00B2F740
extern CEngineVideoOptionsSink* g_pEngineVideoOptionsSink;     // 0x01436E58 (descriptive)
double GFGetTime();                                           // 0x009E1BC0
class CEngine : public CEngineLayout { public:
    std::vector<CCamera>& PreloadCameraList() { return *(std::vector<CCamera>*)PreloadCameras; }
private:
    void LoadNewScene(const CEngineInterpolationInfo& info);
};

void CEngine::LoadNewScene(const CEngineInterpolationInfo& info)
{
    if (NewSceneFlag)
        g_pEngineSceneSystem->FlushNewScene();
    if (g_pEngineVideoOptionsSink)
        g_pEngineVideoOptionsSink->OnVideoOptionsChanged();
    double startTime = GFGetTime();
    if (CurrentCamera)
        PreloadCameraList().push_back(*CurrentCamera);
}
