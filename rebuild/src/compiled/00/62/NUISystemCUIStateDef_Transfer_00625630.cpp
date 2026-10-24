// NUISystem::CUIStateDef::Transfer(CPersistContext& context) -- persist one UI state definition: graphic index, position, zoom, colour, update time, change type, linear flag, change flag and the list of children it leaves alone
// Leaf: ?Transfer@CUIStateDef@NUISystem
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00625630, 167 bytes (+ int3 padding), virtual, `ret 4`.
// Identity: vtable slot 18 of NUISystem::CUIStateDef (vtable 0x0125871C, RTTI `.?AVCUIStateDef@NUISystem@@`), the
// def-class Transfer slot; the catalog label "InitialiseDefaultCameraTags" is wrong.
// Callees (typed CPersistContext transfers; the catalog labels on them are propagated): unsigned long 0x00431020,
// float 0x00431061 (x9), EStateChangeType 0x006256E0 (the enum template, emitted in this module), bool 0x0043314A,
// vector<unsigned long> 0x00464841.
// Layout: Ego_r.pdb CUIStateDef (0x84) re-based on the retail bytes: the def base is 0x3c bytes (PDB 0x40) and the
// STLport vector is 12 bytes, so GraphicIndex sits at +0x3c and LinearChange at +0x78. No generated engine header.
// Shape: speed TU (address registers rotate eax/ecx/edx, the context reloaded into ecx from edi before every call).
// Members are transferred in declaration order except StateChangeType, LinearChange and StateChangeFlag, which retail
// transfers after UpdateTime (retail order 0x3c, 0x48..0x68, 0x44, 0x78, 0x40, 0x6c).
struct C2DVector { float X; float Y; };
struct CRGBFloatColour { float R; float G; float B; float A; };
struct CULongVector { unsigned long* _M_start; unsigned long* _M_finish; unsigned long* _M_end_of_storage; };

namespace NUISystem {
enum EStateChangeType { STATE_CHANGE_TYPE_NONE };
}

struct CPersistContext {
    void Transfer(unsigned long& value);                  // 0x00431020
    void Transfer(float& value);                          // 0x00431061
    void Transfer(NUISystem::EStateChangeType& value);    // 0x006256E0
    void Transfer(bool& value);                           // 0x0043314A
    void Transfer(CULongVector& value);                   // 0x00464841
};

struct CDefClassBase {
    virtual void Transfer(CPersistContext& context);
    unsigned char _body[0x3c - 4];
};

namespace NUISystem {

struct CUIStateDef : CDefClassBase {
    unsigned long    GraphicIndex;                  // +0x3c
    unsigned long    StateChangeFlag;               // +0x40
    EStateChangeType StateChangeType;               // +0x44
    C2DVector        Position;                      // +0x48
    C2DVector        Zoom;                          // +0x50
    CRGBFloatColour  Colour;                        // +0x58
    float            UpdateTime;                    // +0x68
    CULongVector     ChildrenNotAffected;           // +0x6c
    bool             LinearChange;                  // +0x78
    virtual void Transfer(CPersistContext& context);
};

void CUIStateDef::Transfer(CPersistContext& context)
{
    context.Transfer(GraphicIndex);
    context.Transfer(Position.X);
    context.Transfer(Position.Y);
    context.Transfer(Zoom.X);
    context.Transfer(Zoom.Y);
    context.Transfer(Colour.R);
    context.Transfer(Colour.G);
    context.Transfer(Colour.B);
    context.Transfer(Colour.A);
    context.Transfer(UpdateTime);
    context.Transfer(StateChangeType);
    context.Transfer(LinearChange);
    context.Transfer(StateChangeFlag);
    context.Transfer(ChildrenNotAffected);
}

}