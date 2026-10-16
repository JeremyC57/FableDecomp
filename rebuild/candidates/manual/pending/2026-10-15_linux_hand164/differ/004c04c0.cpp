// NGraphicAppearance::CTCGraphicAppearance::SetAsHighlighted(float, float, const CRGBColour&, bool, bool, EAppearanceMeshEffectPriority, void*) -- build a highlight mesh effect (its two mode bytes from the thing's appearance sub-def, else 0x80/0) and install it through SetPMeshEffect
// Grade: DIFFER(155v155) -- register allocation only: retail stages the thing def in eax (`mov ecx,eax` after the out push) and reads the sub-def straight into ebp; ours loads ecx directly, lea uses edx, and routes the def through ecx; arg loads ecx/edx swapped
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C04C0, length 155 bytes (+5 int3 padding), `ret 1Ch`.
// FableWin.pdb: ?SetAsHighlighted@CTCGraphicAppearance@NGraphicAppearance@@QAEXMMABVCRGBColour@@_N1W4EAppearanceMeshEffectPriority@2@PAX@Z
// Callees: CDefClassBase::GetSubDef<...> @ 0x0042B0A2 (manifest "GetSubDef<CAbilityDef>", propagated label; the sub-def carries
// two mode bytes at +0x54/+0x55), operator new @ 0x00BFEA1A, the highlight mesh-effect ctor @ 0x004C3000 (manifest label
// "CEnginePrimitive2DText", propagated; six arguments), CTCGraphicAppearance::SetPMeshEffect @ 0x004C0240.
// Shape: the sub-def out pointer starts null in the `push ecx` frame word and shares the zero register with the second mode
// value; its object is read into ebp right after the call and released by hand at the end (null test, `dec [ebp+4] ; jne`,
// slot 1). The counted pointer argument is built in the argument area (declared copy ctor) from the raw `new` result.
// Layout: CTCBase ParentThing +0x04; the thing's def CDefPointer +0x70.
struct CRGBColour { unsigned char B, G, R, A; };

struct CDefPointeeBase {
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1: `call [eax+4]`
    long m_refCount;                            // +0x04
};
template <class T>
struct CDefPointer {
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) {}
    T* operator->() const { return (T*)Object; }
};
struct CHighlightDef : CDefPointeeBase {
    unsigned char _pad08[0x54 - 0x08];
    unsigned char HighlightMode;                // +0x54
    unsigned char HighlightExtra;               // +0x55
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<const CHighlightDef>* out);   // 0x0042B0A2
};
struct CThing {
    unsigned char _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;            // +0x70
};

struct CEngineMeshEffect {
    virtual void v00();
    long m_refCount;
    unsigned char _pad08[0x1c - 0x08];
    CEngineMeshEffect(float a, float b, const CRGBColour& colour, bool flag, long mode, long extra);   // 0x004C3000
};
template <class T>
struct CIVCountedPointer {
    T* Data;
    CIVCountedPointer(T* p) : Data(p) {}
    CIVCountedPointer(const CIVCountedPointer& o);
};
enum EAppearanceMeshEffectPriority { MESH_EFFECT_PRIORITY_DEFAULT };

namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    void SetPMeshEffect(CIVCountedPointer<CEngineMeshEffect> effect, bool a, bool b, bool c,
                        EAppearanceMeshEffectPriority priority, void* owner);   // 0x004C0240
    void SetAsHighlighted(float a, float b, const CRGBColour& colour, bool p4, bool p5,
                          EAppearanceMeshEffectPriority priority, void* owner);
};

void CTCGraphicAppearance::SetAsHighlighted(float a, float b, const CRGBColour& colour, bool p4, bool p5,
                                            EAppearanceMeshEffectPriority priority, void* owner)
{
    CDefClassBase* thingDef = ParentThing->PDef.operator->();
    CDefPointer<const CHighlightDef> out;
    long mode = 0x80;
    long extra = 0;
    bool found = thingDef->GetSubDef(&out);
    CHighlightDef* def = (CHighlightDef*)out.Object;
    if (found) {
        mode = def->HighlightMode;
        extra = def->HighlightExtra;
    }
    SetPMeshEffect(CIVCountedPointer<CEngineMeshEffect>(new CEngineMeshEffect(a, b, colour, false, mode, extra)),
                   p4, p5, false, priority, owner);
    if (def && --def->m_refCount == 0)
        def->Delete();
}
}
