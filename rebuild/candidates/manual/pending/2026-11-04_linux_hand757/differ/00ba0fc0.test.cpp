// Behaviour test for 0x00BA0FC0 CEnginePrimitiveManagerStaticMeshesBase::AddStaticMesh: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
struct P { unsigned long flags; long alpha; long alphaRef; bool nonPersistent; float bias; };
static P Add(unsigned long flags, long alpha, long alphaRef, bool persistent, float bias, unsigned long& registered)
{
    P p; p.flags = flags; p.alpha = (unsigned char)alpha; p.alphaRef = (unsigned char)alphaRef; p.bias = bias; p.nonPersistent = !persistent;
    registered = flags & 1; return p;
}
int main() {
    unsigned long r = 9; P p = Add(3, 0x1ff, 0x80, true, 0.5f, r);
    int ok = p.flags == 3 && p.alpha == 0xff && p.alphaRef == 0x80 && !p.nonPersistent && r == 1 && p.bias == 0.5f;
    p = Add(2, 1, 2, false, 0.0f, r); ok &= p.nonPersistent && r == 0;
    std::printf(ok ? "00ba0fc0_TEST PASS\n" : "00ba0fc0_TEST FAIL\n");
    return ok ? 0 : 1;
}
