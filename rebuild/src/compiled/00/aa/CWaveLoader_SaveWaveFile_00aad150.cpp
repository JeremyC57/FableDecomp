// CWaveLoader::SaveWaveFile(CWaveFile* wave, CDataOutputStream* output) -- write a 16-bit PCM RIFF/WAVE file: build the "fmt " body and the "WAVE" form in memory streams, then emit "RIFF", the form size and the form
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AAD150, 450 bytes, `ret 8` (thiscall, `this` unused; manifest signature
//   CWaveLoader::SaveWaveFile(CWaveFile*, CDataOutputStream*)). No EH frame: the module is built without /EHsc.
// Callees: CMemoryDataOutputStream ctor 0x00996AF0 / dtor 0x00996B30 / GetSize 0x00994B40 / PeekData 0x00995E70 /
//   Write(const void*, long) 0x00996CC0 (direct: the local's type is known), CDataOutputStream::WriteUShort 0x00993E80,
//   WriteULong 0x00993EA0, WriteString(const CCharString&) 0x009959F0, CCharString(const char*, long) 0x0099EBF0,
//   ~CCharString 0x0099EAE0; the output stream's virtual Write is slot 4 (+0x10).
// Literals: "WAVE" 0x0129E798, "fmt " 0x0129E790, "data" 0x0129E788, "RIFF" 0x0129E780.
// Shape: each tag is an implicit const char* -> CCharString temporary (homed in the dead `wave` argument slot once
//   `wave` sits in esi, later in the dead slot again while `output` is loaded); the channel count is read as a 16-bit
//   value (`movsx`/`mov ax`), the sample data pointer is CArray::GetData (`size ? first : (short*)this`).
// Layout: Ego_r.pdb CWaveFile {Channels +0, SampleLength +4, SamplingFrequency +8, AudioData CArray<short> +0xC}
//   (retail 12-byte STLport vector). CMemoryDataOutputStream is 0x18 bytes in retail (landed ctor 0x00996AF0;
//   engine/CMemoryDataOutputStream.h carries the PDB 0x1C view), so a local model is used.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CDataOutputStream {
    CDataOutputStream();                                // CDataOutputStream base ctor (inline in retail)
    virtual ~CDataOutputStream();
    virtual void SetPosition(unsigned long position);
    virtual unsigned long GetPosition() const;
    virtual void Close();
    virtual void Write(const void* data, long length);  // slot 4 (+0x10)
    CCharString NewLine;                                // +0x04
    void WriteUShort(unsigned short value);             // 0x00993E80
    void WriteULong(unsigned long value);               // 0x00993EA0
    void WriteString(const CCharString& text);          // 0x009959F0
};

struct CMemoryDataOutputStream : CDataOutputStream {    // 0x18 bytes in retail
    char* First; char* Last; char* End;                 // +0x08 CArray<char>
    unsigned long Position;                             // +0x14
    CMemoryDataOutputStream();                          // 0x00996AF0
    ~CMemoryDataOutputStream();                         // 0x00996B30
    virtual void Write(const void* data, long length);  // 0x00996CC0
    long GetSize() const;                               // 0x00994B40
    const void* PeekData() const;                       // 0x00995E70
};

template<class T> struct CArray {                       // retail STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    const T* GetData() const { return size() > 0 ? _M_start : (const T*)this; }
};

struct CWaveFile {
    unsigned long Channels;                             // +0x00
    unsigned long SampleLength;                         // +0x04
    unsigned long SamplingFrequency;                    // +0x08
    CArray<short> AudioData;                            // +0x0c
};

struct CWaveLoader {
    void SaveWaveFile(CWaveFile* wave, CDataOutputStream* output);
};

void CWaveLoader::SaveWaveFile(CWaveFile* wave, CDataOutputStream* output)
{
    CMemoryDataOutputStream format;
    format.WriteUShort(1);                              // WAVE_FORMAT_PCM
    format.WriteUShort((unsigned short)wave->Channels);
    format.WriteULong(wave->SamplingFrequency);
    format.WriteULong((short)wave->Channels * wave->SamplingFrequency * 2);
    format.WriteUShort((unsigned short)((short)wave->Channels * 2));
    format.WriteUShort(16);

    CMemoryDataOutputStream form;
    form.WriteString("WAVE");
    form.WriteString("fmt ");
    form.WriteULong(format.GetSize());
    form.Write(format.PeekData(), format.GetSize());
    form.WriteString("data");
    form.WriteULong(wave->AudioData.size() * sizeof(short));
    form.Write(wave->AudioData.GetData(), wave->AudioData.size() * sizeof(short));

    output->WriteString("RIFF");
    output->WriteULong(form.GetSize());
    output->Write(form.PeekData(), form.GetSize());
}