// NGraphicAppearance::CTCGraphicAppearance::SetAsHighlighted(float, float, const CRGBColour&, bool, bool, EAppearanceMeshEffectPriority, void*) -- build a highlight mesh effect (its two mode bytes from the thing's appearance sub-def, else 0x80/0) and install it through SetPMeshEffect
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C04C0, length 155 bytes (+5 int3 padding), `ret 1Ch`.
// FableWin.pdb: ?SetAsHighlighted@CTCGraphicAppearance@NGraphicAppearance@@QAEXMMABVCRGBColour@@_N1W4EAppearanceMeshEffectPriority@2@PAX@Z
// Callees: CDefClassBase::GetSubDef<...> @ 0x0042B0A2 (manifest "GetSubDef<CAbilityDef>", propagated label; the sub-def carries
// two mode bytes at +0x54/+0x55), operator new @ 0x00BFEA1A, the highlight mesh-effect ctor @ 0x004C3000 (manifest label
// "CEnginePrimitive2DText", propagated; six arguments), CTCGraphicAppearance::SetPMeshEffect @ 0x004C0240.
// Shape: the sub-def pointer starts null in the `push ecx` frame word and shares the zero register with the second mode
// value; GetSubDef is a same-TU noinline stand-in body that fills it through `T**` (writes only the out slot), so it is
// read into ebp right after the call and released by hand at the end (null test, `dec [ebp+4] ; jne`, slot 1). The parent
// thing is read into a local first (the def is staged in eax, `mov ecx,eax` after the out push). The counted pointer
// argument is built in the argument area (declared copy ctor) from the raw `new` result.
// Layout: CTCBase ParentThing +0x04; the thing's def CDefPointer +0x70.
class CRGBColour { public: unsigned char B, G, R, A; };

class CDefPointeeBase { public:
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1: `call [eax+4]`
    long m_refCount;                            // +0x04
};
template <class T>
class CDefPointer { public:
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) {}
    T* operator->() const { return (T*)Object; }
};
struct CHighlightDef : CDefPointeeBase {
    unsigned char _pad08[0x54 - 0x08];
    unsigned char HighlightMode;                // +0x54
    unsigned char HighlightExtra;               // +0x55
};
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CHighlightDef** out);   // 0x0042B0A2
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CHighlightDef** out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    *out = (CHighlightDef*)def;
    return true;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;            // +0x70
};

class CEngineMeshEffect { public:
    virtual void v00();
    long m_refCount;
    unsigned char _pad08[0x1c - 0x08];
    CEngineMeshEffect(float a, float b, const CRGBColour& colour, bool flag, long mode, long extra);   // 0x004C3000
};
template <class T>
class CIVCountedPointer { public:
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
    CThing* thing = ParentThing;
    CDefClassBase* thingDef = thing->PDef.operator->();
    CHighlightDef* def = 0;
    long mode = 0x80;
    long extra = 0;
    if (thingDef->GetSubDef(&def)) {
        mode = def->HighlightMode;
        extra = def->HighlightExtra;
    }
    SetPMeshEffect(CIVCountedPointer<CEngineMeshEffect>(new CEngineMeshEffect(a, b, colour, false, mode, extra)),
                   p4, p5, false, priority, owner);
    if (def && --def->m_refCount == 0)
        def->Delete();
}
}