// 00aad650 CWaveLoader::LoadWaveFile
// Leaf: ?LoadWaveFile@CWaveLoader@@QAE?AV?$CCountedPointer@VCWaveFile@@@@AAVCDataInputStream@@@Z
// Retail: 0x00AAD650,1575 bytes,public thiscall returning CCountedPointer<CWaveFile>.
// Flags: none
#define FABLE_RETAIL_CWAVELOADER_PASS22_EXCEPTIONS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_READSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_C
#include "fable_string.h"
#include "engine/CDataInputStream.h"
#include "engine/CWaveFile.h"
#include "engine/CWaveLoader.h"
#include "engine/CWaveFileException.h"

template<class T> inline long FABLE_FASTCALL CBasicString<T>::Compare(const T* a,const T* b)
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

// Original STLport4.6 sized-vector constructor; native called body00AADD80, not a declaration-only surrogate.
namespace std {
template<> vector<short>::vector(size_type n)
    : _Vector_base<short,allocator<short> >(n,allocator<short>())
{
    this->_M_finish=uninitialized_fill_n(this->_M_start,n,short());
}
}

// Original donor033FF070 and PDB params chan,freq,data; expanded in native00AADB85..00AADBBA.
inline void CWaveFile::Initialise(unsigned long chan,unsigned long freq,const CArray<short>& data)
{
    Channels=chan;
    SamplingFrequency=freq;
    AudioData=data;
    SampleLength=AudioData.size()/Channels;
}

CCountedPointer<CWaveFile> CWaveLoader::LoadWaveFile(CDataInputStream& is)
{
    CCountedPointer<CWaveFile> wavefile;

    CCharString file_header = is.ReadString(4);
    if (file_header != "RIFF")
        throw CWaveFileException("Not a WAV file!");
    unsigned long total_length = is.ReadULONG();
    unsigned long end_pos = is.GetPosition() + total_length;

    CCharString datatype = is.ReadString(4);
    if (datatype != "WAVE")
        throw CWaveFileException("Not a WAV file!");

    CWaveFormatHeader wave_format;
    while (is.GetPosition() < end_pos) {
        CCharString chunk_header = is.ReadString(4);
        unsigned long chunk_length = is.ReadULONG();
        unsigned long chunk_start = is.GetPosition();
        if (chunk_header == "fmt ") {
            wave_format.FormatTag = is.ReadUWORD();
            wave_format.Channels = is.ReadUWORD();
            wave_format.SamplesPerSec = is.ReadULONG();
            wave_format.AvgBytesPerSec = is.ReadULONG();
            wave_format.BlockAlign = is.ReadUWORD();
            wave_format.BitsPerSample = is.ReadUWORD();
        } else if (chunk_header == "data") {
            if (wave_format.FormatTag == 1) {
            unsigned long samples = chunk_length / wave_format.BlockAlign;
            unsigned long size = samples * wave_format.Channels;
            CArray<short> audio_data(size);
            unsigned long bits = wave_format.BlockAlign * 8 / wave_format.Channels;
            if (bits == 16) {
                // Donor033FF2E7 and033FF32B retain the original constant-false loop-scope guard.
                if (false) {} else for (unsigned long c1 = 0; c1 < size; c1++)
                    audio_data[c1] = is.ReadSWORD();
            } else if (bits == 8) {
                // Donor033FF2E7 and033FF32B retain the original constant-false loop-scope guard.
                if (false) {} else for (unsigned long c1 = 0; c1 < size; c1++)
                    audio_data[c1] = (short)(((unsigned short)is.ReadUBYTE() - 0x80u) << 8);
            } else {
                throw CWaveFileException("WAV data is not 16 or 8 bits!");
            }
            wavefile.Reset(new CWaveFile);
            wavefile->Initialise(wave_format.Channels,wave_format.SamplesPerSec,audio_data);
            } else {
                throw CWaveFileException("Unsupported WAV compression!");
            }
        }
        is.SetPosition(chunk_start + chunk_length);
    }
    return wavefile;
}