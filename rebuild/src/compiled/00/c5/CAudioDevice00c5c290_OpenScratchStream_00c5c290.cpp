// CAudioDevice_00c5c290::OpenScratchStream() -- prepares the device, opens the embedded stream object at +0x1f0 on
// the 16-byte-aligned global scratch buffer (0x80 bytes, mode 2, the device's format word at +0x5c, no callbacks)
// and makes it the active stream (+0xd8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5C290, length 55 bytes.
// Callees: CAudioDevice_00c5c290::Prepare 0x00C5A2A0 (on `this`, no arguments; the manifest's "CMap::Update" label is
//          propagated), CAudioStream_00c5c290::Open 0x00C5CE00 (__fastcall on the member at +0x1f0, six arguments; the
//          manifest's nine-argument CReactionMatch constructor label is propagated garbage).
// The aligned buffer address is `(g_scratch + 15) & ~15`: VC7.1 folds the +15 into the relocation addend
// (`mov ecx, offset g_scratch+0Fh ; and ecx,0FFFFFFF0h`); the global lives at 0x0143BFA8. The chunk's
// CArenaRoundDef::Copy label does not fit (no def base, a 0x1f0+ byte audio object in the 0x00C5xxxx audio module).
extern unsigned char g_audioScratch[];                   // 0x0143BFA8

struct CAudioStream_00c5c290 {
    unsigned char _body[0x20];
    void Open(unsigned char* buffer, unsigned long bufferSize, long mode, unsigned long format,
              void* callback, void* callbackContext);    // 0x00C5CE00
};

struct CAudioDevice_00c5c290 {
    unsigned char _pad0[0x5c];
    unsigned long Format;                                // +0x5c
    unsigned char _pad1[0x78];
    CAudioStream_00c5c290* ActiveStream;                 // +0xd8
    unsigned char _pad2[0x114];
    CAudioStream_00c5c290 ScratchStream;                 // +0x1f0
    void Prepare();                                      // 0x00C5A2A0
    void OpenScratchStream();
};

void CAudioDevice_00c5c290::OpenScratchStream()
{
    Prepare();
    ScratchStream.Open((unsigned char*)(((unsigned long)g_audioScratch + 15) & ~15u), 0x80, 2, Format, 0, 0);
    ActiveStream = &ScratchStream;
}