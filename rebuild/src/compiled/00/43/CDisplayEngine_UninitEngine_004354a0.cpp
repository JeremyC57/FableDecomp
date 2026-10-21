// CDisplayEngine::UninitEngine() -- releases the 3D engine and unregisters it from the engine manager
// Leaf: UninitEngine@CDisplayEngine_Methods
// Retail: Fable.exe @ 0x004354A0, 29 bytes; CDisplayEngine vtable entry (abs ref 0x0123157C); Ego_r.pdb
// ?UninitEngine@CDisplayEngine@@MAEXXZ. Body: Engine3D (+0x2c)->Release() (CIEngine slot 3, +0xc, Ego_r.pdb
// ?Release@CIEngine@@UAEXXZ), then CTBaseSingleton<CEngineManager>::Get() (0x00436FB0, static, returns the instance in eax)
// ->RemoveEngine(*Engine3D) (0x006404B0, `ret 4`; manifest label "AddLearnedExpression" is ICF-propagated). The argument
// push precedes the Get() call (arguments are evaluated before the object expression).
// Layout: engine/CDisplayEngine.h (Engine3D +0x2c, as Ego_r.pdb). engine/CIEngine.h is data-only, so CIEngine's virtuals
// are declared locally (the display header only forward-declares it).
#include "engine/CDisplayEngine.h"
class CEngineInit;
struct CIEngine {
    virtual ~CIEngine();                                 // slot 0
    virtual void Validate() const;                       // slot 1 (CInitBaseClass)
    virtual bool Initialise(const CEngineInit& init);    // slot 2
    virtual void Release();                              // slot 3 (+0xc)
};
class CEngineManager {
public:
    void RemoveEngine(CIEngine& engine);                 // 0x006404B0
};
template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                          // 0x00436FB0
};
struct CDisplayEngine_Methods : CDisplayEngine {
    void UninitEngine();
};
void CDisplayEngine_Methods::UninitEngine()
{
    Engine3D->Release();
    CTBaseSingleton<CEngineManager>::Get().RemoveEngine(*Engine3D);
}