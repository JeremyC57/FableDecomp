// CDSoundStreamBuffer::CreateLoopingBuffer(unsigned long rate) -- create the stream's DirectSound buffer (helper 0x00C57140 with 1, rate, 1), then take its IDirectSound3DBuffer, switch 3D processing off (SetMode DS3DMODE_DISABLE, immediate), mute its EAX 2.0 direct path (lDirect -10000 through IKsPropertySet::Set) and start it looping; false at the first failure, else whether Play returned DS_OK
// Grade: DIFFER(206v206) -- block placement only: VC7.1 puts the merged `xor al,al ; pop esi ; add esp,4Ch ; ret 4` false block after the LAST failure test (Set), retail after the FIRST (the CreateBuffer test, `jne` over it, every later `jl` jumping back); six early returns, an && body with one final return and an || chain all give the end placement. `hr == 0 ? true : false` fixed the `sbb al,al ; inc al` tail (`!Play()` gives `sbb eax,eax ; inc eax`).
// Leaf: ?CreateLoopingBuffer@CDSoundStreamBuffer@@UAE_NK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C56D70, 206 bytes, thiscall `ret 4`.
// Identity (descriptive method name; GameCODA has no PDB or RTTI): slot 6 of the stream-buffer vtable 0x012B8D90 (its only
//   reference, 0x012B8DA8), the class of the landed IsChunkReady / LockChunk / UnlockChunk 0x00C57070..0x00C57110 and
//   ReleaseBuffer 0x00C57040 (slot 7: Stop + Release of the same +0x04 buffer). GUIDs from the exe:
//   0x012AC5E4 IID_IDirectSound3DBuffer {279AFA86-4981-11CE-A521-0020AF0BE560}, 0x012A9714 IID_IKsPropertySet
//   {31EFAC30-515C-11D0-A9AA-00AA0061BE93}, 0x012BEEA0 DSPROPSETID_EAX20_BufferProperties {A8FA6881-B476-11D3-BDB9-00C0F02DDF87}
//   (bound as g_eaxSourcePropertySetGuid_012beea0). Catalog gapscan name is a placeholder.
// Shape (literal retail): the 3D-buffer pointer is homed in the dead `rate` slot; IKsPropertySet is queried twice and
//   neither interface is released; the 0x48-byte EAXBUFFERPROPERTIES is zeroed with an inline memset (`rep stosd`) after
#include <string.h>

struct GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
};
extern const GUID IID_IDirectSound3DBuffer;                      // 0x012AC5E4
extern const GUID IID_IKsPropertySet;                            // 0x012A9714
extern const unsigned char g_eaxSourcePropertySetGuid_012beea0[16];   // 0x012BEEA0 DSPROPSETID_EAX20_BufferProperties

struct EAXBUFFERPROPERTIES {                       // EAX 2.0 buffer properties (0x48 bytes)
    long          lDirect;
    long          lDirectHF;
    long          lRoom;
    long          lRoomHF;
    float         flRoomRolloffFactor;
    long          lObstruction;
    float         flObstructionLFRatio;
    long          lOcclusion;
    float         flOcclusionLFRatio;
    float         flOcclusionRoomRatio;
    long          lOutsideVolumeHF;
    float         flAirAbsorptionFactor;
    unsigned long dwFlags;
    long          _reserved[5];
};

struct IKsPropertySet {                            // COM: __stdcall, `this` pushed
    virtual long __stdcall QueryInterface(const GUID& iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1
    virtual unsigned long __stdcall Release();                               // slot 2
    virtual long __stdcall Get(const GUID& set, unsigned long id, void* instance, unsigned long instanceBytes,
                               void* data, unsigned long dataBytes, unsigned long* returned);   // slot 3
    virtual long __stdcall Set(const GUID& set, unsigned long id, void* instance, unsigned long instanceBytes,
                               void* data, unsigned long dataBytes);                            // slot 4 (+0x10)
};

struct IDirectSound3DBuffer {                      // COM: __stdcall, `this` pushed
    virtual long __stdcall QueryInterface(const GUID& iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1
    virtual unsigned long __stdcall Release();                               // slot 2
    virtual void __stdcall v03(); virtual void __stdcall v04(); virtual void __stdcall v05();
    virtual void __stdcall v06(); virtual void __stdcall v07(); virtual void __stdcall v08();
    virtual void __stdcall v09(); virtual void __stdcall v10(); virtual void __stdcall v11();
    virtual void __stdcall v12(); virtual void __stdcall v13(); virtual void __stdcall v14();
    virtual void __stdcall v15(); virtual void __stdcall v16(); virtual void __stdcall v17();
    virtual long __stdcall SetMode(unsigned long mode, unsigned long apply);  // slot 18 (+0x48)
};

struct IDirectSoundBuffer {                        // COM: __stdcall, `this` pushed
    virtual long __stdcall QueryInterface(const GUID& iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1
    virtual unsigned long __stdcall Release();                               // slot 2
    virtual void __stdcall v03(); virtual void __stdcall v04(); virtual void __stdcall v05();
    virtual void __stdcall v06(); virtual void __stdcall v07(); virtual void __stdcall v08();
    virtual void __stdcall v09(); virtual void __stdcall v10(); virtual void __stdcall v11();
    virtual long __stdcall Play(unsigned long reserved, unsigned long priority, unsigned long flags);   // slot 12 (+0x30)
};

class CDSoundStreamBuffer {                        // vtable 0x012B8D90
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05();
    virtual bool CreateLoopingBuffer(unsigned long rate);   // slot 6
    IDirectSoundBuffer* m_pBuffer;                 // +0x04
    void*               m_pDevice;                 // +0x08
    bool CreateBuffer_00c57140(long channels, unsigned long rate, long frames);   // 0x00C57140
};

bool CDSoundStreamBuffer::CreateLoopingBuffer(unsigned long rate)
{
    if (!CreateBuffer_00c57140(1, rate, 1))
        return false;
    IDirectSound3DBuffer* buffer3d;
    if (m_pBuffer->QueryInterface(IID_IDirectSound3DBuffer, (void**)&buffer3d) < 0)
        return false;
    IKsPropertySet* properties;
    if (buffer3d->QueryInterface(IID_IKsPropertySet, (void**)&properties) < 0)
        return false;
    if (buffer3d->SetMode(2, 0) < 0)
        return false;
    if (buffer3d->QueryInterface(IID_IKsPropertySet, (void**)&properties) < 0)
        return false;
    EAXBUFFERPROPERTIES eax;
    memset(&eax, 0, sizeof(eax));
    eax.lDirect = -10000;
    if (properties->Set(*(const GUID*)g_eaxSourcePropertySetGuid_012beea0, 1, 0, 0, &eax, sizeof(eax)) < 0)
        return false;
    long hr = m_pBuffer->Play(0, 0, 1);
    return hr == 0 ? true : false;
}
