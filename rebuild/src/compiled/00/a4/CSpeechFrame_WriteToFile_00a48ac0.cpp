// CSpeechFrame::WriteToFile(CDataOutputStream* stream) const -- save one lip-sync analysis frame: the 0x0FF7DA7A tag, the frequency count, each frequency's real part, then amplitude, median, base frequency and variance
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A48AC0, length 129 bytes (`ret 4`).
// Callees: CDataOutputStream::WriteULong @ 0x00993EA0, WriteFloat @ 0x00993EE0 (both under the propagated "WriteCBYTE" label).
// Layout: Ego_r.pdb CSpeechFrame (0x20) with the retail 12-byte CArray<CComplexNumber> Frequencies at +0, so the floats sit at
// +0x0c..+0x18 (PDB +0x10..+0x1c).
// Shape: the loop bound is the array size re-read every pass (unsigned `jb`), the element read as `m_first[i]`.
// Leaf: WriteToFile
struct CDataOutputStream {
    void WriteULong(unsigned long value);           // 0x00993EA0
    void WriteFloat(float value);                   // 0x00993EE0
};

struct CComplexNumber { float Real; float Imaginary; };

template <class T> struct CArray {                  // 12-byte retail array {first, last, end}
    T* m_first; T* m_last; T* m_end;
    unsigned long Size() const { return m_last - m_first; }
    const T& operator[](unsigned long i) const { return m_first[i]; }
};

struct CSpeechFrame {
    CArray<CComplexNumber> Frequencies;             // +0x00
    float Amplitude;                                // +0x0c
    float MedianFrequency;                          // +0x10
    float BaseFrequency;                            // +0x14
    float FrequencyVariance;                        // +0x18
    void WriteToFile(CDataOutputStream* stream) const;
};

void CSpeechFrame::WriteToFile(CDataOutputStream* stream) const
{
    stream->WriteULong(0x0ff7da7a);
    stream->WriteULong(Frequencies.Size());
    for (unsigned long i = 0; i < Frequencies.Size(); ++i)
        stream->WriteFloat(Frequencies[i].Real);
    stream->WriteFloat(Amplitude);
    stream->WriteFloat(MedianFrequency);
    stream->WriteFloat(BaseFrequency);
    stream->WriteFloat(FrequencyVariance);
}