// CPersistTraits::TransferBinaryIn(CMemoryDataInputStream*, C3DVector*) -- reads a C3DVector from the binary stream (0x009950F0, by value) and stores it through the output pointer
// Leaf: TransferBinaryIn
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047CF3F, 34 bytes. Pragmas: frame pointer kept
// (`optimize("y",off)`) in a size-optimised TU, as every CPersistTraits<T>::TransferBinaryIn TU.
// Callee: 0x009950F0 CMemoryDataInputStream::Read3DVector (__thiscall, C3DVector by hidden pointer).
//
// The manifest label `CPhysicsForceModelGotoOutsideSimple::DoAttractToPoint` is propagated and
// wrong. Its only caller is the binary branch of a CPersistContext::Transfer (0x004859F2), which
// calls it on a traits object in its frame (`lea ecx,[ebp-0Ch]`) with the load stream and the
// value pointer: CPersistTraits<C3DVector>::TransferBinaryIn, `this` unused, `ret 8`. The vector
// comes back in a 12-byte frame temporary and is copied to *value as a POD under /Os (three
// `movsd` through esi/edi).
struct C3DVector {
    float x;
    float y;
    float z;
};

struct CMemoryDataInputStream {
    C3DVector Read3DVector();              // 0x009950F0
};

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream* stream, T* value);
};

template <> void CPersistTraits<C3DVector>::TransferBinaryIn(CMemoryDataInputStream* stream, C3DVector* value)
{
    *value = stream->Read3DVector();
}
