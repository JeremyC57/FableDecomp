// CPersistTraits<CEngineGraphic>::TransferBinaryOut(CDataOutputStream*, const CEngineGraphic*) -- write Type, BankIndex, AnimStep, RenderSizeX and AdditiveAlpha
// Retail: Fable.exe (TLC, VC7.1) @ 0x0067D7D0, 72 bytes, `ret 8` (__fastcall, the empty traits `this` is unused).
// Callees: CDataOutputStream::WriteLong @ 0x00993EB0 (the Type byte widened, `movzx`), WriteULong @ 0x00993EA0
//   (BankIndex), WriteFloat @ 0x00993EE0 (AnimStep, RenderSizeX; the speed TU copies the float through a GPR),
//   WriteEBOOL @ 0x00993E30 (AdditiveAlpha).
// Layout (PDB CEngineGraphic, 0x0E bytes): BankIndex +0, AnimStep +4, RenderSizeX +8, AdditiveAlpha +0xC, Type (uchar)
//   +0xD. No engine header exists; local model with the PDB member names. Speed-optimised (default) TU.
#include "engine/CDataOutputStream.h"


class CEngineGraphic { public:
    unsigned long BankIndex;        // +0x00
    float         AnimStep;         // +0x04
    float         RenderSizeX;      // +0x08
    bool          AdditiveAlpha;    // +0x0C
    unsigned char Type;             // +0x0D
};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<CEngineGraphic> {
    void TransferBinaryOut(CDataOutputStream* stream, const CEngineGraphic* value);
};

void CPersistTraits<CEngineGraphic>::TransferBinaryOut(CDataOutputStream* stream, const CEngineGraphic* value)
{
    CDataOutputStream* out = (CDataOutputStream*)stream;
    out->WriteSLONG(value->Type);
    out->WriteULONG(value->BankIndex);
    out->WriteFloat(value->AnimStep);
    out->WriteFloat(value->RenderSizeX);
    out->WriteEBOOL(value->AdditiveAlpha);
}