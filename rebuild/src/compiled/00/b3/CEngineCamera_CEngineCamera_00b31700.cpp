// CEngineCamera::CEngineCamera(bool buildDebugScript) -- engine camera constructor: CEngineComponent base, vtable, member defaults; with the flag set it builds (and drops) a local eight-entry debug script array
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B31700, 151 bytes (+9 int3), __fastcall, ret 4, returns `this`.
// Leaf: ??0CEngineCamera@@
// The catalog label (CAnimInit::Clear) is propagated: the vtable stored here, 0x012A12A4, is
// CEngineCamera's (rtti-vtables.tsv), the body returns `this` and both callers (0x00B26614, 0x00B2781A)
// construct the camera.
// Callees: CEngineComponent::CEngineComponent 0x00B59710 (vptr, a dword, two flags), retail vector<long>
// push_back 0x009EBD90, free 0x00BFEA14.
// Layout: engine/CEngineCamera.h is the PDB layout, where DebugScript (a CArray) is 16 bytes; the retail
// array is 12 (first/last/end), so CurrentDebugScriptElement and DebugScriptRunning sit at +0x214/+0x218
// instead of +0x218/+0x21C -- the class is modelled locally for that reason.
// Shape: the embedded CEngineCameraDesc and the DebugScript array have inline constructors (the byte at
// desc+0x68 and the three array words are stored right after the vtable); the scalar defaults are body
// assignments in source order (both viewport overrides -1, the camera/window pointers, the debug state).
// The flag branch holds a local array (three zero words at [esp+0Ch]) filled by eight `push_back(3)`
// calls -- the constant temporary is re-stored every iteration in the dead flag argument slot -- and its
// inline destructor frees the storage when it is non-null.
#include <stdlib.h>

struct CEngineComponent {                          // vtable 0x012A2544
    virtual void v00();
    long GlobalPointerAddress;                                      // +0x04
    bool m_enabled;                                // +0x08
    bool KeepAlive;                                      // +0x09
    CEngineComponent();                            // 0x00B59710
};

class CCamera;
struct C2DBoxF;

struct CEngineCameraDesc {                         // 0x6C bytes
    unsigned char m_data[0x68];
    bool m_valid;                                  // +0x68
    CEngineCameraDesc() : m_valid(false) {}
};

struct CLongArray {                                // retail vector<long>: first / last / end of storage
    long* m_pFirst;
    long* m_pLast;
    long* m_pEnd;
    CLongArray() : m_pFirst(0), m_pLast(0), m_pEnd(0) {}
    ~CLongArray() { if (m_pFirst) free(m_pFirst); }
    void push_back(const long& value);             // 0x009EBD90
};

struct CDebugScriptArray {                         // CArray<CEngineCamera::CDebugScriptElement>, retail 12 bytes
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    CDebugScriptArray() : m_pFirst(0), m_pLast(0), m_pEnd(0) {}
};

struct CEngineCamera : CEngineComponent {
    CCamera* GameCamera;                           // +0x0C
    C2DBoxF* GameWindow;                           // +0x10
    CEngineCameraDesc CameraDesc;                  // +0x14
    unsigned char m_transforms[0x180];             // +0x80 view/inverse/projection matrices, look vector, frustum planes
    long ConsoleOverrideViewPortWidth;             // +0x200
    long ConsoleOverrideViewPortHeight;            // +0x204
    CDebugScriptArray DebugScript;                 // +0x208
    long CurrentDebugScriptElement;                // +0x214
    bool DebugScriptRunning;                       // +0x218
    double DebugScriptElementStartTime;            // +0x220
    virtual void v01();
    CEngineCamera(bool buildDebugScript);
};

CEngineCamera::CEngineCamera(bool buildDebugScript)
{
    ConsoleOverrideViewPortWidth = -1;
    ConsoleOverrideViewPortHeight = -1;
    GameCamera = 0;
    GameWindow = 0;
    DebugScriptRunning = false;
    CurrentDebugScriptElement = 0;
    if (buildDebugScript) {
        CLongArray script;
        for (long i = 0; i < 8; ++i)
            script.push_back(3);
    }
}