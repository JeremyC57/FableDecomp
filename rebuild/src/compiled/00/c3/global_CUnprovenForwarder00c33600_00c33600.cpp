// CUnprovenForwarder_00c33600(arg) -- forward to the global object's member
// Leaf: ?CUnprovenForwarder_00c33600@@YGXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C33600, 10 bytes: `mov ecx, offset 0x0143BB58 ; jmp 0x00C3D310`.
// Shape (family K004, representative 0x0048573D): a forwarder whose only work is a member call on one global object, which
//   VC7.1 emits as the `this` load and a tail jump.
// Identity (descriptive): a one-argument __stdcall forwarder (callers CAudioResources 0x00C1C700 / the triple-resource destructor 0x00C1C230 push one word and do not clean up) to the member 0x00C3D310 of the global 0x0143BB58, whose names are all stand-ins (g_audioSystem / g_logger / ...).
class CUnprovenSingleton_0143bb58 {                      // the global object at 0x0143BB58 (owner unnamed)
public:
    void CUnprovenMethod_00c3d310(void* arg);                // 0x00C3D310
};
extern CUnprovenSingleton_0143bb58 g_CUnprovenSingleton_0143bb58;   // 0x0143BB58
void __stdcall CUnprovenForwarder_00c33600(void* arg)
{
    g_CUnprovenSingleton_0143bb58.CUnprovenMethod_00c3d310(arg);
}