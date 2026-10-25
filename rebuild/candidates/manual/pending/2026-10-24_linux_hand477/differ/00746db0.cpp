// Grade: DIFFER(247v247) -- same length; retail copies C3DVector temporaries x->ecx, y->edx, z->eax (and loads appearance late), ours eax/ecx/edx: register-preference/scheduling of the vector copies (no copy ctor scalarises centre into edi/ebx/ebp, 255v247; user operator= 237v247; two returns 223v247)
// NStealth::GetStabPosOnThing(CThing& thing) -- the point a stealth stab aims at: the STAB_POS helper, else midway between the thing's position and its EYE_SET_L dummy
// Leaf: ?GetStabPosOnThing@NStealth@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00746DB0, 247 bytes, __fastcall (hidden result ecx, thing edx), plain `ret`.
// PDB: ?GetStabPosOnThing@NStealth@@YI?AVC3DVector@@AAVCThing@@@Z (the catalog's global label was the leaf alone).
// Callees: CTCGraphicAppearance::GetHelperPoint 0x004C0560, CThing::GetPosition 0x004C73D0,
//   CTCGraphicAppearance::GetDummyObject 0x004C08C0.
// Data: the static helper names g_CCharString_013bacdc "STAB_POS" and g_CCharString_013bac54 "EYE_SET_L" (as named by
//   their landed dynamic initializers / users); 0.5f @ 0x0122F59C.
// Shape: `if (!helper) { centre = GetPosition(); if (!dummy) dummyPos = centre; stabPos = (dummyPos + centre) * 0.5f; }
//   return stabPos;` with C3DVector's inline value operators (the scaled temporary is copied into stabPos).
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                        // 0x0099EAE0
};
extern CCharString g_CCharString_013bacdc;                 // "STAB_POS"
extern CCharString g_CCharString_013bac54;                 // "EYE_SET_L"

class CTCGraphicAppearance {
public:
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector* position, float time);   // 0x004C0560
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position,
                        CRightHandedSet* orientation, float scale);                                                    // 0x004C08C0
};

class CThing {
public:
    unsigned char _pad0[0x64];
    CTCGraphicAppearance* GraphicAppearance;               // +0x64
    const C3DVector& GetPosition() const;                  // 0x004C73D0
};

namespace NStealth {
C3DVector __fastcall GetStabPosOnThing(CThing& thing)
{
    C3DVector stabPos;
    if (!thing.GraphicAppearance->GetHelperPoint(g_CCharString_013bacdc, 0, 0, &stabPos, 1.0f)) {
        C3DVector centre = thing.GetPosition();
        C3DVector dummyPos;
        CRightHandedSet orientation;
        if (!thing.GraphicAppearance->GetDummyObject(g_CCharString_013bac54, 0, 0, &dummyPos, &orientation, 1.0f))
            dummyPos = centre;
        stabPos = (dummyPos + centre) * 0.5f;
    }
    return stabPos;
}
}
