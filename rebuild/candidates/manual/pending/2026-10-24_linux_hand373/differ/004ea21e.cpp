// CPersistContext::Transfer<NGraphicAppearance::CReplaceableMeshes>(CReplaceableMeshes& value) -- binary persist of a replaceable-mesh set: CRC-check the empty key, then load (mode 2) or save (mode 3) it through a CPersistTraits<CReplaceableMeshes> local alive across the switch
// Grade: DIFFER(80v77) -- scheduling only: retail hoists `lea ecx,[ebp-0Ch]` (the traits) above the mode switch so the default path calls the destructor with it and the two arms reuse it; ours re-materialises it in each arm and before the destructor (3 extra bytes) -- tried: switch, if/else-if, a traits reference, implicit traits ctor/dtor over the vector member
// Leaf: ??$Transfer@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EA21E, 77 bytes, `ret 4`; y-off + size TU (frame pointer, `leave`).
// Caller: 0x004EA216 (a def's Transfer forwarding its CReplaceableMeshes member at +0x28 to the context).
// Callees: CPersistContext::CheckCRC @ 0x00404500 (key "" @ 0x0122D70E); the traits' DefaultVal constructor @ 0x004EA2AE
//   (the 12-byte CReplaceableMeshes: its STLport vector zeroed; catalogued as the propagated CIndexBuffer::CIndexBuffer) and
//   destructor @ 0x004EA2BB (catalogued CActiveFile::OnReadFinished); CPersistTraits<CReplaceableMeshes>::TransferBinaryIn
//   @ 0x004EA2C9 and ::TransferBinaryOut @ 0x004EA26B (both landed, thiscall on the traits, out of line here).
// Label correction: the catalog's "CReplaceableMeshes_Transfer" is descriptive; this is the CPersistContext::Transfer<T>
//   template body of the landed CSpecialEffectsStringMap instance 0x0045C8D3, with T = NGraphicAppearance::CReplaceableMeshes.
// Shape: `CheckCRC(""); CPersistTraits<T> traits; switch (Mode) { case 2: traits.TransferBinaryIn(PLoadStream, &value);
//   break; case 3: traits.TransferBinaryOut(PSaveStream, &value); break; }` -- the mode decoded `dec ; dec ; je ; dec ; jne`,
//   `lea ecx,[ebp-0Ch]` hoisted above the switch for the default path's destructor call.
// Layout: CPersistContext Mode +0x18, PLoadStream +0x24, PSaveStream +0x28 (local model as in 0x0045C8D3; no engine header);
//   PDB NGraphicAppearance::CReplaceableMeshes = { CArray<CEngineGraphic> Vector } (retail STLport vector, 12 bytes).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CDataOutputStream;
struct CMemoryDataInputStream;

namespace NGraphicAppearance {
struct CEngineGraphicVector {                   // CArray<CEngineGraphic> = STLport vector, 12 bytes
    void* _M_start;                             // +0x00
    void* _M_finish;                            // +0x04
    void* _M_end_of_storage;                    // +0x08
};
class CReplaceableMeshes {
public:
    CEngineGraphicVector Vector;                // +0x00
};
}

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<NGraphicAppearance::CReplaceableMeshes> {
    NGraphicAppearance::CReplaceableMeshes DefaultVal;                                                  // +0x00
    CPersistTraits();                                                                                   // 0x004EA2AE
    ~CPersistTraits();                                                                                  // 0x004EA2BB
    void TransferBinaryOut(CDataOutputStream* stream, const NGraphicAppearance::CReplaceableMeshes* value);   // 0x004EA26B
    void TransferBinaryIn(CMemoryDataInputStream* stream, NGraphicAppearance::CReplaceableMeshes* value);     // 0x004EA2C9
};

struct CPersistContext {
    unsigned char           _pad00[0x18];
    long                    Mode;                           // +0x18 (2 = binary load, 3 = binary save)
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* PLoadStream;                    // +0x24
    CDataOutputStream*      PSaveStream;                    // +0x28
    void CheckCRC(const char* name);                        // 0x00404500
    template <class T> void Transfer(T& value);
};

template <class T>
void CPersistContext::Transfer(T& value)
{
    CheckCRC("");
    CPersistTraits<T> traits;
    switch (Mode) {
    case 2:
        traits.TransferBinaryIn(PLoadStream, &value);
        break;
    case 3:
        traits.TransferBinaryOut(PSaveStream, &value);
        break;
    }
}

template void CPersistContext::Transfer<NGraphicAppearance::CReplaceableMeshes>(NGraphicAppearance::CReplaceableMeshes& value);
