// CParticleEmitterDatabase::GenerateHeaderFile(const CCharString& filename, long options) -- write the particle database's header text file: the header file name (relative to the .par file), then a BEGIN_EMITTER/END_EMITTER block per emitter (each emitter writes its own entry) inside BEGIN_DATABASE/END_DATABASE, and save the text to the given file
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE8810, length 368 bytes, `ret 8`.
// Callees: CCharString() 0x0099E4B0 / CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
// CPersistContext::CPersistContext(CCharString&) 0x009BAE10 (saving into the string; trivial destructor, as the landed
// CScriptBase::DoScriptDataPersist 0x00CB7CB0), CPersistContext::Transfer(CCharString&) 0x009BAB50 (text out; the helper's
// result temporary binds to its non-const reference through the VC7.1 extension), the
// __fastcall relative-path helper 0x00998570 (compares the two names, returns a CCharString; manifest label
// propagated), the emitter's header writer 0x00AEF1F0 (CPersistContext&, long), CCharString::SaveToFile 0x0099E5F0.
// Strings: "HeaderFilename \"" 0x0129F154, "\"\r\n" 0x01244608, "BEGIN_DATABASE\r\n" 0x0129F140, "BEGIN_EMITTER\r\n"
// 0x0129F130, "END_EMITTER\r\n" 0x0129F120, "END_DATABASE\r\n" 0x0129F110.
// Layout: Ego_r.pdb CParticleEmitterDatabase less 0xC (retail CSymbolMap is narrower): Emitters (CVectorMap<long,
// CCountedPointer<CParticleEmitter>>, 12-byte pairs) +0x28, ParFilename +0x44, HeaderFilename +0x48.
// Shape: each literal is a named CCharString local in its own block (`lea ecx ; push ecx`), the relative path is an
// argument temporary passed straight from the helper's result; the emitter walk is an iterator loop over the map's
// vector (`it != end()` re-read each pass).
class CCharString { public:
    void* m_pData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    void SaveToFile(const CCharString& filename);      // 0x0099E5F0
};
CCharString __fastcall GetRelativePathName(const CCharString& path, const CCharString& relativeTo);   // 0x00998570

class CPersistContext { public:                               // retail 0x2c bytes, trivial destructor
    unsigned char _pad00[0x2c];
    CPersistContext(CCharString& output);              // 0x009BAE10
    void Transfer_009bab50(CCharString& value);                 // 0x009BAB50
};

namespace NParticleEngine {
class CParticleEmitter { public:
    void GenerateHeader(CPersistContext& context, long options);   // 0x00AEF1F0
};
}

struct CEmitterEntry {                                 // pair<long, CCountedPointer<CParticleEmitter>>
    long                               Key;            // +0
    NParticleEngine::CParticleEmitter* Emitter;        // +4 (counted pointer data)
    void*                              EmitterInfo;    // +8
};

struct CParticleEmitterDatabase {
    unsigned char  _pad00[0x28];
    CEmitterEntry* Emitters_start;                     // +0x28 CVectorMap's vector
    CEmitterEntry* Emitters_finish;                    // +0x2c
    CEmitterEntry* Emitters_end;                       // +0x30
    unsigned char  _pad34[0x44 - 0x34];
    CCharString    ParFilename;                        // +0x44 (PDB +0x50)
    CCharString    HeaderFilename;                     // +0x48 (PDB +0x54)
    void GenerateHeaderFile(const CCharString& filename, long options);
};

void CParticleEmitterDatabase::GenerateHeaderFile(const CCharString& filename, long options)
{
    CCharString text;
    CPersistContext context(text);
    {
        CCharString line("HeaderFilename \"");
        context.Transfer_009bab50(line);
    }
    context.Transfer_009bab50(GetRelativePathName(ParFilename, HeaderFilename));
    {
        CCharString line("\"\r\n");
        context.Transfer_009bab50(line);
    }
    {
        CCharString line("BEGIN_DATABASE\r\n");
        context.Transfer_009bab50(line);
    }
    for (CEmitterEntry* it = Emitters_start; it != Emitters_finish; ++it) {
        {
            CCharString line("BEGIN_EMITTER\r\n");
            context.Transfer_009bab50(line);
        }
        it->Emitter->GenerateHeader(context, options);
        {
            CCharString line("END_EMITTER\r\n");
            context.Transfer_009bab50(line);
        }
    }
    {
        CCharString line("END_DATABASE\r\n");
        context.Transfer_009bab50(line);
    }
    text.SaveToFile(filename);
}