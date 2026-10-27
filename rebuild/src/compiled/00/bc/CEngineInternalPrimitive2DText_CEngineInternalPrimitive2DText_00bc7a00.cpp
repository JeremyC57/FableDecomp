// CEngineInternalPrimitive2DText::~CEngineInternalPrimitive2DText() -- destroy the CWideString member, then the CIVCountedPointeeBase base (which stores its own vtable)
// Leaf: ??1CEngineInternalPrimitive2DText@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BC7A00, 19 bytes, thiscall, plain `ret`.
// Identity (proven): its caller 0x00BC7AE0 is slot 0 (the scalar deleting destructor) of vtable 0x012A67E4 (RTTI .?AVCEngineInternalPrimitive2DText@@); it destroys the CWideString at +0x60 and stores the root CIVCountedPointeeBase vtable 0x012354D0.
// Callees: the member destructor 0x0099B510.
// Shape (family M034, representative 0x00453E1A): an empty destructor of a `__declspec(novtable)` class (no own vtable store,
//   as in retail) over a base whose inline destructor stores the base vtable; the member destructor runs first.
// Header debt: the class chain is modelled locally (no engine/ header carries the destructors).
class CWideString {
public:
    ~CWideString();                                       // 0x0099B510
    void* PStringData;
};
enum EEngine2DLayer { ENGINE_2D_LAYER_NONE = 0 };
class CIVCountedPointeeBase {                             // Ego_r.pdb: vptr + IVRefCount
public:
    virtual ~CIVCountedPointeeBase() { }
    unsigned long IVRefCount;
};
class __declspec(novtable) CEngineInternalPrimitive2DText : public CIVCountedPointeeBase {
public:
    virtual ~CEngineInternalPrimitive2DText();
    unsigned char _primitive[0x58];                       // +0x08 the 2D primitive bases' members (no destructors)
    CWideString Text;                                     // +0x60
    unsigned long _text_state;                            // +0x64
    EEngine2DLayer Layer;                                 // +0x68 (Ego_r.pdb 104)
};
CEngineInternalPrimitive2DText::~CEngineInternalPrimitive2DText()
{
}