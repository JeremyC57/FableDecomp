// NUISystem::CComponent::AddPositionChild(CComponent* child) -- add the child to the position-children set, make this its position parent, hand it the render zooms, then its parent position and relative parent position (each either this component's own position or the position scaled by the parent zoom, converted to screen space for a screen-converted component, and offset by this component's own parent positions unless it is the relative root)
// Leaf: AddPositionChild
// Retail: Fable.exe (TLC, VC7.1) @ 0x00531BA0, 682 bytes (688 with padding), virtual, `ret 4`, 8-byte aligned frame.
// Callees: the position-children set's insert_unique 0x00535240 (pair result by hidden pointer), the screen conversion
//   `__fastcall` C2DVector ConvertToScreen(C2DVector) 0x0052E580, and the component's virtual slots 18 SetParentPosition
//   (+0x48, writes +0x4c), 20 SetParentZoom (+0x50, writes +0x74), 102 / 103 (+0x198 / +0x19c, both forward to slot 101),
//   114 SetRelativeParentPosition (+0x1c8, writes +0x100), 115 SetRelativeParentZoom (+0x1cc, writes +0x110), 117
//   (+0x1d4, FlagsSetTwo bit 7) and 125 SetPositionParent (+0x1f4, writes +0x118). Slot names are descriptive.
// Layout: local model -- the retail NUISystem::CComponent layout of the sibling CComponent(long) 0x005334A0 (no engine
//   header): Position +0x34, ParentPosition +0x4c, ParentZoom +0x74, RenderZoom +0x7c, RelativeParentPosition +0x100,
//   RelativeRenderZoom +0x108, RelativeParentZoom +0x110, PositionChildren +0x11c.
// Forms: a scaled position is a copy then in-place `*=` (`fld Y; fld X; fmul; fstp; fmul`), an offset is a further
//   in-place `+=`; the two screen-converted results without an offset are named locals (that is what gives the
//   `and esp,-8` frame and the one shared temporary slot); the unscaled offsets use the member operator+ and an explicit
//   ctor, whose operand order reproduces the retail fadd order.
struct C2DVector {
    float X;
    float Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {}
    C2DVector& operator*=(const C2DVector& o) { X *= o.X; Y *= o.Y; return *this; }
    C2DVector& operator+=(const C2DVector& o) { X += o.X; Y += o.Y; return *this; }
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
};
C2DVector __fastcall ConvertToScreen(C2DVector position);   // 0x0052E580

namespace NUISystem {
class CComponent;
struct CComponentSetIterator { void* _M_node; };
struct CComponentSetInsertResult { CComponentSetIterator first; bool second; };
struct CComponentSet {                                      // std::set<CComponent*> (STLport, 12 bytes)
    void* _M_header;
    unsigned long _M_node_count;
    unsigned long _M_key_compare;
    CComponentSetInsertResult insert_unique(CComponent* const& value);   // 0x00535240
    CComponentSetInsertResult insert(CComponent* const& value) { return insert_unique(value); }
};
void* InsertNode_CComponentSet(CComponentSet* set, unsigned long key);   // stand-in callee of the body below
#pragma auto_inline(off)
// Same-TU stand-in for the out-of-line _Rb_tree::insert_unique 0x00535240: it reads the key only, so VC7.1 knows the
// argument's address does not escape (the caller keeps the child pointer in a register afterwards, as retail).
__declspec(noinline) CComponentSetInsertResult CComponentSet::insert_unique(CComponent* const& value)
{
    CComponentSetInsertResult result;
    result.first._M_node = InsertNode_CComponentSet(this, (unsigned long)value);
    result.second = result.first._M_node != 0;
    return result;
}
#pragma auto_inline(on)

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
    virtual void SetParentPosition(const C2DVector& position);      // slot 18 (+0x48)
    virtual void VSlot19();
    virtual void SetParentZoom(const C2DVector& zoom);              // slot 20 (+0x50)
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
    virtual void VSlot43();
    virtual void VSlot44();
    virtual void VSlot45();
    virtual void VSlot46();
    virtual void VSlot47();
    virtual void VSlot48();
    virtual void VSlot49();
    virtual void VSlot50();
    virtual void VSlot51();
    virtual void VSlot52();
    virtual void VSlot53();
    virtual void VSlot54();
    virtual void VSlot55();
    virtual void VSlot56();
    virtual void VSlot57();
    virtual void VSlot58();
    virtual void VSlot59();
    virtual void VSlot60();
    virtual void VSlot61();
    virtual void VSlot62();
    virtual void VSlot63();
    virtual void VSlot64();
    virtual void VSlot65();
    virtual void VSlot66();
    virtual void VSlot67();
    virtual void VSlot68();
    virtual void VSlot69();
    virtual void VSlot70();
    virtual void VSlot71();
    virtual void VSlot72();
    virtual void VSlot73();
    virtual void VSlot74();
    virtual void VSlot75();
    virtual void VSlot76();
    virtual void VSlot77();
    virtual void VSlot78();
    virtual void VSlot79();
    virtual void VSlot80();
    virtual void VSlot81();
    virtual void VSlot82();
    virtual void VSlot83();
    virtual void VSlot84();
    virtual void VSlot85();
    virtual void VSlot86();
    virtual void VSlot87();
    virtual void VSlot88();
    virtual void VSlot89();
    virtual void VSlot90();
    virtual void VSlot91();
    virtual void VSlot92();
    virtual void VSlot93();
    virtual void VSlot94();
    virtual void VSlot95();
    virtual void VSlot96();
    virtual void VSlot97();
    virtual void VSlot98();
    virtual void VSlot99();
    virtual void VSlot100();
    virtual void VSlot101();
    virtual bool IsPositionParentRelative() const;                  // slot 102 (+0x198)
    virtual bool IsZoomIgnored() const;                             // slot 103 (+0x19c)
    virtual void VSlot104();
    virtual void VSlot105();
    virtual void VSlot106();
    virtual void VSlot107();
    virtual void VSlot108();
    virtual void VSlot109();
    virtual void VSlot110();
    virtual void VSlot111();
    virtual void VSlot112();
    virtual void VSlot113();
    virtual void SetRelativeParentPosition(const C2DVector& position);   // slot 114 (+0x1c8)
    virtual void SetRelativeParentZoom(const C2DVector& zoom);      // slot 115 (+0x1cc)
    virtual void VSlot116();
    virtual bool IsScreenConverted() const;                         // slot 117 (+0x1d4)
    virtual void VSlot118();
    virtual void VSlot119();
    virtual void VSlot120();
    virtual void VSlot121();
    virtual void VSlot122();
    virtual void VSlot123();
    virtual void VSlot124();
    virtual void SetPositionParent(CComponent* parent);             // slot 125 (+0x1f4)
    unsigned char _pad04[0x34 - 0x04];                      // CObserver / CActionParam bases, Name, States, Time
    C2DVector   Position;                                   // +0x34
    C2DVector   TargetPosition;                             // +0x3c
    C2DVector   InitialPosition;                            // +0x44
    C2DVector   ParentPosition;                             // +0x4c
    C2DVector   RenderPosition;                             // +0x54
    C2DVector   Zoom;                                       // +0x5c
    C2DVector   TargetZoom;                                 // +0x64
    C2DVector   InitialZoom;                                // +0x6c
    C2DVector   ParentZoom;                                 // +0x74
    C2DVector   RenderZoom;                                 // +0x7c
    unsigned char _pad84[0xf0 - 0x84];                      // colours, timers, children, parents, deletion lists
    C2DVector   RenderPositionSave;                         // +0xf0
    C2DVector   RelativeRenderPosition;                     // +0xf8
    C2DVector   RelativeParentPosition;                     // +0x100
    C2DVector   RelativeRenderZoom;                         // +0x108
    C2DVector   RelativeParentZoom;                         // +0x110
    CComponent* PPositionParent;                            // +0x118
    CComponentSet PositionChildren;                         // +0x11c
    unsigned char _pad128[0x134 - 0x128];
    void AddPositionChild(CComponent* child);
};

void CComponent::AddPositionChild(CComponent* child)
{
    PositionChildren.insert(child);
    child->SetPositionParent(this);
    child->SetParentZoom(RenderZoom);
    child->SetRelativeParentZoom(RelativeRenderZoom);
    if (IsPositionParentRelative()) {
        if (IsZoomIgnored()) {
            child->SetParentPosition(Position);
        } else {
            C2DVector position = Position;
            position *= ParentZoom;
            child->SetParentPosition(position);
        }
        if (IsScreenConverted()) {
            if (IsZoomIgnored()) {
                C2DVector screen = ConvertToScreen(Position);
                child->SetRelativeParentPosition(screen);
            } else {
                C2DVector screen = ConvertToScreen(Position);
                screen *= RelativeParentZoom;
                child->SetRelativeParentPosition(screen);
            }
        } else {
            if (IsZoomIgnored()) {
                child->SetRelativeParentPosition(Position);
            } else {
                C2DVector position = Position;
                position *= RelativeParentZoom;
                child->SetRelativeParentPosition(position);
            }
        }
    } else {
        if (IsZoomIgnored()) {
            child->SetParentPosition(ParentPosition + Position);
        } else {
            C2DVector position = Position;
            position *= ParentZoom;
            position += ParentPosition;
            child->SetParentPosition(position);
        }
        if (IsScreenConverted()) {
            if (IsZoomIgnored()) {
                child->SetRelativeParentPosition(ConvertToScreen(Position) + RelativeParentPosition);
            } else {
                C2DVector screen = ConvertToScreen(Position);
                screen *= RelativeParentZoom;
                screen += RelativeParentPosition;
                child->SetRelativeParentPosition(screen);
            }
        } else {
            if (IsZoomIgnored()) {
                child->SetRelativeParentPosition(C2DVector(RelativeParentPosition.X + Position.X, RelativeParentPosition.Y + Position.Y));
            } else {
                C2DVector position = Position;
                position *= RelativeParentZoom;
                position += RelativeParentPosition;
                child->SetRelativeParentPosition(position);
            }
        }
    }
}
}