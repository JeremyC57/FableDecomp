// CAudioWaveOut::Initialize(const void* device, void* window, bool multithreaded) -- vtable slot 1 of the waveOut output: available when Windows reports at least one waveOut device (waveOutGetNumDevs() != 0); the arguments are unused
// Leaf: ?Initialize@CAudioWaveOut@@UAE_NPBXPAX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5D5F0, 14 bytes (+ int3 padding), thiscall `ret 0Ch`; slot 1 of the
//   vtable 0x012B8FCC.
// Callee: waveOutGetNumDevs (WINMM IAT 0x01440498).
// Name: descriptive (no PDB/RTTI); the class is the landed CAudioWaveOut; the slot and its parameters parallel the
//   DirectSound slot 1 0x00C56F70 and the OpenAL slot 1 0x00C57460 (this chunk). The manifest label is a placeholder.
// Shape: retail's `test eax,eax ; setne al` comes from the if/return-false form; `return n != 0;` gives
//   `neg ; sbb ; neg` (as in 0x00C57420).
extern "C" __declspec(dllimport) unsigned int __stdcall waveOutGetNumDevs();   // IAT 0x01440498
// The waveOut output of the libpk audio driver (vtable 0x012B8FCC, no RTTI; ctor 0x00C5D750): the landed CAudioWaveOut
// model of WriteWaveBuffer 0x00C5D6B0 / Initialize 0x00C5D7F0 (Headers +4, WaveOut +8, BufferCount +0x10, QueuedCount
// +0x14, WriteIndex +0x18, the CRITICAL_SECTION at +0x20). Its slots parallel the DirectSound (0x012B8D98) and OpenAL
// (0x012B8ED4) stream buffers: 1 Initialize (device, window, multithreaded), 3 Initialize (channels, bytes), 5 Close,
// 6 the chunk-ready test, 7 the chunk pointer, 8 WriteWaveBuffer.
struct WAVEHDR_32 {                                     // WAVEHDR, 0x20 bytes
    char*         lpData;                               // +0x00
    unsigned char _body[0x1c];
};
struct CRITICAL_SECTION_18 { unsigned char _body[0x18]; };

struct CAudioWaveOut {
    virtual void v00();
    virtual bool Initialize(const void* device, void* window, bool multithreaded);   // slot 1: 0x00C5D5F0
    virtual void v02();
    virtual bool Initialize(unsigned long channels, unsigned long bufferBytes);      // slot 3: 0x00C5D7F0
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void* LockChunk();                                                       // slot 7: 0x00C5D6A0
    WAVEHDR_32* Headers;                                 // +4
    void* WaveOut;                                       // +8
    unsigned char _pad0[4];
    long BufferCount;                                    // +0x10
    long QueuedCount;                                    // +0x14
    long WriteIndex;                                     // +0x18
    unsigned char _pad1[4];
    CRITICAL_SECTION_18 Lock;                            // +0x20
};

bool CAudioWaveOut::Initialize(const void* device, void* window, bool multithreaded)
{
    if (waveOutGetNumDevs() == 0)
        return false;
    return true;
}