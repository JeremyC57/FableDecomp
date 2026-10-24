// CActionInputControl::TransferBinaryOut(CDataOutputStream& stream) const -- serialise one control binding: the five enum fields as longs, then the 2D control direction
// Leaf: ?TransferBinaryOut@CActionInputControl@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00405D60, 78 bytes (+2 int3 pad). Callees:
// CDataOutputStream::WriteSLONG 0x00993EB0 (x5; manifest label WriteCBYTE is propagated),
// CDataOutputStream::Write2DVector 0x00993F90.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi] ; push edi ; mov edi,[stream] ; push eax ;
//   mov ecx,edi ; call WriteSLONG ; (same for +4, +8, +0xc, +0x10) ; add esi,0x14 ; push esi ;
//   mov ecx,edi ; call Write2DVector ; pop edi ; pop esi ; ret 4
// Identity: Ego_r.pdb / FableWin.pdb `?TransferBinaryOut@CActionInputControl@@QBEXAAVCDataOutputStream@@@Z`
// (the manifest's WriteToFile is descriptive); GFSerialiseVectorBinaryOut<CActionInputControl>
// 0x0040FCA0 inlines this same body per element. Layout (Ego_r.pdb, 0x1C): GameAction +0,
// ControllerType +4, KeyboardKey +8, XboxButton +0xC, MouseButton +0x10, ControlDirection +0x14.
// engine/CDataOutputStream.h is a flattened data record; the stream is only a call target here.
struct C2DVector {
    float X;                                      // +0
    float Y;                                      // +4
};

class CDataOutputStream {
public:
    void WriteSLONG(long value);                  // 0x00993EB0
    void Write2DVector(const C2DVector& value);   // 0x00993F90
};

class CActionInputControl {
public:
    long GameAction;                              // +0x00 EGameAction
    long ControllerType;                          // +0x04 EControllerType
    long KeyboardKey;                             // +0x08 EInputKey
    long XboxButton;                              // +0x0C EXboxControllerButton
    long MouseButton;                             // +0x10 EMouseButtonControl
    C2DVector ControlDirection;                   // +0x14
    void TransferBinaryOut(CDataOutputStream& stream) const;
};

void CActionInputControl::TransferBinaryOut(CDataOutputStream& stream) const
{
    stream.WriteSLONG(GameAction);
    stream.WriteSLONG(ControllerType);
    stream.WriteSLONG(KeyboardKey);
    stream.WriteSLONG(XboxButton);
    stream.WriteSLONG(MouseButton);
    stream.Write2DVector(ControlDirection);
}