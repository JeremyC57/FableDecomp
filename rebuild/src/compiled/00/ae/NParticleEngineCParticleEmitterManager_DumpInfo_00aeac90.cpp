// NParticleEngine::CParticleEmitterManager::DumpInfo(const CWideString& fileName) -- write a text dump of every live emitter (name, position, and a Dead/Alive line per particle system) to the named file
// Leaf: DumpInfo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AEAC90, length 546 bytes (`ret 4`, 8-byte aligned frame).
// Callees: CCharString() @ 0x0099E4B0, ~CCharString @ 0x0099EAE0, operator+=(const CCharString&) @ 0x0099F0A0,
//   operator+=(const char*) @ 0x0099F100, operator+(const char*, const CCharString&) @ 0x0099F690,
//   operator+(const CCharString&, const char*) @ 0x0099F600, operator+(const CCharString&, const CCharString&) @ 0x0099F570,
//   CCharString::FormatDouble @ 0x009A08B0 (all __fastcall, hidden result in ecx), CParticleEmitter::GetNoParticles
//   @ 0x00AEDCA0, CDiskFileWin32(const CWideString&, FileAccessType, unsigned long) @ 0x0099AD80 and its inline
//   destructor (vtable 0x0122D06C, Close 0x0099A920, ~CWideString 0x0099B510, ~CBaseClassNonCopyable 0x0099A300),
//   CCharString::SaveToFile(CAFile&) @ 0x0099E5F0.
// Layout: the manager's emitter list is an STLport list<CParticleEmitter*> (head node pointer at +0); emitter
//   ParticleSystems +0x08, Transform translation +0xd4, Name +0x140 (landed 0x00AEBAF0/0x00AEDAF0 models); the
//   system's Dead flag is bit 2 of its flag byte +0x50.
// Shape: CDiskFileWin32 is the landed 0x006F9BE0 local model. Each coordinate goes through its own by-value
//   `GetPosition()` temporary (C3DVector with a user copy ctor): VC7.1 then loads Y and X up front into separate 8-byte
//   frame slots ([esp+48h], [esp+50h]) and feeds Z straight to the x87, as retail does. One named C3DVector copy packs
//   the fields adjacently (frame 0x64), float locals fold into reloads (frame 0x5c).
class CWideString { public:
    void* PStringData;
    ~CWideString();                                    // 0x0099B510
};
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();                                          // 0x0099A300
};
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CAFile : public CBaseClassNonCopyable { public:
};
class CDiskFileWin32 : public CAFile { public:                       // vtable 0x0122D06C, 0x1c bytes
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // slot 1
    virtual void Close();                              // 0x0099A920 (slot 2)
    long        AccessType;                            // +0x04
    long        Position;                              // +0x08
    long        Length;                                // +0x0c
    void*       WinFileHandle;                         // +0x10
    bool        Opened;                                // +0x14
    CWideString Pathname;                              // +0x18
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    ~CDiskFileWin32() { Close(); }
};
class CCharString { public:
    void* PStringData;
    CCharString();                                     // 0x0099E4B0
    ~CCharString();                                    // 0x0099EAE0
    const CCharString& operator+=(const CCharString& other); // 0x0099F0A0
    const CCharString& operator+=(const char* text);         // 0x0099F100
    void SaveToFile(CAFile& file) const;               // 0x0099E5F0
    static CCharString __fastcall FormatDouble(long digits, double value);   // 0x009A08B0
};
CCharString __fastcall operator+(const CCharString& a, const CCharString& b);   // 0x0099F570
CCharString __fastcall operator+(const CCharString& a, const char* b);          // 0x0099F600
CCharString __fastcall operator+(const char* a, const CCharString& b);          // 0x0099F690
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};
namespace NParticleEngine {
class CParticleSystem { public:
    unsigned char _state00[0x50];
    bool Initialised : 1;                              // +0x50
    bool Enabled : 1;
    bool Dead : 1;
};
struct CParticleSystemPtr {                            // CCountedPointer<CParticleSystem>
    CParticleSystem* Data;
    void*            Info;
};
class CParticleEmitter { public:
    unsigned char       _base00[0x08];
    CParticleSystemPtr* ParticleSystems_start;         // +0x08
    CParticleSystemPtr* ParticleSystems_finish;        // +0x0c
    CParticleSystemPtr* ParticleSystems_end;           // +0x10
    unsigned char       _state14[0xc0];                // +0x14
    C3DVector           Position;                      // +0xd4 Transform translation
    unsigned char       _statee0[0x60];                // +0xe0
    CCharString         Name;                          // +0x140
    C3DVector GetPosition() const { return Position; }
    long GetNoParticles();                             // 0x00AEDCA0
};
struct SEmitterNode {                                  // STLport list node
    SEmitterNode*     _M_next;
    SEmitterNode*     _M_prev;
    CParticleEmitter* _M_data;
};
class CParticleEmitterManager { public:
    SEmitterNode* Emitters;                            // +0x00 STLport list<CParticleEmitter*> head
    void DumpInfo(const CWideString& fileName);
};

void CParticleEmitterManager::DumpInfo(const CWideString& fileName)
{
    CCharString info;
    for (SEmitterNode* it = Emitters->_M_next; it != Emitters; it = it->_M_next) {
        CParticleEmitter* emitter = it->_M_data;
        info += "Emitter " + emitter->Name;
        info += " Pos(" + CCharString::FormatDouble(6, emitter->GetPosition().X) + ", " + CCharString::FormatDouble(6, emitter->GetPosition().Y)
            + ", " + CCharString::FormatDouble(6, emitter->GetPosition().Z) + ")\r\n";
        long count = emitter->GetNoParticles();
        for (long i = 0; i < count; ++i) {
            CParticleSystem* system = emitter->ParticleSystems_start[i].Data;
            info += "\tSystem\r\n";
            if (system->Dead)
                info += "\t\tDead\r\n";
            else
                info += "\t\tAlive\r\n";
        }
    }
    CDiskFileWin32 file(fileName, FILE_ACCESS_WRITE, 2);
    info.SaveToFile(file);
}
}