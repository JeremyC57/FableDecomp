#include <cstdio>

// Model of 0x00405D60 CActionInputControl::TransferBinaryOut: write the five enum fields as SLONGs then the 2D
// control direction (the stream records what it is given).
struct C2DVector {
    float X;
    float Y;
};
class CDataOutputStream {
public:
    long Longs[8];
    int LongCount;
    float VectorX, VectorY;
    int VectorAt;
    CDataOutputStream() : LongCount(0), VectorX(0), VectorY(0), VectorAt(-1) { }
    void WriteSLONG(long value) { Longs[LongCount++] = value; }
    void Write2DVector(const C2DVector& value) { VectorX = value.X; VectorY = value.Y; VectorAt = LongCount; }
};
class CActionInputControl {
public:
    long GameAction;
    long ControllerType;
    long KeyboardKey;
    long XboxButton;
    long MouseButton;
    C2DVector ControlDirection;
    void TransferBinaryOut(CDataOutputStream& stream) const
    {
        stream.WriteSLONG(GameAction);
        stream.WriteSLONG(ControllerType);
        stream.WriteSLONG(KeyboardKey);
        stream.WriteSLONG(XboxButton);
        stream.WriteSLONG(MouseButton);
        stream.Write2DVector(ControlDirection);
    }
};

int main()
{
    CActionInputControl control;
    control.GameAction = 11;
    control.ControllerType = 2;
    control.KeyboardKey = 0x41;
    control.XboxButton = 7;
    control.MouseButton = 1;
    control.ControlDirection.X = 0.5f;
    control.ControlDirection.Y = -1.0f;
    CDataOutputStream stream;
    control.TransferBinaryOut(stream);
    if (stream.LongCount != 5 || stream.Longs[0] != 11 || stream.Longs[1] != 2 || stream.Longs[2] != 0x41 ||
        stream.Longs[3] != 7 || stream.Longs[4] != 1 || stream.VectorAt != 5 || stream.VectorX != 0.5f ||
        stream.VectorY != -1.0f) {
        std::printf("00405d60_TEST FAIL\n");
        return 1;
    }
    std::printf("00405d60_TEST PASS\n");
    return 0;
}