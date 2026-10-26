// CEngineCamera::ConsoleAddDebugScript(CEngineCamera& camera, const vector<CGenericVar>& params) -- console command: build one debug-script element {world position, normalised look vector, FOV, time to next element} from the float values of eight parsed console arguments and append it to the camera's DebugScript
// Leaf: ?ConsoleAddDebugScript@CEngineCamera@@CIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B317A0, length 154 bytes. Private static `__fastcall` (camera in ecx, the
// argument vector in edx), plain `ret`.
// Name: Ego_r.pdb / Ego_d.pdb name sets ?ConsoleAddDebugScript@CEngineCamera@@CIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@
// VCGenericVar@@@std@@@std@@@Z (`CI` = private static __fastcall). Layouts (struct_layouts_egor): CEngineCamera::DebugScript
// (CArray<CEngineCamera::CDebugScriptElement>) at +0x208 (520); CDebugScriptElement {C3DVector WorldPos +0, C3DVector
// LookVector +0xC, float FOV +0x18, float TimeToNextElement +0x1C} (32 bytes); CGenericVar is 0x1C bytes with ValFloat at
// +0x18 (the `0x18 + k*0x1C` loads). The catalog name AddCameraKey and the manifest CTCActionUseReadable::OnCreate were
// descriptive / propagated. No direct xref: console commands are registered through function pointers built elsewhere.
// Callees: C3DVector::AccurateNormalise @ 0x00A14440 (in place; catalog label "C3DVector::GetScaled"), the DebugScript vector's
// push_back @ 0x00B31C10 (its only caller is this function).
// Shape: the two vectors are `element.V = C3DVector(a, b, c)` with only the 3-float ctor declared: the temporary is built in
// a shared frame slot and block-copied through integer registers (a user copy ctor or operator= turns the copies into x87
// loads/stores).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector& AccurateNormalise();                              // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
};

class CGenericVar {                                // 0x1C bytes (struct_layouts_egor)
public:
    unsigned char _pad00[0x18];                    // +0x00 (vptr, Type, ValString, ValSLONG, ValULONG, ValBool)
    float ValFloat;                                // +0x18
};

namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage;
    const _Tp& operator[](unsigned int __n) const { return *(_M_start + __n); }
    void push_back(const _Tp& __x);
};
}

class CEngineCamera {
public:
    struct CDebugScriptElement {
        C3DVector WorldPos;                        // +0x00
        C3DVector LookVector;                      // +0x0C
        float FOV;                                 // +0x18
        float TimeToNextElement;                   // +0x1C
    };
    unsigned char _pad00[0x208];
    std::vector<CDebugScriptElement> DebugScript;  // +0x208 (CArray<CDebugScriptElement>; push_back @ 0x00B31C10)
private:
    static void __fastcall ConsoleAddDebugScript(CEngineCamera& camera, const std::vector<CGenericVar>& params);
};

void __fastcall CEngineCamera::ConsoleAddDebugScript(CEngineCamera& camera, const std::vector<CGenericVar>& params)
{
    CDebugScriptElement element;
    element.WorldPos = C3DVector(params[0].ValFloat, params[1].ValFloat, params[2].ValFloat);
    element.LookVector = C3DVector(params[3].ValFloat, params[4].ValFloat, params[5].ValFloat);
    element.FOV = params[6].ValFloat;
    element.TimeToNextElement = params[7].ValFloat;
    element.LookVector.AccurateNormalise();
    camera.DebugScript.push_back(element);
}
