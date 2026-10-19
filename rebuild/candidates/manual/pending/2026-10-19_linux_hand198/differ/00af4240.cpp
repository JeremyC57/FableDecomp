// Grade: DIFFER(219v219) -- only the strength-reduced walk pointer: retail biases it to the entry +0x14 (InstanceName; component at -0x0c, flag at -4), ours to the instance state at +8; tried a plain iterator loop (registers differ) and the landed state-pointer walk
// NParticleEngine::CParticleSystem::WriteBinary(CDataOutputStream&) -- save the system's name, its Enabled and ScaleParticles flags, its scale, then the component count and, per component, its name, instance name, enabled flag and body followed by a '{' marker byte
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF4240, length 224 bytes (`ret 4`; manifest label correct).
// Callees: CDataOutputStream::WriteNullTerminatedString @ 0x00995AE0 (x2), WriteBool @ 0x00993E30 (x3), Write3DVector
//          @ 0x00993FD0, WriteULong @ 0x00993EA0 (x2), WriteCBYTE @ 0x00993E50 ('{' = 0x7b); ~CCharString @ 0x0099EAE0;
//          the component's virtual slots 3 (+0x0c, its type name by value) and 16 (+0x40, WriteBinary).
// Layout: the retail CParticleSystem of the landed PreparePrimitives 0x00AF40B0 (16-byte component map at +0x0c of
// 24-byte pair<EPSCType, CComponentInstance>, flag byte +0x50: Enabled bit 1, ScaleParticles bit 7), plus Scale
// (C3DVector, +0x60) and Name (CCharString, +0x6c).
// Shape: the count is the signed 24-byte difference; the walk keeps the entry pointer in a frame slot and a
// pointer to its instance name in ebx (the component and flag read at -0x0c / -0x04 from it); the name temporary
// is destroyed right after it is written.
struct C3DVector { float X, Y, Z; };

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                // 0x0099EAE0
};

class CDataOutputStream {
public:
    void WriteBool(bool value);                    // 0x00993E30
    void WriteCBYTE(char value);                   // 0x00993E50
    void WriteULong(unsigned long value);          // 0x00993EA0
    void Write3DVector(const C3DVector& value);    // 0x00993FD0
    void WriteNullTerminatedString(const CCharString& s);   // 0x00995AE0
};

struct CParticleUpdateParameters { unsigned char m_data[0x24]; };   // retail 0x24 bytes (PDB 0x2c)

namespace NParticleEngine {
struct CPSCBase {                                  // particle-system component; slots by retail call offsets
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CCharString GetName();                                     // slot 3 (+0x0c)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void WriteBinary(CDataOutputStream& stream);               // slot 16 (+0x40)
};

struct SCountBlock;
template <class T> struct CCountedPointer {
    T*           Object;
    SCountBlock* Block;
    T* operator->() const { return Object; }
};
}

// Tail of NParticleEngine::CComponentInstance after its vptr (PDB: vptr +0, Component +4, Enabled +0xc, InstanceName +0x10).
struct SComponentState {
    NParticleEngine::CCountedPointer<NParticleEngine::CPSCBase> Component;   // entry +0x08
    bool          Enabled;                                                  // entry +0x10
    unsigned long InstanceName;                                             // entry +0x14
};

struct SComponentEntry {                           // pair<EPSCType, CComponentInstance>, 24 bytes
    long            first;                         // +0x00 EPSCType
    void*           m_instanceVtable;              // +0x04 CComponentInstance vptr
    SComponentState second;                        // +0x08
};

struct CParticleSystem {
    void*                     m_vtable;            // +0x00
    void*                     PParentEmitter;      // +0x04
    void*                     PEngine;             // +0x08
    SComponentEntry*          m_componentsFirst;   // +0x0c  Components (CVectorMap: first/last/end, compare, dirty)
    SComponentEntry*          m_componentsLast;    // +0x10
    SComponentEntry*          m_componentsEnd;     // +0x14
    unsigned char             m_componentsCompare; // +0x18
    bool                      m_componentsDirty;   // +0x19
    void*                     Particles;           // +0x1c  (STLport list head node)
    void*                     Decals;              // +0x20
    CParticleUpdateParameters UpdateParameters;    // +0x24
    long                      NoActiveParticles;   // +0x48
    long                      NoActiveDecals;      // +0x4c
    bool                      Initialised : 1;     // +0x50 bit 0
    bool                      Enabled : 1;
    bool                      Dead : 1;
    bool                      NormalisedTimeEnabled : 1;
    bool                      UseAllAttractors : 1;
    bool                      Visible : 1;
    bool                      StayWithEmitter : 1;
    bool                      ScaleParticles : 1;  // bit 7
    float                     GlobalAlpha;         // +0x54
    float                     FadeAlpha;           // +0x58
    float                     NormalisedTime;      // +0x5c
    C3DVector                 Scale;               // +0x60
    CCharString               Name;                // +0x6c
    void WriteBinary(CDataOutputStream& stream);
};

void CParticleSystem::WriteBinary(CDataOutputStream& stream)
{
    stream.WriteNullTerminatedString(Name);
    stream.WriteBool(Enabled);
    stream.WriteBool(ScaleParticles);
    stream.Write3DVector(Scale);
    stream.WriteULong(m_componentsLast - m_componentsFirst);
    SComponentEntry* it = m_componentsFirst;
    if (it != m_componentsLast) {
        SComponentState* state = &it->second;
        do {
            NParticleEngine::CPSCBase* component = state->Component.Object;
            stream.WriteNullTerminatedString(component->GetName());
            stream.WriteULong(state->InstanceName);
            stream.WriteBool(state->Enabled);
            component->WriteBinary(stream);
            stream.WriteCBYTE('{');
            ++it;
            state = (SComponentState*)((char*)state + sizeof(SComponentEntry));
        } while (it != m_componentsLast);
    }
}
