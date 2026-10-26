// CWaveFile::ConvertToMono() -- average interleaved stereo 16-bit samples into a mono buffer
// Retail: Fable.exe @ 0x00AAD5D0, 116 bytes. PDB ?ConvertToMono@CWaveFile@@QAEXXZ
// Callees: CArray<short>::CArray(unsigned long) 0x00AADD80 (label "Vector_Construct_uint16_v2"),
// CArray<short>::operator=(const CArray<short>&) 0x00AADE80 (label "Vector_Assign_uint16"), the inlined
// CArray<short> destructor frees through 0x00BFEA14 (label "free"; it is the sized-delete/free import).
// CWaveFile (retail 24 bytes; PDB 28 with a 16-byte debug CArray): +0x00 Channels, +0x04 SampleLength,
// +0x08 SamplingFrequency, +0x0C CArray<short> AudioData (12-byte start/finish/end-of-storage vector).
// Audio/lip-sync layer: CWaveFile feeds CSpeechAnalyser / CLipSyncDataBank (replacement work later).
// Shape: only stereo files convert; AudioData and SampleLength are re-read every iteration (the short store can
// alias them), the mono buffer pointer is a register; sample = (L + R) >> 1 (arithmetic shift, no rounding).
// The mono buffer's start stays in edi through the assignment and the inlined destructor because the local does not
// escape: the constructor and operator= (both CArray<short> instantiations in retail's TU) are same-TU stand-ins.
extern "C" void __cdecl free(void* p);   // 0x00BFEA14

template <class T> class CArray { public:
    T* Data;            // +0x00
    T* DataEnd;         // +0x04
    T* StorageEnd;      // +0x08
    explicit CArray(unsigned long size);            // 0x00AADD80
    CArray& operator=(const CArray& other);         // 0x00AADE80
    ~CArray() { if (Data) free(Data); }
    T& operator[](unsigned long i) { return Data[i]; }
};

short* ConstructShorts_00aadd80(unsigned long size);
void AssignShorts_00aade80(short** data, short** dataEnd, short** storageEnd, const short* first, const short* last);
// Stand-ins for the CArray<short> constructor (0x00AADD80) and operator= (0x00AADE80), instantiated in retail's TU: the
// local's bounds pass by value, so it does not escape and its start stays in a register across the assignment.
template <> __declspec(noinline) CArray<short>::CArray(unsigned long size)
{
    Data = ConstructShorts_00aadd80(size);
    DataEnd = Data + size;
    StorageEnd = DataEnd;
}
template <> __declspec(noinline) CArray<short>& CArray<short>::operator=(const CArray<short>& other)
{
    short* data = Data;
    short* dataEnd = DataEnd;
    short* storageEnd = StorageEnd;
    AssignShorts_00aade80(&data, &dataEnd, &storageEnd, other.Data, other.DataEnd);
    Data = data;
    DataEnd = dataEnd;
    StorageEnd = storageEnd;
    return *this;
}

class CWaveFile { public:
    unsigned long Channels;            // +0x00
    unsigned long SampleLength;        // +0x04
    unsigned long SamplingFrequency;   // +0x08
    CArray<short> AudioData;           // +0x0C
    void ConvertToMono();
};

void CWaveFile::ConvertToMono()
{
    if (Channels == 2)
    {
        CArray<short> mono(SampleLength);
        short* out = &mono[0];
        for (unsigned long i = 0; i < SampleLength; ++i)
            out[i] = (short)((AudioData[i * 2] + AudioData[i * 2 + 1]) >> 1);
        Channels = 1;
        AudioData = mono;
    }
}