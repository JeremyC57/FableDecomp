// CCutsceneDef::Transfer(CPersistContext& context) -- transfer the cutscene def's eight string lists (Macro, SkipCond, SetupCond, Lights, LightScene, Sound, Answer0, Answer1) through the binary vector transfer
// Leaf: ?Transfer@CCutsceneDef@@QAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F2A1D0, 107 bytes (+ int3 padding), `ret 4`; slot 18 of the no-RTTI
//   CCutsceneDef vtable 0x012FB6E0 (its Sizeof slot 0x00F29D90 returns 0x9c; stored by the ctor at 0x00F29D2B). Ego_r.pdb
//   ?Transfer@CCutsceneDef@@UAEXAAVCPersistContext@@@Z. The catalog's "InitialiseDefaultCameraTags" is wrong.
// Callee: CPersistContext::TransferVectorBinary<CCharString>(vector<CCharString>&) @ 0x004331F9 (landed),
//   eight times. No base-class Transfer call.
// Layout: Ego_r.pdb CCutsceneDef (192 bytes) -> retail 0x9c: the CParentDefClassBase part is 0x3c in retail (as the landed
//   CFrontEndDef::Transfer 0x004331C0 models it, PDB 0x40) and each STLport vector is 12 bytes (PDB 16): Macro +0x3c,
//   SkipCond +0x48, Lights +0x54, LightScene +0x60, SetupCond +0x6c, Sound +0x78, Answer0 +0x84, Answer1 +0x90.
//   Modelled without virtuals as the landed CFrontEndDef::Transfer (the vtable slot is the only reference).
// Shape: one call per member in source order (SetupCond is transferred third, before Lights), `this` in esi, the context
//   in edi; the last member address is formed by `add esi,90h`.
class CCharString {
public:
    void* PStringData;
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
}
class CPersistContext {
public:
    template <class T> void TransferVectorBinary(std::vector<T>& v);   // 0x004331F9 for CCharString
};
class CCutsceneDef {
public:
    unsigned char              _base[0x3c];      // CParentDefClassBase (vptr, 0x39 bytes, padding)
    std::vector<CCharString>  Macro;            // +0x3c
    std::vector<CCharString>  SkipCond;         // +0x48
    std::vector<CCharString>  Lights;           // +0x54
    std::vector<CCharString>  LightScene;       // +0x60
    std::vector<CCharString>  SetupCond;        // +0x6c
    std::vector<CCharString>  Sound;            // +0x78
    std::vector<CCharString>  Answer0;          // +0x84
    std::vector<CCharString>  Answer1;          // +0x90
    void Transfer(CPersistContext& context);
};
void CCutsceneDef::Transfer(CPersistContext& context)
{
    context.TransferVectorBinary(Macro);
    context.TransferVectorBinary(SkipCond);
    context.TransferVectorBinary(SetupCond);
    context.TransferVectorBinary(Lights);
    context.TransferVectorBinary(LightScene);
    context.TransferVectorBinary(Sound);
    context.TransferVectorBinary(Answer0);
    context.TransferVectorBinary(Answer1);
}