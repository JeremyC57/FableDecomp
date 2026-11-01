// SPosition2D::Add(const CIndexedValue& value) -- add the pair's two words to the position
// Leaf: ?Add@SPosition2D@@QAEXABUCIndexedValue@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C46DA0, 19 bytes (thiscall, `ret 4`).
// Name (naming wave 20, chunk 687): descriptive (libpk, no PDB) -- CAmbientEvent::Trigger 0x00C61D10 adds the scene's +0x40 pair (0x00C25790) to the
//   looked-up sound position before the jitter; was the propagated CQuadric::OperatorPlusEq (a same-type pointer model).
struct CIndexedValue {
    float X;
    float Y;
};
struct SPosition2D {
    float X;
    float Y;
    void Add(const CIndexedValue& value);
};
void SPosition2D::Add(const CIndexedValue& value)
{
    X = value.X + X;
    Y = value.Y + Y;
}
