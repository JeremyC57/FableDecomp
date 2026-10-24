// std::_Construct(CGameCameraManager::CQuake*, const CGameCameraManager::CQuake&) -- placement copy of a camera quake: a trivially copyable 0x30-byte record (eleven dwords and the Global flag)
// Retail: Fable.exe @ 0x0069C860, 75 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: none (inline trivial copy).
// Leaf: _Construct
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N0069c860 {
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }   // user memberwise copy: fixed-offset dword moves, no block copy
};
struct CQuake {                                  // CGameCameraManager::CQuake (PDB, sizeof 0x30)
    C3DVector Position;                          // +0x00
    C3DVector Axis;                              // +0x0C
    long      Frame;                             // +0x18
    float     Amplitude;                         // +0x1C
    float     Duration;                          // +0x20
    float     Frequency;                         // +0x24
    float     Phase;                             // +0x28
    bool      Global;                            // +0x2C
    __forceinline CQuake(const CQuake& o)        // the implicit memberwise copy, spelled out so VC7.1 inlines it into _Construct
        : Position(o.Position), Axis(o.Axis), Frame(o.Frame), Amplitude(o.Amplitude), Duration(o.Duration),
          Frequency(o.Frequency), Phase(o.Phase), Global(o.Global) { }
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CQuake* p, const CQuake& value)
{
    if (p != 0)
        new (p) CQuake(value);
}
}