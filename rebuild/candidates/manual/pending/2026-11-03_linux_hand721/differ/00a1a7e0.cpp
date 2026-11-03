// CNavigatorAStar::GetIntersectionPointsBetweenNodes(const CANavQuadTreeNode& from, const CANavQuadTreeNode& to, float distance) const
// Leaf: ?GetIntersectionPointsBetweenNodes@CNavigatorAStar@@ABE?AU?$pair@VC2DVector@@V1@@std@@ABVCANavQuadTreeNode@@0M@Z
// Grade: DIFFER(862v862) -- one x87 scheduling swap at 0x00A1A9F6: retail tests `test ah,5` of the first compare before the hoisted second `fcomp [DELTA_NOTIONAL_ZERO]`; ours issues the second fcomp first. Every block layout, slot and constant matches (13 probes: compare spellings, bool locals, nested else, an AreEqual helper, single exit all no better).
// Flags: none
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)
extern const float DELTA_NOTIONAL_ZERO;    // 0x0129BA3C
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
};
class C2DBoxF { public:
    float TLX, TLY, BRX, BRY;
    C2DBoxF(const C2DVector& centre, float radius) : TLX(centre.X - radius), TLY(centre.Y - radius), BRX(centre.X + radius), BRY(centre.Y + radius) { }
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
}
class CANavQuadTreeNode { public:
    virtual void v00();
    C2DVector     Position;
    unsigned char QuadTreeLevel;
    unsigned char NavLayer;
    bool          LeafNode;
    bool          Blocked;
    float GetSize() const;                                   // 0x00A75B30
};
class CNavigatorAStar { private:
    std::pair<C2DVector, C2DVector> GetIntersectionPointsBetweenNodes(const CANavQuadTreeNode& from, const CANavQuadTreeNode& to, float distance) const;
};

std::pair<C2DVector, C2DVector> CNavigatorAStar::GetIntersectionPointsBetweenNodes(const CANavQuadTreeNode& from, const CANavQuadTreeNode& to, float distance) const
{
    C2DBoxF fromBox(from.Position, from.GetSize() * 0.5f);
    C2DBoxF toBox(to.Position, to.GetSize() * 0.5f);
    if ((float)fabs(fromBox.BRX - toBox.TLX) < DELTA_NOTIONAL_ZERO) {
        if ((float)fabs(from.Position.Y - to.Position.Y) < DELTA_NOTIONAL_ZERO) {
            C2DVector first(fromBox.BRX - distance, from.Position.Y);
            C2DVector second(fromBox.BRX + distance, from.Position.Y);
            return std::pair<C2DVector, C2DVector>(first, second);
        } else {
            float gradient = (to.Position.Y - from.Position.Y) / (to.Position.X - from.Position.X);
            float offset1 = from.GetSize() * 0.5f - distance;
            C2DVector first(fromBox.BRX - distance, offset1 * gradient + from.Position.Y);
            float offset2 = from.GetSize() * 0.5f + distance;
            C2DVector second(fromBox.BRX + distance, offset2 * gradient + from.Position.Y);
            return std::pair<C2DVector, C2DVector>(first, second);
        }
    } else if ((float)fabs(fromBox.TLX - toBox.BRX) < DELTA_NOTIONAL_ZERO) {
        if ((float)fabs(from.Position.Y - to.Position.Y) < DELTA_NOTIONAL_ZERO) {
            C2DVector first(fromBox.TLX + distance, from.Position.Y);
            C2DVector second(fromBox.TLX - distance, from.Position.Y);
            return std::pair<C2DVector, C2DVector>(first, second);
        } else {
            float gradient = (to.Position.Y - from.Position.Y) / (to.Position.X - from.Position.X);
            float offset1 = distance - from.GetSize() * 0.5f;
            C2DVector first(fromBox.TLX + distance, offset1 * gradient + from.Position.Y);
            float offset2 = -(from.GetSize() * 0.5f) - distance;
            C2DVector second(fromBox.TLX - distance, offset2 * gradient + from.Position.Y);
            return std::pair<C2DVector, C2DVector>(first, second);
        }
    } else if ((float)fabs(fromBox.BRY - toBox.TLY) < DELTA_NOTIONAL_ZERO) {
        if ((float)fabs(from.Position.X - to.Position.X) < DELTA_NOTIONAL_ZERO) {
            C2DVector first(from.Position.X, fromBox.BRY - distance);
            C2DVector second(from.Position.X, fromBox.BRY + distance);
            return std::pair<C2DVector, C2DVector>(first, second);
        } else {
            float gradient = (to.Position.X - from.Position.X) / (to.Position.Y - from.Position.Y);
            float offset1 = from.GetSize() * 0.5f - distance;
            C2DVector first(offset1 * gradient + from.Position.X, fromBox.BRY - distance);
            float offset2 = from.GetSize() * 0.5f + distance;
            C2DVector second(offset2 * gradient + from.Position.X, fromBox.BRY + distance);
            return std::pair<C2DVector, C2DVector>(first, second);
        }
    } else {
        if ((float)fabs(from.Position.X - to.Position.X) < DELTA_NOTIONAL_ZERO) {
            C2DVector first(from.Position.X, fromBox.TLY + distance);
            C2DVector second(from.Position.X, fromBox.TLY - distance);
            return std::pair<C2DVector, C2DVector>(first, second);
        } else {
            float gradient = (to.Position.X - from.Position.X) / (to.Position.Y - from.Position.Y);
            float offset1 = distance - from.GetSize() * 0.5f;
            C2DVector first(offset1 * gradient + from.Position.X, fromBox.TLY + distance);
            float offset2 = -(from.GetSize() * 0.5f) - distance;
            C2DVector second(offset2 * gradient + from.Position.X, fromBox.TLY - distance);
            return std::pair<C2DVector, C2DVector>(first, second);
        }
    }
}
