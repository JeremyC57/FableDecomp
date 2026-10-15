// AddCameraKey(CEngineCamera* camera, const CScriptParams& params) -- engine-camera debug-script helper: build a key {position, normalised direction, two floats} from the values of eight parsed parameters and push it on the camera's key vector at +0x208
// Leaf: AddCameraKey
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B317A0, length 154 bytes. `__fastcall` (camera in ecx, the parameter block in
// edx), plain `ret`.
// Label correction: catalogued as CTCActionUseReadable::OnCreate (propagated). The body sits in the CEngineCamera module between
// CEngineCamera::Process (0x00B314E0) and CAnimInit/vector helpers, takes its two arguments in ecx/edx and has no direct call,
// jump or absolute reference in the image (an unreferenced static helper of the debug-script code at 0x00B31160); the class
// owning the key vector at +0x208 is inferred, the member names are descriptive.
// Callees: C3DVector::Normalise() @ 0x00A14440 (in place; catalog label "C3DVector::GetScaled"), the key vector's push_back
// @ 0x00B31C10 (catalog "CScriptModuleTransitionBetweenPoints::OnInitLeaf", the usual propagated push_back label).
// Shape: each parameter is a 0x1C-byte record with its float value at +0x18 (offsets 0x18 + k*0x1C, k = 0..7). The two vectors
// are `key.V = C3DVector(a, b, c)` with only the 3-float ctor declared: the temporary is built in a shared frame slot and block-
// copied through integer registers (a user copy ctor or operator= turns the copies into x87 loads/stores).
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    void Normalise();                              // 0x00A14440
};
struct CScriptParam {                              // 0x1C-byte parsed parameter
    unsigned char _pad00[0x18];
    float Value;                                   // +0x18
};
struct CScriptParams { CScriptParam* Data; };
struct CCameraKey {
    C3DVector Position;
    C3DVector Direction;
    float Time;
    float Speed;
};
struct CCameraKeyVector {
    CCameraKey* _M_start; CCameraKey* _M_finish; CCameraKey* _M_end_of_storage;
    void push_back(const CCameraKey& key);         // 0x00B31C10
};
struct CEngineCamera {
    unsigned char _pad00[0x208];
    CCameraKeyVector Keys;                         // +0x208
};

void __fastcall AddCameraKey(CEngineCamera* camera, const CScriptParams& params)
{
    CCameraKey key;
    key.Position = C3DVector(params.Data[0].Value, params.Data[1].Value, params.Data[2].Value);
    key.Direction = C3DVector(params.Data[3].Value, params.Data[4].Value, params.Data[5].Value);
    key.Time = params.Data[6].Value;
    key.Speed = params.Data[7].Value;
    key.Direction.Normalise();
    camera->Keys.push_back(key);
}