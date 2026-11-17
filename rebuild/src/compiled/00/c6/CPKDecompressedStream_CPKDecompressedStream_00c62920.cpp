// CPKDecompressedStream::CPKDecompressedStream(PKCFileManager* opener, const unsigned short* name, const PKCBufferedReaderParams& segment) -- the 0x14-byte owner of a sound's decoded stream: no source, no stream, position -1, not finished; with the test-tone switch set it owns a new tone stream (constructed with 800.0f) instead; otherwise it opens the named source through the opener (slot 0) and, when that succeeds, aligns the segment start up to the source's block size (slot 2) and creates the matching decoder stream for the name (0x00C73AF0)
// Leaf: ??0CPKDecompressedStream@@QAE@PAVPKCFileManager@@PBGABUPKCBufferedReaderParams@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C62920, 256 bytes, thiscall `ret 0Ch`, returns `this`; EH frame (state 0 around the tone
//   stream's constructor after the scalar operator new 0x00BFEA1A of 0x10 bytes). The segment copy is a frame local.
// Identity: the only caller 0x00C4812E constructs it in a fresh 0x14-byte block (operator new 0x14). Its accessors are landed as
//   0x00C628A0 / 0x00C628D0 / 0x00C628F0 (the stream's slots 1 / 4 / 5 or -1 without a stream) and 0x00C628C0 (+0x0C, 0 without a
//   stream) under propagated names ("CAIBrain::GetCurrentStateGroupDebugText", "CEditWorld::DrawGetWorldMap",
//   "CHandleOwner_00c628f0::GetIndex"). libpk has no PDB/RTTI: class and member names are descriptive; the catalog's
//   "C3DGroup2_Create" is propagated.
// Callees: operator new @ 0x00BFEA1A, the tone stream CTestToneAudioStream constructor @ 0x00C73EF0 (it ignores its argument; the caller
//   pushes the float 800.0f 0x44480000), the opener's slot 0, the source's slot 2, the decoder-stream factory @ 0x00C73AF0
//   (__fastcall: source in ecx, name in edx, the segment by reference on the stack; it classifies the name's extension).
// Global: the test-tone switch 0x0143C5B8 (bool).
// NF53: the stream classes are the shared libpk/IAudioStream.h ones (CTestToneAudioStream, ctor 0x00C73EF0 taking the ignored float;
//   CreateDecoderStream 0x00C73AF0 with the PKCBufferedReaderParams segment by reference). Was CVtable_012bf014 / SStreamSegment /
//   CreateDecoderStream_00c73af0 stand-ins (same bytes).
#define FABLE_RETAIL_LIBPK_DECODER_FACTORY_PASS23
#include "libpk/IAudioStream.h"
// agentNF57 (q10485): the class is the landed descriptive CPKDecompressedStream (dtor 0x00C62750, Seek 0x00C626A0, Read 0x00C62790):
//   the same 0x14-byte object (source +0, stream +4, position +8, +0x0C, flag +0x10), and the only caller 0x00C47F10 builds it with
//   `new` 0x14 here and destroys it with 0x00C62750. The opener is libpk/PKCFileManager.h's PKCFileManager (slot 0
//   Open(const unsigned short*) returning the CAudioFile source) and the source libpk/CAudioFile.h's CAudioFile (slot 2
//   GetUnitSize = the alignment read here; slot 3 Close = what the dtor calls). Was CAudioStreamOwner_00c62920 /
//   IStreamOpener_00c62920 / IStreamSource_00c62920 placeholders.
#define FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
#include "libpk/PKCFileManager.h"


inline unsigned long AlignUp_00c62920(unsigned long value, unsigned long alignment)
{
    return (value + alignment - 1) / alignment * alignment;
}

extern bool g_useTestToneStream;            // 0x0143C5B8

class CPKDecompressedStream {               // 0x14 bytes
public:
    CPKDecompressedStream(PKCFileManager* opener, const unsigned short* name, const PKCBufferedReaderParams& segment);
    CAudioFile*             m_source;       // +0x00
    IAudioStream*           m_stream;       // +0x04
    long                    m_position;     // +0x08 (-1)
    long                    m_0c;           // +0x0c
    bool                    m_finished;     // +0x10
};

CPKDecompressedStream::CPKDecompressedStream(PKCFileManager* opener, const unsigned short* name, const PKCBufferedReaderParams& segment)
    : m_source(0), m_stream(0), m_position(-1), m_finished(false)
{
    if (g_useTestToneStream) {
        m_stream = new CTestToneAudioStream(800.0f);
        return;
    }
    m_source = opener->Open(name);
    if (m_source) {
        PKCBufferedReaderParams aligned = segment;
        aligned.Granularity = AlignUp_00c62920(aligned.Granularity, m_source->GetUnitSize());
        m_stream = CreateDecoderStream(m_source, name, aligned);
    }
}