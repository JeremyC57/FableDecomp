// C2DVectorI::ToC2DVectorF() -- integer point to float point by value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C620B0, oracle length 17 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  fild dword ptr [ecx+4] ; mov eax,[esp+4] ; fild dword ptr [ecx] ; fstp dword ptr [eax] ; fstp dword ptr [eax+4] ; ret 4
// Two longs converted to two floats into the hidden 8-byte result (the manifest says C2DBoxI ->
// C2DBoxF, but a box is 16 bytes; this body converts one 2D point).
struct C2DVectorF {
    float X;    // +0x00
    float Y;    // +0x04
    C2DVectorF(float x, float y) : X(x), Y(y) {}
};

struct C2DVectorI {
    long X;     // +0x00
    long Y;     // +0x04
    C2DVectorF ToC2DVectorF();
};

C2DVectorF C2DVectorI::ToC2DVectorF()
{
    // Constructor arguments are evaluated right to left (fild Y, fild X) and stored X then Y
    // straight into the hidden result; a named temporary goes through the frame (37 bytes).
    return C2DVectorF((float)this->X, (float)this->Y);
}