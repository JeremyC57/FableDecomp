// NUISystem::CComponent::Initialise() -- seed the component from its state-0 record (position, zoom and colour into the current and target copies, then the initial copies), reset the parent position/zoom/colour unless a position parent is set, clear the elapsed timers, then attach and initialise every child that has no parent yet
// Retail: Fable.exe (TLC, VC7.1) @ 0x005339B0, 369 bytes (384 with padding). Vtable 0x0124608C slot 43.
// Callees: the STLport map<unsigned long, NUISystem::CUIState>::operator[] 0x0042D5B1 (out of line); child
//          vtable slots 43 (+0xac, Initialise), 51 (+0xcc, SetParent) and 52 (+0xd0, GetParent).
//
// Shape: each `States[0]` builds its own zero key in the frame slot (`mov [esp+10h],edi` before each call);
// a state's vector is copied into two members through one `const C2DVector&` (`add eax,8` then relative
// reads) while the 4-byte colour goes through the chain `TargetColour = Colour = ...` (one load, two stores);
// ParentColour's channels are one chain (A, B, G, R store order) and the zooms `X = Y = 1.0f`. The two
// `PPositionParent == 0` tests compare memory twice only because the zeroed positions are cleared through an
// inline C2DVector member (`Clear()`): with plain member stores VC7.1 keeps the pointer in eax.
// Layout: local model. NUISystem::CComponent has no engine header; the PDB layout (fable_types.h, 0x15c bytes)
// is retail +8 from Time on (CUIState map 12 bytes at +0x24 in retail), +0x10 after the two 12-byte child
// vectors and +0x24 after the 8-byte CDeletion/list/vector run: Time +0x30, Position +0x34 .. RenderZoom
// +0x7c, Colour +0x84 .. RenderColour +0x94, timers +0x98..+0xac, Children +0xb0, ChildrenToDelete +0xbc,
// PParent +0xc8, Size +0xcc, RelativeParentPosition +0x100, RelativeParentZoom +0x110, PPositionParent +0x118.
namespace NUISystem {

struct C2DVector {
    float X;
    float Y;
    C2DVector() {}
    void Clear() { X = 0.0f; Y = 0.0f; }
};
struct CRGBColour {                         // D3DCOLOR byte order
    unsigned char B, G, R, A;
};
class CUIState { public:                           // PDB 0x24 bytes
    unsigned long StateChangeFlag;          // +0x00
    long          StateChangeType;          // +0x04 EStateChangeType
    C2DVector     Position;                 // +0x08
    C2DVector     Zoom;                     // +0x10
    CRGBColour    Colour;                   // +0x18
    float         UpdateTime;               // +0x1c
    bool          LinearChange;             // +0x20
};
struct CUIStateMap {                        // map<unsigned long, CUIState> (STLport, 12 bytes)
    void*         m_pHeader;
    unsigned long m_nodeCount;
    unsigned char m_compare;
    CUIState& operator[](const unsigned long& key);   // 0x0042D5B1
};
class CComponent;
struct CCountedComponentPointer {           // CCountedPointer<NUISystem::CComponent>
    CComponent* Object;
    void*       Info;
    CComponent* operator->() const { return Object; }
};
struct CComponentPointerVector {            // STLport vector<CCountedPointer<CComponent>> (12 bytes)
    CCountedComponentPointer* _M_start;
    CCountedComponentPointer* _M_finish;
    CCountedComponentPointer* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    CCountedComponentPointer& operator[](unsigned int i) { return _M_start[i]; }
};
class CComponent {
public:
    virtual void VSlot0();
    virtual void VSlot1();
    virtual void VSlot2();
    virtual void VSlot3();
    virtual void VSlot4();
    virtual void VSlot5();
    virtual void VSlot6();
    virtual void VSlot7();
    virtual void VSlot8();
    virtual void VSlot9();
    virtual void VSlot10();
    virtual void VSlot11();
    virtual void VSlot12();
    virtual void VSlot13();
    virtual void VSlot14();
    virtual void VSlot15();
    virtual void VSlot16();
    virtual void VSlot17();
    virtual void VSlot18();
    virtual void VSlot19();
    virtual void VSlot20();
    virtual void VSlot21();
    virtual void VSlot22();
    virtual void VSlot23();
    virtual void VSlot24();
    virtual void VSlot25();
    virtual void VSlot26();
    virtual void VSlot27();
    virtual void VSlot28();
    virtual void VSlot29();
    virtual void VSlot30();
    virtual void VSlot31();
    virtual void VSlot32();
    virtual void VSlot33();
    virtual void VSlot34();
    virtual void VSlot35();
    virtual void VSlot36();
    virtual void VSlot37();
    virtual void VSlot38();
    virtual void VSlot39();
    virtual void VSlot40();
    virtual void VSlot41();
    virtual void VSlot42();
    virtual void Initialise();                                      // slot 43 (+0xac)
    virtual void VSlot44();
    virtual void VSlot45();
    virtual void VSlot46();
    virtual void VSlot47();
    virtual void VSlot48();
    virtual void VSlot49();
    virtual void VSlot50();
    virtual void SetParent(CComponent* parent);                     // slot 51 (+0xcc)
    virtual CComponent* GetParent();                                // slot 52 (+0xd0)
    unsigned char _pad04[0x20];             // +0x04
    CUIStateMap   States;                   // +0x24
    float         Time;                     // +0x30
    C2DVector     Position;                 // +0x34
    C2DVector     TargetPosition;           // +0x3c
    C2DVector     InitialPosition;          // +0x44
    C2DVector     ParentPosition;           // +0x4c
    C2DVector     RenderPosition;           // +0x54
    C2DVector     Zoom;                     // +0x5c
    C2DVector     TargetZoom;               // +0x64
    C2DVector     InitialZoom;              // +0x6c
    C2DVector     ParentZoom;               // +0x74
    C2DVector     RenderZoom;               // +0x7c
    CRGBColour    Colour;                   // +0x84
    CRGBColour    TargetColour;             // +0x88
    CRGBColour    InitialColour;            // +0x8c
    CRGBColour    ParentColour;             // +0x90
    CRGBColour    RenderColour;             // +0x94
    float         PositionTimeElapsed;      // +0x98
    float         PositionTime;             // +0x9c
    float         ZoomTimeElapsed;          // +0xa0
    float         ZoomTime;                 // +0xa4
    float         ColourTimeElapsed;        // +0xa8
    float         ColourTime;               // +0xac
    CComponentPointerVector Children;       // +0xb0
    CComponentPointerVector ChildrenToDelete;   // +0xbc
    CComponent*   PParent;                  // +0xc8
    C2DVector     Size;                     // +0xcc
    unsigned char _padd4[0x2c];             // +0xd4 Deletion, PParentViewport, ActionsIAmParamOf, ShapeChildren,
                                            //       RenderPositionSave, RelativeRenderPosition
    C2DVector     RelativeParentPosition;   // +0x100
    C2DVector     RelativeRenderZoom;       // +0x108
    C2DVector     RelativeParentZoom;       // +0x110
    CComponent*   PPositionParent;          // +0x118
    unsigned char _pad11c[0x18];            // +0x11c PositionChildren, PActionsOnDestruction, Flags .. DefIndex
};

void CComponent::Initialise()
{
    {
        const C2DVector& position = States[0].Position;
        Position = position;
        TargetPosition = position;
    }
    {
        const C2DVector& zoom = States[0].Zoom;
        Zoom = zoom;
        TargetZoom = zoom;
    }
    TargetColour = Colour = States[0].Colour;
    if (PPositionParent == 0) {
        ParentPosition.Clear();
        RelativeParentPosition.Clear();
    }
    ParentColour.R = ParentColour.G = ParentColour.B = ParentColour.A = 0xff;
    ParentZoom.X = ParentZoom.Y = 1.0f;
    if (PPositionParent == 0) {
        RelativeParentZoom.X = RelativeParentZoom.Y = 1.0f;
    }
    InitialPosition = Position;
    InitialZoom = Zoom;
    InitialColour = Colour;
    ZoomTimeElapsed = 0.0f;
    PositionTimeElapsed = 0.0f;
    ColourTimeElapsed = 0.0f;
    Time = 0.0f;
    for (unsigned int i = 0; i < Children.size(); ++i) {
        if (Children[i]->GetParent() == 0) {
            Children[i]->SetParent(this);
            Children[i]->Initialise();
        }
    }
}

}