// CReactionMatch_MRPyramid::TransferBinaryOut(CDataOutputStream& stream) const -- write the asymmetric-pyramid base state, then the three MR shift floats
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FECA0, length 52 bytes (virtual).
// Callees: CReactionMatch_AsymmetricPyramid::TransferBinaryOut @ 0x006FEC30 (the base-class override, called
// non-virtually), CDataOutputStream::WriteFloat @ 0x00993EE0 (x3).
// Layout: engine/CReactionMatch_MRPyramid.h (PDB names): ScarinessShift +0x28, AgreeablenessShift +0x2c,
// AttractivenessShift +0x30; the words +0x04..+0x24 are the flattened CReactionMatch_AsymmetricPyramid base
// (engine/CReactionMatch_AsymmetricPyramid.h).
// Shape: the explicit `Base::TransferBinaryOut(stream)` first (stream loaded into edi before `this` moves to esi),
// then the three floats in declaration order. The base call is reached through a shell over the base layout
// because the generated headers are plain layout structs with no inheritance.
// Leaf: TransferBinaryOut@CReactionMatch_MRPyramid
#include "engine/CReactionMatch_AsymmetricPyramid.h"
#include "engine/CReactionMatch_MRPyramid.h"

struct CDataOutputStream {
    void WriteFloat(float value);                       // 0x00993EE0
};

struct CReactionMatch_AsymmetricPyramid_TransferBinaryOut : CReactionMatch_AsymmetricPyramid {
    void TransferBinaryOut(CDataOutputStream& stream) const;    // 0x006FEC30
};

struct CReactionMatch_MRPyramid_TransferBinaryOut : CReactionMatch_MRPyramid {
    void TransferBinaryOut(CDataOutputStream& stream) const;
};

void CReactionMatch_MRPyramid_TransferBinaryOut::TransferBinaryOut(CDataOutputStream& stream) const
{
    reinterpret_cast<const CReactionMatch_AsymmetricPyramid_TransferBinaryOut*>(this)->TransferBinaryOut(stream);
    stream.WriteFloat(ScarinessShift);
    stream.WriteFloat(AgreeablenessShift);
    stream.WriteFloat(AttractivenessShift);
}
