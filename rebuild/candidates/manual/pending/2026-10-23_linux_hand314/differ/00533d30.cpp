// NUISystem::CComponent::GetSize() -- the extent of the children's base-state boxes (only the listed shape children when ShapeChildren is not empty; centred children extend half their size each way, others from their base position by their size); zero when there are no children or no listed child exists
// Grade: DIFFER(1764v1750) -- 67 blocks: first pass; frame/register choice and the first-index search loop shape as the sibling 0x00531090, recomputed-getter compare order
// Leaf: GetSize
// Retail 0x00533D30, 1750 bytes, `ret 4` (hidden C2DVector return), `and esp,-8` aligned frame.
// Callees: vector<unsigned long> copy ctor 0x00535470; find 0x005348E0 (LTCG `__fastcall`); child virtuals GetStates
//   +0x148 (state map), GetSize +0x5c (C2DVector by hidden return), +0x104 (alignment mode), +0x1a8 (centred); state map
//   operator[] 0x0042D5B1; free 0x00BFEA14.
// Layout: local model -- the PDB CComponent has debug STL sizes; retail (12-byte STLport vectors): Children {start +0xb0,
//   finish +0xb4} of 8-byte CCountedPointer, ShapeChildren +0xe4; CUIState position +8 (this function's use).
extern "C" void __cdecl free(void* p);
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct SRandomTag { SRandomTag() { } };
struct CULongVector {
    unsigned long* Start;
    unsigned long* Finish;
    unsigned long* End;
    CULongVector(const CULongVector& other);           // 0x00535470
    ~CULongVector()
    {
        if (Start)
            free(Start);
    }
};
unsigned long* __fastcall Find(unsigned long* first, unsigned long* last, const unsigned long& value,
                               const SRandomTag& tag);  // 0x005348E0
namespace NUISystem {
struct CUIState {
    unsigned char _pad00[8];
    C2DVector     Position;                            // +0x08
};
struct CUIStateMap {
    CUIState& operator[](const unsigned long& key);    // 0x0042D5B1
};
class CComponent {
public:
    virtual void c00();
    virtual void c01();
    virtual void c02();
    virtual void c03();
    virtual void c04();
    virtual void c05();
    virtual void c06();
    virtual void c07();
    virtual void c08();
    virtual void c09();
    virtual void c0a();
    virtual void c0b();
    virtual void c0c();
    virtual void c0d();
    virtual void c0e();
    virtual void c0f();
    virtual void c10();
    virtual void c11();
    virtual void c12();
    virtual void c13();
    virtual void c14();
    virtual void c15();
    virtual void c16();
    virtual C2DVector GetSize();
    virtual void c18();
    virtual void c19();
    virtual void c1a();
    virtual void c1b();
    virtual void c1c();
    virtual void c1d();
    virtual void c1e();
    virtual void c1f();
    virtual void c20();
    virtual void c21();
    virtual void c22();
    virtual void c23();
    virtual void c24();
    virtual void c25();
    virtual void c26();
    virtual void c27();
    virtual void c28();
    virtual void c29();
    virtual void c2a();
    virtual void c2b();
    virtual void c2c();
    virtual void c2d();
    virtual void c2e();
    virtual void c2f();
    virtual void c30();
    virtual void c31();
    virtual void c32();
    virtual void c33();
    virtual void c34();
    virtual void c35();
    virtual void c36();
    virtual void c37();
    virtual void c38();
    virtual void c39();
    virtual void c3a();
    virtual void c3b();
    virtual void c3c();
    virtual void c3d();
    virtual void c3e();
    virtual void c3f();
    virtual void c40();
    virtual long GetAlignment();
    virtual void c42();
    virtual void c43();
    virtual void c44();
    virtual void c45();
    virtual void c46();
    virtual void c47();
    virtual void c48();
    virtual void c49();
    virtual void c4a();
    virtual void c4b();
    virtual void c4c();
    virtual void c4d();
    virtual void c4e();
    virtual void c4f();
    virtual void c50();
    virtual void c51();
    virtual CUIStateMap& GetStates();
    virtual void c53();
    virtual void c54();
    virtual void c55();
    virtual void c56();
    virtual void c57();
    virtual void c58();
    virtual void c59();
    virtual void c5a();
    virtual void c5b();
    virtual void c5c();
    virtual void c5d();
    virtual void c5e();
    virtual void c5f();
    virtual void c60();
    virtual void c61();
    virtual void c62();
    virtual void c63();
    virtual void c64();
    virtual void c65();
    virtual void c66();
    virtual void c67();
    virtual void c68();
    virtual void c69();
    virtual bool IsCentred();
    struct SChild { CComponent* Data; void* Info; };
    unsigned char _pad04[0xb0 - 0x04];
    SChild*       ChildrenStart;                       // +0xb0
    SChild*       ChildrenFinish;                      // +0xb4
    SChild*       ChildrenEnd;                         // +0xb8
    unsigned char _padbc[0xe4 - 0xbc];
    CULongVector  ShapeChildren;                       // +0xe4
    __forceinline CUIState& BaseState() { return GetStates()[0]; }
};

C2DVector CComponent::GetSize()
{
    if (ChildrenStart == ChildrenFinish)
        return C2DVector(0, 0);
    CULongVector shapes(ShapeChildren);
    long first = -1;
    if (shapes.Start == shapes.Finish)
        first = 0;
    for (unsigned long i = 0, count = ChildrenFinish - ChildrenStart; i < count; ++i) {
        if (shapes.Start == shapes.Finish)
            break;
        if (Find(shapes.Start, shapes.Finish, i, SRandomTag()) != shapes.Finish) {
            first = i;
            break;
        }
    }
    if (first == -1)
        return C2DVector(0, 0);
    C2DVector minimum(ChildrenStart[first].Data->BaseState().Position.X, ChildrenStart[first].Data->BaseState().Position.Y);
    C2DVector size = ChildrenStart[first].Data->GetSize();
    C2DVector maximum(minimum.X + size.X, minimum.Y + size.Y);
    for (unsigned long i = 0; i < (unsigned long)(ChildrenFinish - ChildrenStart); ++i) {
        if (shapes.Start != shapes.Finish && Find(shapes.Start, shapes.Finish, i, SRandomTag()) == shapes.Finish)
            continue;
        if ((long)i == first)
            continue;
        if (ChildrenStart[i].Data->GetAlignment() == 0 && ChildrenStart[i].Data->IsCentred()) {
            CComponent* child = ChildrenStart[i].Data;
            if (child->BaseState().Position.X - child->GetSize().X * 0.5f < minimum.X) {
                child = ChildrenStart[i].Data;
                minimum.X = child->BaseState().Position.X - child->GetSize().X * 0.5f;
            }
            child = ChildrenStart[i].Data;
            if (child->BaseState().Position.Y - child->GetSize().Y * 0.5f < minimum.Y) {
                child = ChildrenStart[i].Data;
                minimum.Y = child->BaseState().Position.Y - child->GetSize().Y * 0.5f;
            }
        } else {
            if (ChildrenStart[i].Data->BaseState().Position.X < minimum.X)
                minimum.X = ChildrenStart[i].Data->BaseState().Position.X;
            if (ChildrenStart[i].Data->BaseState().Position.Y < minimum.Y)
                minimum.Y = ChildrenStart[i].Data->BaseState().Position.Y;
        }
        if (ChildrenStart[i].Data->GetAlignment() == 0 && ChildrenStart[i].Data->IsCentred()) {
            CComponent* child = ChildrenStart[i].Data;
            if (child->GetSize().X * 0.5f + child->BaseState().Position.X > maximum.X) {
                child = ChildrenStart[i].Data;
                maximum.X = child->GetSize().X * 0.5f + child->BaseState().Position.X;
            }
            child = ChildrenStart[i].Data;
            if (child->GetSize().Y * 0.5f + child->BaseState().Position.Y > maximum.Y) {
                child = ChildrenStart[i].Data;
                maximum.Y = child->GetSize().Y * 0.5f + child->BaseState().Position.Y;
            }
        } else {
            CComponent* child = ChildrenStart[i].Data;
            if (child->BaseState().Position.X + child->GetSize().X > maximum.X) {
                child = ChildrenStart[i].Data;
                maximum.X = child->BaseState().Position.X + child->GetSize().X;
            }
            child = ChildrenStart[i].Data;
            if (child->BaseState().Position.Y + child->GetSize().Y > maximum.Y) {
                child = ChildrenStart[i].Data;
                maximum.Y = child->BaseState().Position.Y + child->GetSize().Y;
            }
        }
    }
    return C2DVector(maximum.X - minimum.X, maximum.Y - minimum.Y);
}
}
