// CWaveFile::ConvertToMono() -- average interleaved stereo 16-bit samples into a mono buffer
// Grade: DIFFER(120v116) -- retail keeps mono.Data in edi from before the loop through the inlined vector dtor after the operator= call (needs mod-ref knowledge of 0x00AADE80, LTCG-like); ours reloads the local after the call
// Retail: Fable.exe @ 0x00AAD5D0, 116 bytes. PDB ?ConvertToMono@CWaveFile@@QAEXXZ
// Callees: CArray<short>::CArray(unsigned long) 0x00AADD80 (label "Vector_Construct_uint16_v2"),
// CArray<short>::operator=(const CArray<short>&) 0x00AADE80 (label "Vector_Assign_uint16"), the inlined
// CArray<short> destructor frees through 0x00BFEA14 (label "free"; it is the sized-delete/free import).
// CWaveFile (retail 24 bytes; PDB 28 with a 16-byte debug CArray): +0x00 Channels, +0x04 SampleLength,
// +0x08 SamplingFrequency, +0x0C CArray<short> AudioData (12-byte start/finish/end-of-storage vector).
// Audio/lip-sync layer: CWaveFile feeds CSpeechAnalyser / CLipSyncDataBank (replacement work later).
// Shape: only stereo files convert; AudioData and SampleLength are re-read every iteration (the short store can
// alias them), the mono buffer pointer is a register; sample = (L + R) >> 1 (arithmetic shift, no rounding).
extern "C" void __cdecl free(void* p);   // 0x00BFEA14
extern "C" void* __cdecl malloc(unsigned int n);   // 0x00BFEA0E

template <class T> struct CArray {
    T* Data;            // +0x00
    T* DataEnd;         // +0x04
    T* StorageEnd;      // +0x08
    explicit CArray(unsigned long size);            // 0x00AADD80
    CArray& operator=(const CArray& other);         // 0x00AADE80
    ~CArray() { if (Data) free(Data); }
    T& operator[](unsigned long i) { return Data[i]; }
};

#pragma auto_inline(off)
// STLport vector<short>::operator= (0x00AADE80): reallocate when the source is larger than the capacity,
// otherwise copy in place; defined here so VC7.1 knows it leaves its const argument's start pointer alone.
template <class T> CArray<T>& CArray<T>::operator=(const CArray<T>& other)
{
    if (&other != this)
    {
        unsigned long n = other.DataEnd - other.Data;
        if (n > (unsigned long)(StorageEnd - Data))
        {
            T* fresh = n ? (T*)malloc(n * sizeof(T)) : 0;
            for (unsigned long i = 0; i < n; ++i) fresh[i] = other.Data[i];
            if (Data) free(Data);
            Data = fresh;
            StorageEnd = fresh + n;
        }
        else
            for (unsigned long i = 0; i < n; ++i) Data[i] = other.Data[i];
        DataEnd = Data + n;
    }
    return *this;
}
#pragma auto_inline(on)

struct CWaveFile {
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
