// C2DBoxF::Move(const C2DVector&) -- translate the box so its top-left corner lands on pos, keeping its size
// Leaf: ?Move@C2DBoxF@@QAEXABVC2DVector@@@Z
// Retail: Fable.exe @ 0x0041A810, 39 bytes; Ego_r.pdb same symbol. BR += pos - TL per axis (x87 fld/fsub/fadd/fstp),
// then TL = pos (integer dword copies). Caller 0x005480BA.
// Layout: Ego_r.pdb C2DBoxF (TLX, TLY, BRX, BRY floats); no engine/ header.
class C2DVector {
public:
    float X, Y;
};
class C2DBoxF {
public:
    void Move(const C2DVector& pos);
    float TLX, TLY;                                      // +0x00
    float BRX, BRY;                                      // +0x08
};
void C2DBoxF::Move(const C2DVector& pos)
{
    BRX += pos.X - TLX;
    BRY += pos.Y - TLY;
    TLX = pos.X;
    TLY = pos.Y;
}