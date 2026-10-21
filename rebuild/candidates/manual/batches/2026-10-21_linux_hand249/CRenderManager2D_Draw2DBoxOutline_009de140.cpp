// CRenderManager2D::Draw2DBoxOutline(const C2DBoxF&, const CRGBColour&, unsigned long) -- draw a box's four edges as lines on whole-pixel coordinates
// Leaf: Draw2DBoxOutline
// Retail 0x009DE140, 337 bytes, ret 0xC. Callees: __ftol2 0x00BFEA70, the screen-to-virtual size helpers 0x009E1DA0 /
// 0x009E1DD0 (__stdcall float; one pixel each way), CRenderManager2D::Draw2DLine 0x009DD960.
// Shape: the corners truncate to long in the order TLY, TLX, BRX - 1px, BRY - 1px (the right edge lands in the dead box
// argument slot); every edge passes C2DVector temporaries rebuilt from the longs; the four calls sit in one `&&`
// chain compared `== true` (`cmp al,1 ; jne`) with `return true; return false;` (separate `if (...) return false;`
// statements fold the last test into a `dec/neg/sbb/inc` bool and put `this` in edi instead of esi).
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct CRGBColour { unsigned char B, G, R, A; };
float __stdcall GFVXToAX(float x);                 // 0x009E1DA0
float __stdcall GFVYToAY(float y);                 // 0x009E1DD0

struct CRenderManager2D {
    bool Draw2DLine(const C2DVector& from, const C2DVector& to, const CRGBColour& colour, unsigned long flags);   // 0x009DD960
    bool Draw2DBoxOutline(const C2DBoxF& box, const CRGBColour& colour, unsigned long flags);
};

bool CRenderManager2D::Draw2DBoxOutline(const C2DBoxF& box, const CRGBColour& colour, unsigned long flags)
{
    long top = (long)box.TLY;
    long left = (long)box.TLX;
    long right = (long)(box.BRX - GFVXToAX(1.0f));
    long bottom = (long)(box.BRY - GFVYToAY(1.0f));
    if (Draw2DLine(C2DVector((float)left, (float)top), C2DVector((float)right, (float)top), colour, flags) == true
        && Draw2DLine(C2DVector((float)right, (float)top), C2DVector((float)right, (float)bottom), colour, flags) == true
        && Draw2DLine(C2DVector((float)right, (float)bottom), C2DVector((float)left, (float)bottom), colour, flags) == true
        && Draw2DLine(C2DVector((float)left, (float)bottom), C2DVector((float)left, (float)top), colour, flags) == true)
        return true;
    return false;
}
