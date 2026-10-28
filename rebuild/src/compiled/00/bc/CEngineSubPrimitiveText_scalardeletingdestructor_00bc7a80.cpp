// CEngineSubPrimitiveText::`scalar deleting destructor'(unsigned int) -- the implicit destructor (~CWideString on Text, ~CBaseClassNonCopyable), then the sub-primitive class operator delete on flag bit 0
// Leaf: ??_GCEngineSubPrimitiveText@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BC7A80, 38 bytes, thiscall, `ret 4`.
// Identity (proven): slot 0 of vtable 0x012A6858 (RTTI .?AVCEngineSubPrimitiveText@@ : CEngineSubPrimitiveBase :
//   CBaseClassNonCopyable : CBaseClass); Ego_r.pdb ??_GCEngineSubPrimitiveText@@UAEPAXI@Z. The delete call is
//   CEngineSubPrimitiveBase's static __fastcall class operator delete (FableWin.pdb ??3CEngineSubPrimitiveBase@@SIXPAX@Z):
//   retail 0x00BEB550, an empty `ret` (sub-primitives live in a frame pool), called with the pointer in ecx.
// Callees: CWideString::~CWideString 0x0099B510 (Text +0x2C), CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300 (protected virtual, Ego_r.pdb MAE),
//   CEngineSubPrimitiveBase::operator delete 0x00BEB550.
// Layout (FableWin/Ego_r.pdb names, retail offsets): CEngineSubPrimitiveBase {RenderFlags +4, Alpha +8, Parent +0x0C,
//   NextSubPrimitive +0x10, Manager +0x14}, Pos +0x18, Scale +0x24, Colour +0x28, Text +0x2C, Font +0x30, Mask +0x34.
// Shape: no user destructor in the chain (no vptr store); the vtable (and this ??_G) is emitted by the class's real
//   constructor, written as retail 0x00BC7A50 (base inline ctor: Parent, NextSubPrimitive = 0; Text default-constructed).
class CBaseClass {
public:
    CBaseClass();                                    // 0x0099A2D0
    virtual ~CBaseClass();                           // 0x0099A2E0
};
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                         // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                         // 0x0099A300 (protected virtual: Ego_r.pdb ??1CBaseClassNonCopyable@@MAE@XZ)
};
class CWideString {
public:
    CWideString();                                   // 0x0099AED0
    ~CWideString();                                  // 0x0099B510
    void* PStringData;
};
class C3DVector { public: float X, Y, Z; };
class CRGBColour { public: unsigned char B, G, R, A; };
class CFontBank;
class CTexture;
class CEnginePrimitiveManager;
class CEngineInternalPrimitiveBase { };
class CEngineInternalPrimitiveText : public CEngineInternalPrimitiveBase { };

class CEngineSubPrimitiveBase : public CBaseClassNonCopyable {
public:
    CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase* parent) : Parent(parent), NextSubPrimitive(0) { }
    static void __fastcall operator delete(void* p);         // 0x00BEB550
    unsigned long RenderFlags;                       // +0x04
    unsigned char Alpha;                             // +0x08
    CEngineInternalPrimitiveBase* Parent;            // +0x0C
    CEngineSubPrimitiveBase* NextSubPrimitive;       // +0x10
    CEnginePrimitiveManager* Manager;                // +0x14
};

class CEngineSubPrimitiveText : public CEngineSubPrimitiveBase {
public:
    CEngineSubPrimitiveText(CEngineInternalPrimitiveText* parent);
    C3DVector Pos;                                   // +0x18
    float Scale;                                     // +0x24
    CRGBColour Colour;                               // +0x28
    CWideString Text;                                // +0x2C
    CFontBank* Font;                                 // +0x30
    CTexture* Mask;                                  // +0x34
};

CEngineSubPrimitiveText::CEngineSubPrimitiveText(CEngineInternalPrimitiveText* parent)
    : CEngineSubPrimitiveBase(parent)
{
}