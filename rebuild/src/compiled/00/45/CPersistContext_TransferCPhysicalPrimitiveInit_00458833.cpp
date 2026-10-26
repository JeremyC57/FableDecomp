// CPersistContext::Transfer<CPhysicalPrimitiveInit>(CPhysicalPrimitiveInit& value) -- binary persist of one hit-location primitive: CRC-check the empty key, then load (mode 2) or save (mode 3) it through a CPersistTraits<CPhysicalPrimitiveInit> local
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- the switch arms return instead of break (landed 0x004E9550 shape): the traits this is formed once above the first je and the default arm falls into the destructor call
// Leaf: ??$Transfer@VCPhysicalPrimitiveInit@@@CPersistContext@@QAEXAAVCPhysicalPrimitiveInit@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00458833, 76 bytes, `ret 4`; y-off + size TU (frame pointer, `and [ebp-N],0` zeroing,
//         `leave`), the def module of the landed caller CHitLocationDef::Transfer 0x004585FE (its only call site, 0x0045860E).
// Callees: CPersistContext::CheckCRC @ 0x00404500 (key "" @ 0x0122D70E), CPersistTraits<CPhysicalPrimitiveInit>::TransferBinaryOut
//          @ 0x0067F7D0 and ::TransferBinaryIn @ 0x0067F870 (both landed, thiscall on the traits object, 2 stack args),
//          ~CCountedPointer<NHitLocations::CPhysicalPrimitiveBase> @ 0x0044F3B2 (the catalog label _Dest_val<...COptimisedPrimitive>
//          names a byte-identical counted-pointer release).
// Identity: Ego_r.pdb names ??$Transfer@VCPhysicalPrimitiveInit@@@CPersistContext@@QAEXPBDAAVCPhysicalPrimitiveInit@@@Z (name set
//   only); the retail body takes just the value (one stack argument) and checks the empty key. PDB CPersistTraits<T> is
//   { T DefaultVal; }, here 8 bytes: CPhysicalPrimitiveInit = { CCountedPointer<CPhysicalPrimitiveBase> PPrimitive } = {Data, Count}.
// Layout: CPersistContext Mode +0x18, PLoadStream +0x24, PSaveStream +0x28 (as the landed 0x006420F0 Transfer<CRGBColour>);
//   no engine header for CPersistContext / CPersistTraits, local models.
// Shape: CheckCRC first, then the traits local (its counted pointer zeroed inline: `and [ebp-8],0 / and [ebp-4],0`), the
//   two-case switch on the mode (`dec/dec/je`, `dec/jne`) with the traits address hoisted into ecx ahead of the branch, and
//   the out-of-line counted-pointer release on every path.
// Fix (agent901): the switch arms RETURN instead of break (the landed 0x004E9550 TransferHelper shape): VC7.1 then
//   forms the traits `this` once above the first `je` and the default arm falls straight into the destructor call.
#pragma optimize("y", off)
#pragma optimize("s", on)   // the def module is size-optimised

class CDataOutputStream;
class CMemoryDataInputStream;

namespace NHitLocations { class CPhysicalPrimitiveBase; }
struct SCountBlock;

template <class T> class CCountedPointer { public:
    T*           Data;                  // +0
    SCountBlock* Count;                 // +4
    CCountedPointer() : Data(0), Count(0) {}
    ~CCountedPointer();                 // 0x0044F3B2 for T = NHitLocations::CPhysicalPrimitiveBase
};

class CPhysicalPrimitiveInit {
public:
    CCountedPointer<NHitLocations::CPhysicalPrimitiveBase> PPrimitive;   // +0x00
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CPhysicalPrimitiveInit> {
    CPhysicalPrimitiveInit DefaultVal;                                                   // +0x00
    void TransferBinaryOut(CDataOutputStream* stream, const CPhysicalPrimitiveInit* value); // 0x0067F7D0
    void TransferBinaryIn(CMemoryDataInputStream* stream, CPhysicalPrimitiveInit* value);   // 0x0067F870
};

class CPersistContext { public:
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
        return;
    case 3:
        traits.TransferBinaryOut(PSaveStream, &value);
        return;
    }
}

template void CPersistContext::Transfer<CPhysicalPrimitiveInit>(CPhysicalPrimitiveInit& value);