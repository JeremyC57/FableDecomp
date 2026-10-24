// CDisplayManager::SetViewport(const C2DBoxF& box) -- round a float viewport box to integers and forward to SetViewport(const C2DBoxI&)
// Leaf: ?SetViewport@CDisplayManager@@QAEXABVC2DBoxF@@@Z
// Retail: Fable.exe @ 0x009BF490, 112 bytes, __thiscall `ret 4`; the four rounded corners land in the inlined helpers' result
// slots ([esp+0Ch]..[esp]) and the C2DBoxI(long,long,long,long) temporary at [esp+10h] is passed to 0x009BEF80.
// Retail-asm: each corner (0x009BF49E/0x009BF4AD/0x009BF4BC/0x009BF4CA) is stored to the dead argument slot, reloaded with `fld` and rounded with a bare `fistp dword` (0x009BF4A2/0x009BF4B1/0x009BF4C0/0x009BF4CE) -- the original's inline-asm FtoL helper inlined four times; VC7.1 C++ `(long)f` calls _ftol2 and /QIfist emits `fistp qword` with an 8-byte slot (graded DIFFER 86v112).
class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
class C2DBoxI {
public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI(long tlx, long tly, long brx, long bry) { TLX = tlx; TLY = tly; BRX = brx; BRY = bry; }
    C2DBoxI(const C2DBoxI& b) : TLX(b.TLX), TLY(b.TLY), BRX(b.BRX), BRY(b.BRY) { }
};
class CDisplayManager {
public:
    void SetViewport(const C2DBoxI& box);       // 0x009BEF80
    void SetViewport(const C2DBoxF& box);
};
// round with the current FPU rounding mode (the engine's inline-asm FtoL)
static __forceinline long FtoL(float f)
{
    long result;
    __asm fld f
    __asm fistp result
    return result;
}
void CDisplayManager::SetViewport(const C2DBoxF& box)
{
    SetViewport(C2DBoxI(FtoL(box.TLX), FtoL(box.TLY), FtoL(box.BRX), FtoL(box.BRY)));
}
