// CVtable_012b33f0::Read(void* buffer, long* pOffset, long* pSize) -- under the audio library's scoped lock, copy the sample's data into buffer; unwrap a compressed chunk and decode ADPCM to PCM in place; the data offset and final size are returned through the two pointers; -1 on any failure, else 0
// Leaf: ?Read@CVtable_012b33f0@@UAEJPAXPAJ1@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1E300, 450 bytes, thiscall `ret 0Ch`, EH-framed (the scoped lock).
// Identity (descriptive): slot 1 of the no-RTTI vtable 0x012B33F0 (its only reference, .rdata 0x012B33F4; base vtable
//   0x012B33DC holds five pure slots; ctor 0x00C1E5A0, dtor 0x00C1E260). The other landed slots of this vtable carry
//   address-named classes (0x00C1E280 slot 0, 0x00C1E4D0 / 0x00C1E570 / 0x00C1E500 slots 2..4); libpk has no PDB or RTTI,
//   so the class is named by its vtable like the landed CVtable_012b33a4 sibling. The manifest label
//   "PathFinding_CompressAndQuery" is propagated.
// Callees: PKCProtectionInstance ctor/dtor @ 0x00C0FFC0 / 0x00C0FFE0 over g_lock_0143bb14; the sample's slots 2
//   (CopyData, cf. 0x00C1E090), 4 (compressed flag, cf. 0x00C1E0D0 `mov eax,[ecx+24h]`) and 5 (header size);
//   CSoundSample::GetDataSize @ 0x00C1E9C0; ReadChunkExtent_00c347b0 @ 0x00C347B0 (landed `__fastcall`);
//   CSoundFormat::GetPCMBufferSize @ 0x00C2D920 (landed); operator new[] @ 0x00BFEB22; operator delete @ 0x00BFE9BC;
//   the ADPCM-to-PCM decoder @ 0x00C34630 (`__fastcall`, format in ecx, source in edx, destination and its size on
//   the stack, `ret 8`; descriptive name -- the other caller is the stream-buffer row 0x00C26090).
// Layout: CSoundFormat / CSoundSample as the landed GetPCMBufferSize (0x00C2D920) and CStreamBuffer::Attach (0x00C262E0)
//   models; no engine header covers the libpk audio library.
// Shape: the chunk source pointer is a named local (`mov ecx,[ebp]` scheduled ahead of the out-pointer pushes; the
//   inline argument loads edx first); `operator new[]` must be declared for VC7.1 to call ??_U instead of ??2; the
//   buffer is released with scalar `delete` (??3). The `start` out-word shares the dead pOffset argument slot.
#include <string.h>
void* __cdecl operator new[](unsigned int size);         // 0x00BFEB22
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;                // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);    // 0x00C0FFC0
    ~PKCProtectionInstance();                             // 0x00C0FFE0
};

struct CSoundFormat {                                     // 20-byte format record
    unsigned long  Size;                                  // +0x00
    unsigned long  m_reserved1;                           // +0x04
    unsigned long  m_reserved2;                           // +0x08
    unsigned short Channels;                              // +0x0C
    unsigned short Encoding;                              // +0x0E
    unsigned long  SampleRate;                            // +0x10
    unsigned long GetPCMBufferSize();                     // 0x00C2D920
};

struct CSoundSample {
    virtual void v00(); virtual void v01();
    virtual long CopyData(void* buffer);                  // slot 2 (+0x08)
    virtual void v03();
    virtual long IsCompressed();                          // slot 4 (+0x10)
    virtual long GetHeaderSize();                         // slot 5 (+0x14)
    void*          m_pData;                               // +0x04
    CSoundFormat   m_format;                              // +0x08
    long GetDataSize();                                   // 0x00C1E9C0
};

long __fastcall ReadChunkExtent_00c347b0(void* first, void* second, long* outStart, long* outEnd);   // 0x00C347B0
long __fastcall DecodeSoundData_00c34630(CSoundFormat* format, void* src, void* dst, unsigned long dstSize);   // 0x00C34630

struct CVtable_012b33f0 {
    virtual long GetSize();
    virtual long Read(void* buffer, long* pOffset, long* pSize);
    CSoundSample* m_pSample;                              // +0x04
};

long CVtable_012b33f0::Read(void* buffer, long* pOffset, long* pSize)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    if (m_pSample->CopyData(buffer) < 0)
        return -1;
    CSoundFormat format = m_pSample->m_format;
    *pOffset = m_pSample->GetHeaderSize() + m_pSample->GetDataSize();
    format.Size -= m_pSample->GetHeaderSize();
    if (m_pSample->IsCompressed()) {
        long start;
        long end;
        void* src = (char*)buffer + *pOffset;
        if (ReadChunkExtent_00c347b0(src, (void*)format.Size, &start, &end) < 0)
            return -1;
        *pOffset += start;
        format.Size = end;
    }
    if (format.Encoding != 0) {
        unsigned long pcmSize = format.GetPCMBufferSize();
        unsigned char* pcm = new unsigned char[pcmSize];
        memcpy(pcm, (char*)buffer + *pOffset, format.Size);
        long result = DecodeSoundData_00c34630(&format, pcm, buffer, pcmSize);
        delete pcm;
        *pOffset = 0;
        if (result < 0)
            return -1;
    }
    *pSize = format.Size;
    return 0;
}