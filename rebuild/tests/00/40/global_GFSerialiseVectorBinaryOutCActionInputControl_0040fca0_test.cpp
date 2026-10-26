#include <cstdio>

// Model of 0x0040FCA0 GFSerialiseVectorBinaryOut<CActionInputControl>: write the element count, then each element's
// TransferBinaryOut (five SLONGs and a 2D vector).
class C2DVector { public:
    float X;
    float Y;
};
class CDataOutputStream {
public:
    long Longs[32];
    int LongCount;
    int Vectors;
    CDataOutputStream() : LongCount(0), Vectors(0) { }
    void WriteSLONG(long value) { Longs[LongCount++] = value; }
    void Write2DVector(const C2DVector& value) { ++Vectors; Longs[LongCount++] = (long)(value.X * 10.0f); }
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
template <class T> class CRetailVector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    long size() const { return long(_M_finish - _M_start); }
    T& operator[](long n) { return *(begin() + n); }
};
template <class T> void GFSerialiseVectorBinaryOut(CDataOutputStream& stream, CRetailVector<T>& values)
{
    long count = values.size();
    stream.WriteSLONG(count);
    for (long i = 0; i < count; ++i)
        values[i].TransferBinaryOut(stream);
}

int main()
{
    CActionInputControl items[2];
    for (int i = 0; i < 2; ++i) {
        items[i].GameAction = 100 + i;
        items[i].ControllerType = 1;
        items[i].KeyboardKey = 2;
        items[i].XboxButton = 3;
        items[i].MouseButton = 4;
        items[i].ControlDirection.X = 0.5f + i;
        items[i].ControlDirection.Y = 0.0f;
    }
    CRetailVector<CActionInputControl> values;
    values._M_start = items;
    values._M_finish = values._M_end_of_storage = items + 2;
    CDataOutputStream stream;
    GFSerialiseVectorBinaryOut(stream, values);
    const long expected[] = { 2, 100, 1, 2, 3, 4, 5, 101, 1, 2, 3, 4, 15 };
    if (stream.LongCount != 13 || stream.Vectors != 2) {
        std::printf("0040fca0_TEST FAIL\n");
        return 1;
    }
    for (int i = 0; i < 13; ++i)
        if (stream.Longs[i] != expected[i]) {
            std::printf("0040fca0_TEST FAIL\n");
            return 1;
        }
    CRetailVector<CActionInputControl> empty;
    empty._M_start = empty._M_finish = empty._M_end_of_storage = 0;
    CDataOutputStream stream2;
    GFSerialiseVectorBinaryOut(stream2, empty);
    if (stream2.LongCount != 1 || stream2.Longs[0] != 0) {
        std::printf("0040fca0_TEST FAIL\n");
        return 1;
    }
    std::printf("0040fca0_TEST PASS\n");
    return 0;
}