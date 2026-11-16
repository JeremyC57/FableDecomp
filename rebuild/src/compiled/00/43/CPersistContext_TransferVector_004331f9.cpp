// CPersistContext::TransferVector<CCharString>(std::vector<CCharString>& value) -- unnamed binary vector transfer: CheckCRC(""), then the global vector reader (Mode 2, load stream +0x24) or writer (any other mode, save stream +0x28)
// Leaf: ??$TransferVector@VCCharString@@@CPersistContext@@QAEXAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@Z
// Retail: Fable.exe @ 0x004331F9, 46 bytes, `ret 4` (the vector is the only argument). Catalog label
// CCreatureAction_FireMissileWeapon::FrameUpdate is wrong: the body is a CPersistContext member template --
// CheckCRC("") (0x00404500, Ego_r.pdb ?CheckCRC@CPersistContext@@IAEXPBD@Z; the pooled empty literal at 0x0122D70E), then
// Mode (+0x18) == 2 (binary load) ? GFSerialiseVectorBinaryIn<CCharString>(*PLoadStream +0x24, v) (0x00433273)
// : GFSerialiseVectorBinaryOut<CCharString>(*PSaveStream +0x28, v) (0x00433227, landed as GFSerialiseVectorBinaryOut<CCharString>), both __fastcall as in Ego_r.pdb
// (??$GFSerialiseVectorBinaryIn@..@@YIXAAVCMemoryDataInputStream@@AAV?$vector@..). Callers include CFrontEndDef::Transfer (0x004331C0, vAttractModeMovie at +0x3c) and a dozen other CCharString-vector transfers.
// The name is descriptive: Ego_r.pdb's CPersistContext::TransferVector<T> takes (const char*, vector&); this retail
// instance takes only the vector and handles only the binary modes.
// Layout: Ego_r.pdb CPersistContext (Mode +0x18, PLoadStream +0x24, PSaveStream +0x28; no engine header, as landed 0x00757160).
// Opt: size TU (unaligned entry; the literal pushed before `this` is moved to esi). Shape: a switch on Mode with one
// case (`dec eax ; dec eax ; je`); an if/else compiles to `cmp [esi+18h],2`.
// Name (round 4, NF40): the member template instance ??$TransferVector@VCCharString@@@CPersistContext@@QAEXAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@Z, the unnamed overload engine/CPersistContext.h
//   declares (`template<class T> void TransferVector(std::vector<T>&)`; TLC's release persist macro drops the name argument, as
//   for TransferVectorOfSubComponents; the PDB name sets hold the named ??$TransferVector@VCCharString@@@..QAEXPBDAAV?$vector@..@Z twin).
//   Same body and spelling as the landed 0x006724B0 TransferVector<ENavigatorType>. Was a local CPersistContext model with a
//   stand-in TransferBinary_004331f9 / TransferVectorBinary member; now on the shared header.
#pragma optimize("s", on)
#include "fable_compiler.h"
#define FABLE_RETAIL_CPERSISTCONTEXT
#include "engine/CPersistContext.h"
class CCharString;
template <class T> void FABLE_FASTCALL GFSerialiseVectorBinaryIn(CMemoryDataInputStream& is, std::vector<T, std::allocator<T> >& vec);
template <class T> void FABLE_FASTCALL GFSerialiseVectorBinaryOut(CDataOutputStream& os, std::vector<T, std::allocator<T> >& vec);

template <class T> void CPersistContext::TransferVector(std::vector<T, std::allocator<T> >& vec)
{
    CheckCRC("");
    switch (Mode) {
    case MODE_LOAD_BINARY:
        GFSerialiseVectorBinaryIn(*PLoadStream, vec);
        break;
    default:
        GFSerialiseVectorBinaryOut(*PSaveStream, vec);
        break;
    }
}

template void CPersistContext::TransferVector<CCharString>(std::vector<CCharString, std::allocator<CCharString> >& vec);
