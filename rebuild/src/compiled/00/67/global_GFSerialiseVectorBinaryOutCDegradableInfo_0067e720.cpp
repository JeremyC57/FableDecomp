// GFSerialiseVectorBinaryOut<CDegradableInfo>(CDataOutputStream& stream, vector<CDegradableInfo>& value) -- write the element count, then every degradable stage: health percentage, the raw 14-byte engine graphic, smash emitter, blocks-navigation flag and the record check word
// Leaf: ??$GFSerialiseVectorBinaryOut@VCDegradableInfo@@
//
// Retail 0x0067E720, 139 bytes (+ int3 padding to 144), __fastcall (the stream in ecx, the vector in edx), plain `ret`;
//   speed TU (`imul 92492493h` for the /28 count, the loop counter in the `push ecx` stack word).
// Callees: CDataOutputStream::WriteSLONG @ 0x00993EB0 (the count, SmashParticleEmitter and the check word),
//   WriteFloat @ 0x00993EE0, WriteEBOOL @ 0x00993E30 (catalogued "WriteCBYTE"/"WriteEBOOL"), and the stream's virtual
//   Write(const void*, long) (slot 4, `call [edx+10h]`; Ego_r ?Write@CMemoryDataOutputStream@@UAEXPBXJ@Z).
// Identity: Ego_r.pdb CDegradableInfo (28 bytes) = { float HealthPercentage; CEngineGraphic EngineGraphic (+4, 14 packed
//   bytes); long SmashParticleEmitter (+0x14); bool BlocksNavigation (+0x18); }; the load arm's out-of-line traits is
//   CPersistTraits<CDegradableInfo>::TransferBinaryIn 0x0067DF60.  Callers 0x0067E35A / 0x0067F4FC / 0x0067F53C.
// Shape: the landed generic template (0x0046AFF4 / 0x0046BE4D) with the traits transfer inline: the five writes in
//   member order, then the constant 0x01AB6CE3 every record ends with.
// Model: engine/CDataOutputStream.h is data-only (its vptr is a `void*`), so the writer is the landed local polymorphic
//   view (destructor, SetPosition, GetPosition, Close, Write) over the same {vptr, NewLine} layout; no engine header
//   exists for CDegradableInfo / CEngineGraphic.
class CCharStringData;
struct CDataOutputStreamWriter {
    virtual ~CDataOutputStreamWriter();                     // slot 0
    virtual void SetPosition(unsigned long position);       // slot 1
    virtual unsigned long GetPosition() const;              // slot 2
    virtual void Close();                                   // slot 3
    virtual void Write(const void* data, long size);        // slot 4
    CCharStringData* NewLine_PStringData;                   // +0x04
    void WriteSLONG(long value);                            // 0x00993EB0
    void WriteFloat(float value);                           // 0x00993EE0
    void WriteEBOOL(bool value);                            // 0x00993E30
};

#pragma pack(push, 1)
class CEngineGraphic {                                      // 14 bytes (Ego_r.pdb)
public:
    unsigned long BankIndex;                                // +0x00
    float         AnimStep;                                 // +0x04
    float         RenderSizeX;                              // +0x08
    bool          AdditiveAlpha;                            // +0x0c
    unsigned char Type;                                     // +0x0d
};
#pragma pack(pop)

class CDegradableInfo {                                     // 28 bytes
public:
    float          HealthPercentage;                        // +0x00
    CEngineGraphic EngineGraphic;                           // +0x04
    long           SmashParticleEmitter;                    // +0x14
    bool           BlocksNavigation;                        // +0x18
};

namespace std {
template <class T> class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    T* begin() { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}

const long DEGRADABLE_INFO_RECORD_CHECK = 0x01AB6CE3;       // written after every record

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CDegradableInfo> {
    CDegradableInfo DefaultVal;
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const CDegradableInfo* value) const
    {
        stream.WriteFloat(value->HealthPercentage);
        stream.Write(&value->EngineGraphic, sizeof(CEngineGraphic));
        stream.WriteSLONG(value->SmashParticleEmitter);
        stream.WriteEBOOL(value->BlocksNavigation);
        stream.WriteSLONG(DEGRADABLE_INFO_RECORD_CHECK);
    }
};

template <class T>
void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStreamWriter& stream, std::vector<T>& value)
{
    CPersistTraits<T> traits;
    long count = value.size();
    stream.WriteSLONG(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryOut(stream, &value[i]);
}

template void __fastcall GFSerialiseVectorBinaryOut<CDegradableInfo>(CDataOutputStreamWriter& stream,
                                                                     std::vector<CDegradableInfo>& value);