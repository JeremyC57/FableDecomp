// CAmbientChannel::CAmbientChannel(unsigned long soundId) -- store the argument; the second word starts at its constant
// Leaf: ??0CAmbientChannel@@QAE@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C62120, 18 bytes, thiscall, `ret 4`, returns this.
// Identity (descriptive): its only caller, the landed CAmbientSource::CreateChannels 0x00C470E0 (libpk ambient sound, no PDB/RTTI: descriptive names), news an 8-byte CAmbientChannel per channel sound id and constructs it here ({id, -1}). The manifest's "CThingFilter_SolidToThing::CThingFilter_SolidToThing" is propagated across
//   byte-identical bodies.
// Shape (family M182, representative 0x00661620): member initialisers in declaration order. Speed TU.
class CAmbientChannel {
public:
    CAmbientChannel(unsigned long soundId);
    unsigned long m_soundId;                              // +0x00
    long          m_handle;                               // +0x04 (-1: not playing)
};
CAmbientChannel::CAmbientChannel(unsigned long soundId)
    : m_soundId(soundId), m_handle(-1)
{
}