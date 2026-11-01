// _global::CreateDecoderStream_00c73af0(IStreamSource_00c62920* source, const unsigned short* name, const SStreamSegment& segment) -- libpk's decoder-stream factory (.\CDecompressorInterface.cpp): convert the wide name to ANSI, classify its extension, and build the matching stream over the source segment: the null stream (4 bytes) for .wma, COggVorbisStream for .ogg, the test tone (a random 440..880 Hz frequency) when the tone switch is set; a failed allocation reports "EMEM" (lines 40 / 45) through the error hook and breaks; an unknown extension yields no stream
// Grade: DIFFER(671v672) -- block layout only: retail places the common epilogue right after the type-0 arm and the type-1 / type-2 arms after it (jumping back); VC7.1 here keeps the arms in order with one epilogue at the end; the arms themselves match
// Leaf: CreateDecoderStream_00c73af0
// Flags: /EHsc /GS
extern "C" __declspec(dllimport) int __cdecl rand(void);

struct SStreamSegment {                     // the landed COggVorbisStream model's segment (start, length, flags)
    unsigned long m_word0;
    unsigned long m_word4;
    unsigned long m_word8;
};

class IAudioStream {                        // vtable 0x012BEFFC (landed COggVorbisStream model)
public:
    virtual ~IAudioStream() {}
};

class CVtable_012bf1dc : public IAudioStream {   // libpk's null audio stream (landed ctor 0x00C888F0), 4 bytes
public:
    CVtable_012bf1dc(void* source, const SStreamSegment& segment);   // 0x00C888F0
};

class COggVorbisStream : public IAudioStream {   // 0x340 bytes (landed ctor 0x00C88780)
public:
    COggVorbisStream(void* source, const SStreamSegment& segment);   // 0x00C88780
    unsigned char m_body[0x340 - 4];
};

class CVtable_012bf014 : public IAudioStream {   // the test tone stream (landed 0x00C73EF0), 0x10 bytes
public:
    CVtable_012bf014(float frequency);      // 0x00C73EF0 (argument unused)
    float m_scale;                          // +4
    long  m_position;                       // +8
    long  m_sampleRate;                     // +0xc
};

class IStreamSource_00c62920;               // an opened file source (the landed caller 0x00C62920's model)

void __fastcall WideToMultiByte(const unsigned short* source, char* dest, int destSize);   // 0x00C16760

extern char* _afxPchNil;                    // 0x0139C900 (the shared nil buffer)
extern void (__fastcall* g_pfnErrorReport)(const char* text);   // 0x0143BB2C

class CString {                             // libpk MFC-style string: one word
public:
    char* m_pchData;                        // +0x00
    CString() { m_pchData = _afxPchNil; }
    CString(const char* lpsz);              // 0x00C0BFE0
    ~CString();                             // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CString& __fastcall operator<<(long value);         // 0x00C0DF00
};

struct CString_00c73a00 : CString {         // the landed 0x00C73A00 row's spelling of the string
    CString_00c73a00(const char* lpsz) : CString(lpsz) { }
    long ClassifyAudioExtension() const;    // 0x00C73A00: 1 = .ogg, 0 = .wma, 2 = test tone switch, 3 = other
};

struct CDebugBreak {                        // empty class whose constructor breaks into the debugger
    CDebugBreak();                          // 0x00C16750
};

// libpk's error report: "<file>(<line>) : <text>\n" to the installed report hook, then a debugger break.
#define PK_REPORT_ERROR(line, text)                                                                   \
    {                                                                                                 \
        {                                                                                             \
            CString s;                                                                                \
            s << ".\\CDecompressorInterface.cpp" << "(" << (long)(line) << ") : " << text << "\n";   \
            if (g_pfnErrorReport)                                                                     \
                g_pfnErrorReport(s.m_pchData);                                                        \
        }                                                                                             \
        CDebugBreak();                                                                                \
    }

IAudioStream* __fastcall CreateDecoderStream_00c73af0(IStreamSource_00c62920* source, const unsigned short* name, const SStreamSegment& segment)
{
    char path[0x100];
    WideToMultiByte(name, path, 0x100);
    long type = CString_00c73a00(path).ClassifyAudioExtension();
    IAudioStream* stream;
    if (type == 0) {
        stream = new CVtable_012bf1dc(source, segment);
        if (!stream)
            PK_REPORT_ERROR(40, "EMEM");
        return stream;
    }
    if (type == 1) {
        stream = new COggVorbisStream(source, segment);
        if (!stream)
            PK_REPORT_ERROR(45, "EMEM");
        return stream;
    }
    if (type == 2)
        return new CVtable_012bf014(((float)rand() * (1.0f / 32767.0f) + 1.0f) * 440.0f);
    return 0;
}
