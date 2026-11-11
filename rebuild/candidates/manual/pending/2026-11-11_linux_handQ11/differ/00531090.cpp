// NUISystem::CComponent::GetBoundingBox() -- the union of the children's boxes (only the listed shape children when ShapeChildren is not empty; centred children extend half their size each way, others from their position by their size); a zero box when there are no children or no listed child exists
// Grade: DIFFER(1216v1227) -- 73 blocks: first pass; aligned frame reproduced, frame 0xb4 v 0xa4, first-index search loop shape (retail caches the child count, keeps first in edi and a frame copy), register choice
// Leaf: GetBoundingBox
// Retail 0x00531090, 1232 bytes, `ret 4` (hidden C2DBoxF return), `and esp,-8` aligned frame.
// Callees: vector<unsigned long> copy ctor 0x00535470; find 0x005348E0 (LTCG `__fastcall`); child virtuals GetPosition
//   +0x40 / GetSize +0x1bc (C2DVector by hidden return), +0x104 (alignment mode), +0x1a8 (centred); free 0x00BFEA14.
// Layout: local model -- the PDB CComponent has debug STL sizes; retail (12-byte STLport vectors): Children {start +0xb0,
//   finish +0xb4} of 8-byte CCountedPointer, ShapeChildren +0xe4 (this function's use).
extern "C" void __cdecl free(void* p);
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct C2DBoxF {
    C2DVector Min, Max;
    C2DBoxF(float x0, float y0, float x1, float y1) : Min(x0, y0), Max(x1, y1) { }
};
template<class T> __forceinline const T& Min2(const T& a, const T& b) { return b < a ? b : a; }
template<class T> __forceinline const T& Max2(const T& a, const T& b) { return a < b ? b : a; }
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
    virtual C2DVector GetPosition();
    virtual void c11();
    virtual void c12();
    virtual void c13();
    virtual void c14();
    virtual void c15();
    virtual void c16();
    virtual void c17();
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
    virtual void c52();
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
    virtual void c6b();
    virtual void c6c();
    virtual void c6d();
    virtual void c6e();
    virtual C2DVector GetSize();
    struct SChild { CComponent* Data; void* Info; };
    unsigned char _pad04[0xb0 - 0x04];
    SChild*       ChildrenStart;                       // +0xb0
    SChild*       ChildrenFinish;                      // +0xb4
    SChild*       ChildrenEnd;                         // +0xb8
    unsigned char _padbc[0xe4 - 0xbc];
    CULongVector  ShapeChildren;                       // +0xe4
    C2DBoxF GetBoundingBox();
};

C2DBoxF CComponent::GetBoundingBox()
{
    if (ChildrenStart == ChildrenFinish)
        return C2DBoxF(0, 0, 0, 0);
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
        return C2DBoxF(0, 0, 0, 0);
    C2DVector minimum = ChildrenStart[first].Data->GetPosition();
    C2DVector size = ChildrenStart[first].Data->GetSize();
    C2DVector maximum(minimum.X + size.X, minimum.Y + size.Y);
    for (unsigned long i = 0; i < (unsigned long)(ChildrenFinish - ChildrenStart); ++i) {
        if (shapes.Start != shapes.Finish && Find(shapes.Start, shapes.Finish, i, SRandomTag()) == shapes.Finish)
            continue;
        if ((long)i == first)
            continue;
        if (ChildrenStart[i].Data->GetAlignment() == 0 && ChildrenStart[i].Data->IsCentred()) {
            CComponent* child = ChildrenStart[i].Data;
            minimum.X = Min2(child->GetPosition().X - child->GetSize().X * 0.5f, minimum.X);
            child = ChildrenStart[i].Data;
            minimum.Y = Min2(child->GetPosition().Y - child->GetSize().Y * 0.5f, minimum.Y);
        } else {
            minimum.X = Min2(ChildrenStart[i].Data->GetPosition().X, minimum.X);
            minimum.Y = Min2(ChildrenStart[i].Data->GetPosition().Y, minimum.Y);
        }
        if (ChildrenStart[i].Data->GetAlignment() == 0 && ChildrenStart[i].Data->IsCentred()) {
            CComponent* child = ChildrenStart[i].Data;
            maximum.X = Max2(maximum.X, child->GetSize().X * 0.5f + child->GetPosition().X);
            child = ChildrenStart[i].Data;
            maximum.Y = Max2(maximum.Y, child->GetSize().Y * 0.5f + child->GetPosition().Y);
        } else {
            CComponent* child = ChildrenStart[i].Data;
            maximum.X = Max2(maximum.X, child->GetSize().X + child->GetPosition().X);
            child = ChildrenStart[i].Data;
            maximum.Y = Max2(maximum.Y, child->GetSize().Y + child->GetPosition().Y);
        }
    }
    return C2DBoxF(minimum.X, minimum.Y, maximum.X, maximum.Y);
}
}
