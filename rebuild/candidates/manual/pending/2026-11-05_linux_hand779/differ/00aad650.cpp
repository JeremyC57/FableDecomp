// CWaveLoader::LoadWaveFile(CDataInputStream&) -- parse a RIFF/WAVE stream: the "fmt " chunk's PCM header and the "data" chunk's 8- or 16-bit samples into a new reference-counted CWaveFile; a bad tag, a non-PCM format or an unsupported sample width throws CWaveFileException
// Grade: DIFFER(1575v1574) -- residue: register colouring at two inline reads. The `fmt ` chunk's FormatTag and BlockAlign reads lower `StreamPos + 2` as retail `mov ecx,[ebp+4] ; add ecx,2` where ours emits `mov eax,[ebp+4] ; lea ecx,[eax+2]` (the other four reads of the chunk match); the extra byte is the trailing int3 the 1574-byte extent keeps. 16 grades.
// Next idea: the two odd sites are the first read of the chunk and the one after SamplesPerSec / AvgBytesPerSec (two ReadULONG calls): try reading the six header fields into locals first (`unsigned short formatTag = stream.ReadUWORD();`) and assigning the struct afterwards, or a CWaveFormatHeader::Read(stream) helper; also try the FormatTag / BlockAlign reads as `ReadType<unsigned short>()` with the Channels / BitsPerSample reads as ReadUWORD (probe s mixed them the other way).
// Leaf: ?LoadWaveFile@CWaveLoader@@QAE?AV?$CCountedPointer@VCWaveFile@@@@AAVCDataInputStream@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AAD650, 1574 bytes (0x00AAD650..0x00AADC76: the body, `ret 8` at 0x00AADC45, then the two
//   cold throw blocks); the 1840-byte ledger row runs on into the int3 padding and the next COMDATs (vector<short>(n) 0x00AADD80 ...).
//   thiscall (`this` unused), hidden result pointer, no EH frame (the module is built without /EHsc).
// Identity: Ego_r.pdb ?LoadWaveFile@CWaveLoader@@QAE?AV?$CCountedPointer@VCWaveFile@@@@AAVCDataInputStream@@@Z; the only caller
//   0x00A4289F; literals "RIFF" 0x0129E780, "WAVE" 0x0129E798, "fmt " 0x0129E790, "data" 0x0129E788.
// Callees: CDataInputStream::ReadString(unsigned long) 0x00996BE0, CCharString::operator!=(const char*) 0x0099E960,
//   CBasicString<char>::operator==(const char*) 0x004115A0 (from the inline CCharString::operator==; the null-string arm is the
//   strcmp intrinsic, `repe cmpsb` against ""), ~CCharString 0x0099EAE0, CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0
//   (the inline Read's slow path), GetPosition (vtable slot 2) / SetPosition (slot 1), vector<short>(n) 0x00AADD80 and
//   vector<short>::operator= 0x00AADE80 (CArray<short> stand-ins, same TU), CCountedPointer<CWaveFile>::Reset 0x00AADFF0,
//   operator new 0x00BFEA1A, operator delete 0x00BFE9BC, free 0x00BFEA14, _CxxThrowException 0x00BFEB84 with ThrowInfo 0x01369580.
// Shape: the exception temporaries and the ReadType<unsigned char> temporary are homed in the dead `stream` argument slot;
//   the format header's AvgBytesPerSec / BitsPerSample stores are dead (read into temporaries only); SampleLength is
//   AudioData.size() (signed pointer difference, `sar`) divided unsigned by Channels.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_READSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#include "fable_string.h"
#include "engine/CDataInputStream.h"
#include "engine/CWaveFile.h"
#include "engine/CWaveLoader.h"
#include "engine/CWaveFileException.h"

// Row-local template bodies (the landed 0x009BB110 precedent): fable_string.h declares CBasicString<T>::Compare 0x00411570 and
// operator==(const T*) 0x004115A0; retail's TU holds their bodies -- VC7.1 calls operator== out of line here (0x00AAD7FB,
// 0x00AADA29) but, seeing that it writes nothing, keeps chunkId's PStringData in esi across the call for the "data" compare.
template<class T> inline long __fastcall CBasicString<T>::Compare(const T* a,const T* b)
{
    while(*a || *b) {
        if(*a<*b)return -1;
        if(*a>*b)return 1;
        ++a; ++b;
    }
    return 0;
}
template<class T> inline bool CBasicString<T>::operator==(const T* text) const
{ return Compare(PData,text)==0; }

CCountedPointer<CWaveFile> CWaveLoader::LoadWaveFile(CDataInputStream& stream)
{
    CCountedPointer<CWaveFile> result;

    CCharString tag = stream.ReadString(4);
    if (tag != "RIFF")
        throw CWaveFileException();
    unsigned long riffSize = stream.ReadULONG();
    unsigned long riffEnd = stream.GetPosition() + riffSize;

    CCharString form = stream.ReadString(4);
    if (form != "WAVE")
        throw CWaveFileException();

    CWaveFormatHeader format;
    while (stream.GetPosition() < riffEnd) {
        CCharString chunkId = stream.ReadString(4);
        unsigned long chunkSize = stream.ReadULONG();
        unsigned long chunkStart = stream.GetPosition();
        if (chunkId == "fmt ") {
            format.FormatTag = stream.ReadUWORD();
            format.Channels = stream.ReadUWORD();
            format.SamplesPerSec = stream.ReadULONG();
            format.AvgBytesPerSec = stream.ReadULONG();
            format.BlockAlign = stream.ReadUWORD();
            format.BitsPerSample = stream.ReadUWORD();
        } else if (chunkId == "data") {
            if (format.FormatTag != 1)
                throw CWaveFileException();
            unsigned long sampleCount = chunkSize / format.BlockAlign * format.Channels;
            CArray<short> samples(sampleCount);
            long bitsPerSample = format.BlockAlign * 8 / format.Channels;
            if (bitsPerSample == 16) {
                for (unsigned long i = 0; i < sampleCount; i++)
                    samples[i] = stream.ReadSWORD();
            } else if (bitsPerSample == 8) {
                for (unsigned long i = 0; i < sampleCount; i++)
                    samples[i] = (short)(((unsigned short)stream.ReadUBYTE() - 0x80) << 8);
            } else {
                throw CWaveFileException();
            }
            result.Reset(new CWaveFile);
            result->Channels = format.Channels;
            result->SamplingFrequency = format.SamplesPerSec;
            result->AudioData = samples;
            result->SampleLength = result->AudioData.size() / result->Channels;
        }
        stream.SetPosition(chunkStart + chunkSize);
    }
    return result;
}
