// CMainGameComponent::ConsoleSetResolution(CMainGameComponent& component, const std::vector<CGenericVar>& args) -- console command: switch the display to the width, height and colour depth given as the three integer arguments
// Leaf: ?ConsoleSetResolution@CMainGameComponent@@KIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00417568, length 114 bytes, plain `ret` after `leave` (static
// `__fastcall`: component in ecx, args in edx). Name: Ego_r.pdb / FableWin.pdb
// ?ConsoleSetResolution@CMainGameComponent@@KIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z.
// Body: a CCharString message is formatted with "Nearest supported resolution is (%d,%d,%d)" (0x0122EED8) from
// args[0..2].ValSLONG and never shown (the console print was compiled out, the string is only destroyed);
// then PDisplayEngine->PreChangeResolution(), g_pDisplayManager->SetDisplayMode(C2DExtentsI(width, height), depth,
// 0, 0) and component.PostChangeResolution(width, height, depth) with the requested values.
// Callees: CCharString::CCharString() @ 0x0099E4B0, CCharString::Format(const char*, ...) @ 0x0099F1F0 (varargs member:
//          __cdecl with `this` pushed first), CDisplayEngine::PreChangeResolution @ 0x00434A10,
//          CDisplayManager::SetDisplayMode @ 0x009BFF40, CMainGameComponent::PostChangeResolution @ 0x00416365,
//          CCharString::~CCharString @ 0x0099EAE0.
// Layout: CGenericVar is 28 bytes (vptr, Type, ValString, ValSLONG +0x0c, ValULONG, ValBool, ValFloat), as in the
// landed ConsoleForceUpdateTickSpeed (0x00416C35); CMainGameComponent PDisplayEngine +0x28.
// Shape: size TU with a frame pointer (`optimize("s", on)` + `("y", off)`): the component reference is homed at
// [ebp-4], the three values live in esi/edi/ebx across every call, the C2DExtentsI temporary sits at [ebp-10h].
// Model: local -- engine/CDisplayEngine.h and CWorld.h exist, but the generated header is data-only (pack(1), no member
// functions, no virtuals: the vptr and counted pointers are flattened into raw words); this body calls their member functions,
// so they are declared locally.
#pragma optimize("s", on)
#pragma optimize("y", off)
class CCharString {
public:
    CCharString();                                       // 0x0099E4B0
    ~CCharString();                                      // 0x0099EAE0
    void Format(const char* format, ...);                // 0x0099F1F0
private:
    void* PStringData;
};
class CGenericVar {
public:
    virtual ~CGenericVar();
    long          Type;                                  // +0x04 EGenericVarType
    void*         ValString;                             // +0x08 CCharString
    long          ValSLONG;                              // +0x0c
    unsigned long ValULONG;                              // +0x10
    bool          ValBool;                               // +0x14
    float         ValFloat;                              // +0x18
};
namespace std {
template <class T> class allocator {};
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T& operator[](unsigned int n) const { return _M_start[n]; }
};
}
enum ESurfaceMultisampleType { SURFACE_MULTISAMPLE_NONE = 0 };
enum ECreateDeviceSuccess { CREATE_DEVICE_OK = 0 };
struct C2DExtentsI {
    long Width;
    long Height;
    C2DExtentsI(long width, long height) : Width(width), Height(height) {}
};
struct CDisplayManager {
    ECreateDeviceSuccess SetDisplayMode(const C2DExtentsI& resolution, unsigned long bitsPerPixel,
                                        ESurfaceMultisampleType antialiasing, unsigned long refreshRate);   // 0x009BFF40
};
extern CDisplayManager* g_pDisplayManager;               // 0x013B8390
class CDisplayEngine {
public:
    void PreChangeResolution();                          // 0x00434A10
};
class CWorld;
class CMainGameComponent {
public:
    void PostChangeResolution(unsigned long width, unsigned long height, unsigned long depth);   // 0x00416365
    unsigned char   _pad00[0x24];
    CWorld*         PWorld;                              // +0x24 scoped_ptr<CWorld>
    CDisplayEngine* PDisplayEngine;                      // +0x28 scoped_ptr<CDisplayEngine>
protected:
    static void __fastcall ConsoleSetResolution(CMainGameComponent& component, const std::vector<CGenericVar>& args);
};

void __fastcall CMainGameComponent::ConsoleSetResolution(CMainGameComponent& component, const std::vector<CGenericVar>& args)
{
    CCharString message;
    unsigned long width = args[0].ValSLONG;
    unsigned long height = args[1].ValSLONG;
    unsigned long depth = args[2].ValSLONG;
    message.Format("Nearest supported resolution is (%d,%d,%d)", width, height, depth);
    component.PDisplayEngine->PreChangeResolution();
    g_pDisplayManager->SetDisplayMode(C2DExtentsI(width, height), depth, SURFACE_MULTISAMPLE_NONE, 0);
    component.PostChangeResolution(width, height, depth);
}