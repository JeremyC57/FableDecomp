// CEngineSkyRenderer::KeepHistoryInRange(C2DVector*, float) -- wrap a two-entry 2D scroll history back into [-range, range]: when the current X (or Y) has run past +range or -range, shift both history entries' X (or Y) by the range
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B61AA0, length 143 bytes (+ int3 padding). No callees.
// The body is byte-identical to 0x00BE9530 (the same inline helper compiled into a second sky/cloud TU).
// Shape: a `__fastcall` member that never reads `this` (ecx is reused for the history pointer, `ret 8`); each axis is
// `if (h[0].X > range) {...} else if (-range > h[0].X) {...}` (`fcomp ; test ah,41h ; jne`, the negation is `fld ; fchs`);
// `x += range` loads the range first (`fld range ; fadd [x]`), `x -= range` loads x first; the Y tests end in their own
// `ret 8`. `this` has no members here, so the engine header (engine/CEngineSkyRenderer.h) is not needed.
class C2DVector { public:
    float X, Y;
};

class CEngineSkyRenderer { public:
    void KeepHistoryInRange(C2DVector* history, float range);
};

void CEngineSkyRenderer::KeepHistoryInRange(C2DVector* history, float range)
{
    if (history[0].X > range) {
        history[0].X -= range;
        history[1].X -= range;
    } else if (-range > history[0].X) {
        history[0].X += range;
        history[1].X += range;
    }
    if (history[0].Y > range) {
        history[0].Y -= range;
        history[1].Y -= range;
    } else if (-range > history[0].Y) {
        history[0].Y += range;
        history[1].Y += range;
    }
}