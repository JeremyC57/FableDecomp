// _STL::for_each(const CNavigationPosition* first, const CNavigationPosition* last, CSaveNavigationPositionToFile f) -- write every navigation position of a range to a file relative to the map origin (position - MapPos, layer: 12 bytes each), return the functor
// Leaf: ??$for_each@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A7E440, 105 bytes, `ret 10h`. /Gr algorithm TU: the hidden functor-result pointer
//   in ecx, first in edx; last and the 12-byte functor on the stack.
// Identity: Ego_r.pdb `??$_For_each@PBVCNavigationPosition@@VCSaveNavigationPositionToFile@@@std@@...` (the Dinkumware
//   spelling of the same algorithm); CSaveNavigationPositionToFile = {CAFile* File +0, C2DVector MapPos +4} and
//   CNavigationPosition = {C2DVector Position +0, long Layer +8} (Ego_r.pdb layouts). The catalog keeps the PDB name.
// Callees: CAFile vtable slot 4 Write(const void*, unsigned long size, bool) (+0x10; the landed 0x00A77D60 model).
// Shape: STLport `for (; first != last; ++first) f(*first); return f;` with operator() inline: a relative
//   CNavigationPosition local built by its (position, layer) constructor from `pos.Position - MapPos` (the layer dword
//   stored first, the two differences finish on the FPU stack: `fxch` then two `fstp`), written with size 12 and false;
//   the functor has a user constructor, so it is returned through the hidden pointer (three dwords copied).
class C2DVector {
public:
    float X;
    float Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
    C2DVector operator-(const C2DVector& v) const { return C2DVector(X - v.X, Y - v.Y); }
};

class CNavigationPosition {
public:
    C2DVector Position;                             // +0x00
    long      Layer;                                // +0x08
    CNavigationPosition(const C2DVector& position, long layer) : Position(position), Layer(layer) { }
};

class CAFile {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void Write(const void* data, unsigned long size, bool async);   // slot 4 (+0x10)
};

class CSaveNavigationPositionToFile {
public:
    CAFile*   File;                                 // +0x00
    C2DVector MapPos;                               // +0x04
    CSaveNavigationPositionToFile(CAFile* file, const C2DVector& mapPos) : File(file), MapPos(mapPos) { }
    void operator()(const CNavigationPosition& pos) const
    {
        CNavigationPosition relative(pos.Position - MapPos, pos.Layer);
        File->Write(&relative, sizeof(relative), false);
    }
};

namespace _STL {
template <class _InputIter, class _Function>
_Function __fastcall for_each(_InputIter __first, _InputIter __last, _Function __f)
{
    for (; __first != __last; ++__first)
        __f(*__first);
    return __f;
}
template CSaveNavigationPositionToFile __fastcall for_each(const CNavigationPosition*, const CNavigationPosition*, CSaveNavigationPositionToFile);
}