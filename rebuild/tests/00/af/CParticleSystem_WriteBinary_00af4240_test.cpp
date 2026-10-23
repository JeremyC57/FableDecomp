#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#endif
extern "C" int __cdecl printf(const char* format, ...);
// instance modelled WITH its vptr (pair.second at entry +4, as Ego_r.pdb): VC then biases the strength-reduced pointer to InstanceName (+0x14) like retail
// NParticleEngine::CParticleSystem::WriteBinary(CDataOutputStream&) -- save the system's name, its Enabled and ScaleParticles flags, its scale, then the component count and, per component, its name, instance name, enabled flag and body followed by a '{' marker byte
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF4240, length 224 bytes (`ret 4`; manifest label correct).
// Callees: CDataOutputStream::WriteNullTerminatedString @ 0x00995AE0 (x2), WriteBool @ 0x00993E30 (x3), Write3DVector
//          @ 0x00993FD0, WriteULong @ 0x00993EA0 (x2), WriteCBYTE @ 0x00993E50 ('{' = 0x7b); ~CCharString @ 0x0099EAE0;
//          the component's virtual slots 3 (+0x0c, its type name by value) and 16 (+0x40, WriteBinary).
// Layout: the retail CParticleSystem of the landed PreparePrimitives 0x00AF40B0 (16-byte component map at +0x0c of
// 24-byte pair<EPSCType, CComponentInstance>, flag byte +0x50: Enabled bit 1, ScaleParticles bit 7), plus Scale
// (C3DVector, +0x60) and Name (CCharString, +0x6c).
// Shape: the count is the signed 24-byte difference; the walk keeps the entry pointer in a frame slot and VC's
// strength-reduced pointer to its instance name in ebx (component and flag at -0x0c / -0x04); the name temporary
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

// NParticleEngine::CComponentInstance (Ego_r.pdb, 20 bytes): vptr +0, Component +4, Enabled +0xc, InstanceName +0x10.
struct CComponentInstance {
    void*         __vftable;                                                // entry +0x04
    NParticleEngine::CCountedPointer<NParticleEngine::CPSCBase> Component;   // entry +0x08
    bool          Enabled;                                                  // entry +0x10
    unsigned long InstanceName;                                             // entry +0x14
};

struct SComponentEntry {                           // pair<EPSCType, CComponentInstance>, 24 bytes
    long            first;                         // +0x00 EPSCType
    CComponentInstance second;                        // +0x04
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
    for (SComponentEntry* it = m_componentsFirst; it != m_componentsLast; ++it) {
        CComponentInstance& instance = it->second;
        NParticleEngine::CPSCBase* component = instance.Component.Object;
        stream.WriteNullTerminatedString(component->GetName());
        stream.WriteULong(instance.InstanceName);
        stream.WriteBool(instance.Enabled);
        component->WriteBinary(stream);
        stream.WriteCBYTE('{');
    }
}

// ---- test harness: a stream that logs every write ----
static unsigned long g_log[64]; static char g_kind[64]; static int g_n;
static int g_dtors;
static void Log(char k, unsigned long v) { if (g_n < 64) { g_kind[g_n] = k; g_log[g_n] = v; } ++g_n; }
CCharString::~CCharString() { ++g_dtors; }
void CDataOutputStream::WriteBool(bool value) { Log('b', value ? 1 : 0); }
void CDataOutputStream::WriteCBYTE(char value) { Log('c', (unsigned char)value); }
void CDataOutputStream::WriteULong(unsigned long value) { Log('u', value); }
void CDataOutputStream::Write3DVector(const C3DVector& value) { Log('v', (unsigned long)value.X); }
void CDataOutputStream::WriteNullTerminatedString(const CCharString& s) { Log('s', (unsigned long)s.PStringData); }
namespace NParticleEngine {
void CPSCBase::v00() {} void CPSCBase::v01() {} void CPSCBase::v02() {}
CCharString CPSCBase::GetName() { CCharString s; s.PStringData = 0; return s; }
void CPSCBase::v04() {} void CPSCBase::v05() {} void CPSCBase::v06() {} void CPSCBase::v07() {}
void CPSCBase::v08() {} void CPSCBase::v09() {} void CPSCBase::v10() {} void CPSCBase::v11() {}
void CPSCBase::v12() {} void CPSCBase::v13() {} void CPSCBase::v14() {} void CPSCBase::v15() {}
void CPSCBase::WriteBinary(CDataOutputStream&) {}
}
struct CTestComponent : NParticleEngine::CPSCBase {
    unsigned long m_tag;
    virtual CCharString GetName() { CCharString s; s.PStringData = (void*)m_tag; return s; }
    virtual void WriteBinary(CDataOutputStream& stream) { stream.WriteULong(m_tag + 1000); }
};
static int g_fail;
static void Check(int ok, const char* what) { if (!ok) { ++g_fail; printf("FAIL %s\n", what); } }
int main()
{
    static CParticleSystem ps;
    static SComponentEntry entries[2];
    static CTestComponent comps[2];
    comps[0].m_tag = 11; comps[1].m_tag = 22;
    entries[0].second.Component.Object = &comps[0]; entries[0].second.InstanceName = 501; entries[0].second.Enabled = true;
    entries[1].second.Component.Object = &comps[1]; entries[1].second.InstanceName = 502; entries[1].second.Enabled = false;
    ps.Name.PStringData = (void*)7;
    ps.Enabled = true; ps.ScaleParticles = false;
    ps.Scale.X = 3.0f;
    ps.m_componentsFirst = entries; ps.m_componentsLast = entries + 2; ps.m_componentsEnd = entries + 2;
    CDataOutputStream stream;
    ps.WriteBinary(stream);
    static const char kinds[] = "sbbvusubucsubuc";
    static const unsigned long vals[] = { 7, 1, 0, 3, 2, 11, 501, 1, 1011, '{', 22, 502, 0, 1022, '{' };
    Check(g_n == 15, "write count");
    for (int i = 0; i < 15 && i < g_n; ++i) {
        if (g_kind[i] != kinds[i] || g_log[i] != vals[i]) { printf("at %d: %c %lu\n", i, g_kind[i], g_log[i]); Check(0, "write sequence"); }
    }
    Check(g_dtors >= 2, "name temporaries destroyed");
    g_n = 0; ps.m_componentsLast = entries;
    ps.WriteBinary(stream);
    Check(g_n == 5 && g_kind[4] == 'u' && g_log[4] == 0, "empty map");
    printf(g_fail ? "00af4240_TEST FAIL\n" : "00af4240_TEST PASS\n");
    return g_fail;
}