// _global::GFSerialiseVectorBinaryOut<CActionInputControl>(CDataOutputStream& stream, std::vector<CActionInputControl>& values) -- write the element count, then every control binding's TransferBinaryOut (inlined)
// Leaf: ??$GFSerialiseVectorBinaryOut@VCActionInputControl@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040FCA0, 145 bytes (+15 int3 pad). Callees:
// CDataOutputStream::WriteSLONG 0x00993EB0, CDataOutputStream::Write2DVector 0x00993F90.
//
// Retail bytes:  push ecx ; push ebx ; push ebp ; push esi ; mov ebp,edx ; mov ebx,[ebp] ; push edi ;
//   mov edi,ecx ; mov ecx,[ebp+4] ; sub ecx,ebx ; <divide by 28: 0x92492493> -> esi ; push esi ;
//   mov ecx,edi ; call WriteSLONG ; test esi,esi ; jle X ; xor ebx,ebx ; mov [count],esi ;
//   L: mov esi,[ebp] ; mov eax,[esi+ebx] ; add esi,ebx ; <inlined TransferBinaryOut(esi)> ;
//   mov eax,[count] ; add ebx,0x1c ; dec eax ; mov [count],eax ; jne L ; X: ... ret
// Identity: FableWin.pdb `??$GFSerialiseVectorBinaryOut@VCActionInputControl@@@@YIXAAVCDataOutputStream@@AAV?$vector@...`
// (__fastcall: stream in ecx, the vector in edx). The STLport vector {_M_start, _M_finish,
// _M_end_of_storage}; `size()` is the 28-byte element divide; the loop re-reads _M_start every pass
// (`values[i]` through `begin() + i`, the calls may alias it) with the byte offset strength-reduced
// into ebx and the trip count spilled to the `push ecx` slot. CActionInputControl::TransferBinaryOut
// (out of line at 0x00405D60) is defined in this TU and inlined.
class C2DVector { public:
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

template <class T>
class CRetailVector {                             // STLport std::vector<T> storage
public:
    T* _M_start;                                  // +0
    T* _M_finish;                                 // +4
    T* _M_end_of_storage;                         // +8
    T* begin() { return _M_start; }
    long size() const { return long(_M_finish - _M_start); }
    T& operator[](long n) { return *(begin() + n); }
};

template <class T>
void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStream& stream, CRetailVector<T>& values)
{
    long count = values.size();
    stream.WriteSLONG(count);
    for (long i = 0; i < count; ++i)
        values[i].TransferBinaryOut(stream);
}

template void __fastcall GFSerialiseVectorBinaryOut<CActionInputControl>(CDataOutputStream& stream, CRetailVector<CActionInputControl>& values);