// CEnginePrimitiveManagerText::CreateInternalPrimitive(long type) -- type 5: allocate a 3D text primitive (0x68 bytes, flags 0x2000000); type 0x27: a 2D text primitive (0x6c bytes, flags 0x80000000); null for any other type or a failed allocation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC68B0, length 125 bytes (+ int3 padding).
// Callees: operator new @ 0x00BFEA1A (non-throwing), CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase @
// 0x00B8FF00 (type, flags), CWideString::CWideString() @ 0x0099AED0 (the Text member at +0x60). Vtables 0x012A676C =
// CEngineInternalPrimitiveText, 0x012A67E4 = CEngineInternalPrimitive2DText (rtti-vtables.tsv).
// Layout: PDB CEngineInternalPrimitiveText (0x68): base (0x48), Pos +0x48, Scale +0x54, Colour +0x58, ScaleIn3D +0x5c,
// Text +0x60, Font +0x64; CEngineInternalPrimitive2DText (0x6c) derives from it and adds Layer at +0x68.
// Shape: a two-case `switch` on the type (compare 5 first, its block placed after the 0x27 block); each case is the
// inlined constructor chain: base call, the text class's vtable, the CWideString member constructor, and for the
// 2D class its own vtable last; `this` is unused.
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A

class CEngineInternalPrimitiveBase { public:              // engine/CEngineInternalPrimitiveBase.h (PDB, 0x48 bytes): the base constructor takes (type, flags)
    virtual void v00();
    unsigned char _pad04[0x44];                    // +0x04 .. +0x48
    CEngineInternalPrimitiveBase(long type, unsigned long flags);   // 0x00B8FF00
};

class CWideString {
public:
    void* m_pStringData;
    CWideString();                                 // 0x0099AED0
};

class CEngineInternalPrimitiveText : public CEngineInternalPrimitiveBase { public:   // vtable 0x012A676C; 0x68 bytes
    float         Pos[3];                          // +0x48 C3DVector
    float         Scale;                           // +0x54
    unsigned long Colour;                          // +0x58 CRGBColour
    bool          ScaleIn3D;                       // +0x5c
    CWideString   Text;                            // +0x60
    void*         Font;                            // +0x64 CFontBank*
    virtual void v00();
    CEngineInternalPrimitiveText(long type, unsigned long flags) : CEngineInternalPrimitiveBase(type, flags) { }
};

struct CEngineInternalPrimitive2DText : CEngineInternalPrimitiveText {   // vtable 0x012A67E4; 0x6c bytes
    long Layer;                                    // +0x68 EEngine2DLayer
    virtual void v00();
    CEngineInternalPrimitive2DText() : CEngineInternalPrimitiveText(0x27, 0x80000000) { }
};

class CEnginePrimitiveManagerText { public:
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);
};

CEngineInternalPrimitiveBase* CEnginePrimitiveManagerText::CreateInternalPrimitive(long type)
{
    switch (type) {
    case 5:
        return new CEngineInternalPrimitiveText(5, 0x2000000);
    case 0x27:
        return new CEngineInternalPrimitive2DText;
    }
    return 0;
}