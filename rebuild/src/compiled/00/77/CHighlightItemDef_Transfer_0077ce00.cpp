// CHighlightItemDef::Transfer(CPersistContext& context) -- persist the highlight item def (run flag, emitters, rotation, bobbing, delay)
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) @ 0x0077CE00, 448 bytes (virtual, `ret 4`).
// Callees: CPersistContext::CheckCRC 0x00404500 (x8, the empty key "" 0x0122D70E);
//   CPersistTraits<bool>::TransferBinaryIn 0x00403EB0 / CDataOutputStream::WriteBool 0x00993E30;
//   CPersistTraits<long>::TransferBinaryIn 0x0040FE60 / CDataOutputStream::WriteLong 0x00993EB0;
//   CPersistTraits<float>::TransferBinaryIn 0x0040EFB0 / CDataOutputStream::WriteFloat 0x00993EE0.
// Shape: eight inline CPersistContext::Transfer<T> calls (the landed COverheadDisplayDef::Transfer 0x007B7D20 template):
//   member address formed into ebx before the CRC call, `sub eax,2 ; je ; dec eax ; jne` on the mode, the empty traits
//   temporary in the dead argument slot. Retail order is StartAsRunning first and BobTime before BobDistance; the last
//   member reuses `this` (`add edi,44h`). `this` is the engine header layout (CHighlightItemDef.h, Ego_r.pdb).
#include "engine/CHighlightItemDef.h"

class CMemoryDataInputStream;
class CDataOutputStream { public:
    void WriteBool(bool value);                         // 0x00993E30
    void WriteLong(long value);                         // 0x00993EB0
    void WriteFloat(float value);                       // 0x00993EE0
};
template<class T> struct CPersistTraits;
template<> struct CPersistTraits<bool> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, bool* value);   // 0x00403EB0
    static void TransferBinaryOut(CDataOutputStream* stream, const bool& value) { stream->WriteBool(value); }
};
template<> struct CPersistTraits<long> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, long* value);   // 0x0040FE60
    static void TransferBinaryOut(CDataOutputStream* stream, const long& value) { stream->WriteLong(value); }
};
template<> struct CPersistTraits<float> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, float* value);  // 0x0040EFB0
    static void TransferBinaryOut(CDataOutputStream* stream, const float& value) { stream->WriteFloat(value); }
};
class CPersistContext { public:
    unsigned char           _pad00[0x18];
    long                    m_mode;                     // +0x18 (2 = load, 3 = save)
    unsigned char           _pad1c[0x08];
    CMemoryDataInputStream* m_pLoadStream;              // +0x24
    CDataOutputStream*      m_pSaveStream;              // +0x28
    void CheckCRC(const char* name);                    // 0x00404500
    template<class T> void Transfer(const char* name, T& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2:
            CPersistTraits<T>().TransferBinaryIn(m_pLoadStream, &value);
            break;
        case 3:
            CPersistTraits<T>::TransferBinaryOut(m_pSaveStream, value);
            break;
        }
    }
};
struct CHighlightItemDef_Transfer : CHighlightItemDef {
    void Transfer(CPersistContext& context);
};

void CHighlightItemDef_Transfer::Transfer(CPersistContext& context)
{
    context.Transfer("", StartAsRunning);
    context.Transfer("", ParticleEmitter);
    context.Transfer("", PickupParticleEmitter);
    context.Transfer("", RotationSpeed);
    context.Transfer("", DistanceFromGround);
    context.Transfer("", BobTime);
    context.Transfer("", BobDistance);
    context.Transfer("", StartAsRunningDelay);
}