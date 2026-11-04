// Behaviour test for 0x00BE24D0: C2DAlphaMaskInfo cleared state -- bank null, index 0, position factors zero, both flags
// off, and the clip box left untouched (retail never writes +0x18..+0x27 here).
#include <cstdio>
#include <cstring>
#include <new>
struct C4DVector { float X, Y, Z, W; C4DVector() { } C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { } };
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct C2DAlphaMaskInfo {
    void* AlphaMaskBank; unsigned int AlphaMaskBankIndex; C4DVector AlphaMaskPositionFactors; C2DBoxF AlphaMaskClipBox;
    bool MaskObjectsOutsideAlphaMask; bool FilterMask;
    void Reset() { AlphaMaskBank = 0; AlphaMaskBankIndex = 0; AlphaMaskPositionFactors = C4DVector(0, 0, 0, 0); MaskObjectsOutsideAlphaMask = false; FilterMask = false; }
};
int main() {
    static unsigned char raw[sizeof(C2DAlphaMaskInfo)];
    std::memset(raw, 0x5A, sizeof(raw));
    C2DAlphaMaskInfo& m = *reinterpret_cast<C2DAlphaMaskInfo*>(raw);
    m.Reset();
    bool ok = m.AlphaMaskBank == 0 && m.AlphaMaskBankIndex == 0 && m.AlphaMaskPositionFactors.X == 0.0f &&
              m.AlphaMaskPositionFactors.Y == 0.0f && m.AlphaMaskPositionFactors.Z == 0.0f && m.AlphaMaskPositionFactors.W == 0.0f &&
              !m.MaskObjectsOutsideAlphaMask && !m.FilterMask;
    const unsigned char* box = reinterpret_cast<const unsigned char*>(&m.AlphaMaskClipBox);
    for (unsigned int i = 0; i < sizeof(C2DBoxF); ++i) ok = ok && box[i] == 0x5A;
    std::printf(ok ? "00be24d0_TEST PASS\n" : "00be24d0_TEST FAIL\n");
    return ok ? 0 : 1;
}