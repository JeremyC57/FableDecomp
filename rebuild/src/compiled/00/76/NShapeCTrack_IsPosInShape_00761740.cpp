// NShape::CTrack::IsPosInShape(const C3DVector& pos) const -- a position is on the track when its distance to the nearest track segment is below 0.25
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00761740, 54 bytes (row padded to 64), `ret 4`. Virtual slot 4 of
// NShape::CTrack's vtable 0x0126A288; FableWin.pdb lists ?IsPosInShape@CTrack@NShape@@UBE_NABVC3DVector@@@Z.
// Callees: CTrack::GetMinDistanceToTrack(const C3DVector&, float&) const @ 0x00760E70 (thiscall, `ret 8`, bool: false when the
// track has fewer than two points); the 0.25f literal at 0x0126A1FC.
// Shape: the distance local is homed in the dead `pos` argument slot (`lea eax,[esp+4]`); the miss path is the `else` arm
// (its own `xor al,al` exit placed last; `if (!Get()) return false;` reuses the call's zero al instead), the compare `return distance < 0.25f;` materialises 1/0 in eax on the two `fcomp ; test ah,5 ; jp` arms.
struct C3DVector { float X, Y, Z; };

namespace NShape {
struct CShape {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual bool IsPosInShape(const C3DVector& pos) const;   // slot 4
};
struct CTrack : CShape {
    bool GetMinDistanceToTrack(const C3DVector& pos, float& distance) const;   // 0x00760E70
    virtual bool IsPosInShape(const C3DVector& pos) const;
};

bool CTrack::IsPosInShape(const C3DVector& pos) const
{
    float distance;
    if (GetMinDistanceToTrack(pos, distance))
        return distance < 0.25f;
    else
        return false;
}
}