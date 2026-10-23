// Grade: DIFFER(3v1446) -- parked unreached (skeleton only): aligned-stack frame (and esp,-8; 0x3f4 locals) scene loader with rep-movs struct copies, x87 setup and "Enable" config string; not reached beyond decode.
// CEngine::LoadNewScene() -- PARK SKELETON (decoded only)
// Leaf: LoadNewScene
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B277A0, length 1456 bytes.
// Decode: aligned-stack frame (and esp,-8; 0x3f4 locals) scene loader with rep-movs struct copies, x87 setup and "Enable" config string; not reached beyond decode.
struct CEngine {
    unsigned char _pad[0x100];
    void LoadNewScene(long scene);
};

void CEngine::LoadNewScene(long scene)
{
}
