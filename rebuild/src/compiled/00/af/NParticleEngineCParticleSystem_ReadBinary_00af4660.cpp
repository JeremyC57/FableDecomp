// NParticleEngine::CParticleSystem::ReadBinary(CDataInputStream* in) -- load a particle system template: its name, the Enabled and ScaleParticles flags, the scale vector, then every component record (type name, instance name, enabled flag), each created through AddComponent, read by the component itself and followed by one separator byte
// Leaf: ReadBinary
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF4660, length 672 bytes (`ret 4`).
// Callees: the stream's virtual ReadNullTerminatedString (slot 6, CCharString by value), CCharString::operator= @ 0x0099EFB0,
//   ~CCharString @ 0x0099EAE0, CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the slow path of the inlined Read),
//   CDataInputStream::Read3DVector @ 0x009950F0, CParticleSystem::AddComponent @ 0x00AF4530, the component's virtual
//   ReadBinary (slot 15, +0x3c).
// Layout: retail CParticleSystem (Ego_r.pdb minus 0x1c in front of the flags, as the landed 0x00AEE1E0 notes): the bool
//   bit-field byte +0x50 (Initialised, Enabled, Dead, NormalisedTimeEnabled, UseAllAttractors, Visible, StayWithEmitter,
//   ScaleParticles), Scale +0x60, Name +0x6c. CDataInputStream is the landed 0x00B16F60 model (inline Read with the
//   64-bit position range check). Local models: no engine header carries the inline Read.
// Shape: the flags, the count and the instance name go through __forceinline ReadUByte/ReadULong helpers whose own
//   locals share the dead `in` argument slot with the name temporary (the count is forwarded into edi on the fast
//   path, the instance name copied into its named local); only the separator byte is a named local of its own.
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)
class CCharStringData;
class CCharString { public:
    CCharStringData* m_pData;
    const CCharString& operator=(const CCharString& other);                  // 0x0099EFB0
    ~CCharString();                                                    // 0x0099EAE0
};
class C3DVector { public:
    float X, Y, Z;
};
class CDataInputStream { public:
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5();
    virtual CCharString ReadNullTerminatedString();                    // slot 6 (+0x18)
    unsigned long StreamPos;                                           // +0x04
    long StreamSize;                                                   // +0x08
    const unsigned char* CurrentSourcePtr;                             // +0x0c
    unsigned long SourceChunkStreamPos;                                // +0x10
    long SourceChunkBytesRemaining;                                    // +0x14
    C3DVector Read3DVector();                                          // 0x009950F0
    static __forceinline bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
    __forceinline void Read(void* dst, long size)
    {
        __int64 np = (__int64)(StreamPos + size);
        if (!IsValidPosition(np))
            return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(dst, CurrentSourcePtr, size);
            CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        } else
            ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
    __forceinline unsigned char ReadUByte() { unsigned char value; Read(&value, 1); return value; }
    __forceinline unsigned long ReadULong() { unsigned long value; Read(&value, 4); return value; }
private:
    void ReadWithSrcChunkOverflow(unsigned char*, long);   // 0x00993CA0
};
namespace NParticleEngine {
class CPSCBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void ReadBinary(CDataInputStream* in);                     // slot 15 (+0x3c)
};
class CParticleSystem {
public:
    void*         __vftable;                                           // +0x00
    unsigned char _state04[0x4c];                                      // +0x04 emitter, engine, components, lists, parameters
    bool Initialised : 1;                                              // +0x50
    bool Enabled : 1;
    bool Dead : 1;
    bool NormalisedTimeEnabled : 1;
    bool UseAllAttractors : 1;
    bool Visible : 1;
    bool StayWithEmitter : 1;
    bool ScaleParticles : 1;
    float GlobalAlpha;                                                 // +0x54
    float FadeAlpha;                                                   // +0x58
    float NormalisedTime;                                              // +0x5c
    C3DVector Scale;                                                   // +0x60
    CCharString Name;                                                  // +0x6c
    CPSCBase& AddComponent(const CCharString& typeName, const unsigned long& instanceName, bool enabled, bool unused);   // 0x00AF4530
    void ReadBinary(CDataInputStream* in);
};

void CParticleSystem::ReadBinary(CDataInputStream* in)
{
    Name = in->ReadNullTerminatedString();
    Enabled = in->ReadUByte() != 0;
    ScaleParticles = in->ReadUByte() != 0;
    Scale = in->Read3DVector();
    unsigned long count = in->ReadULong();
    for (unsigned long i = 0; i < count; ++i) {
        CCharString typeName = in->ReadNullTerminatedString();
        unsigned long instanceName = in->ReadULong();
        bool enabled = in->ReadUByte() != 0;
        AddComponent(typeName, instanceName, enabled, false).ReadBinary(in);
        unsigned char separator;
        in->Read(&separator, 1);
    }
}
}