// Grade: DIFFER(514v515) -- register allocation: retail keeps the params pointer in ecx (bools tested in al), the sample reference in edi and the system vtable in ebx; ours swaps eax/ecx for the params/bool and ebx/edi for sample/vtable. Structure (inline first-bit loop for mask A, out-of-line FindFirstSetBit hoisted for mask B, listener/emitter position, request temporary, counted active-sound push_back) matches.
// CSoundEmitterLH::vfunc_4(const CSoundSampleRef& sample, const CCharString& name, bool ignoreActive, const SSoundPlayParams& params, bool atListener) -- play a named sample with explicit parameters: build the instance parameters (volume, the two bit-flag masks, position from the listener or the emitter, owner), start it through the sound system and on success track it as a new active sound; returns the handle or -1
// Leaf: vfunc_4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AABD50, length 506 bytes (`ret 14h`), vtable 0x0129E6EC (CSoundEmitterLH) slot 4.
// Label correction: the catalog "CEngineSubPrimitiveAnimatedMesh::Create" is an autoname; rtti-vtables.tsv puts the body in
//   CSoundEmitterLH slot 4 (next to the landed vfunc_5 0x00AAC1D0).
// Callees: SSoundInstanceParams ctor @ 0x00C0A790, its mask setters @ 0x00C0A8A0 / 0x00C0A8C0, FindFirstSetBit @ 0x00A3F860
//   (`__fastcall` mask, byte count), the request ctor/dtor @ 0x00C08720 / 0x00C08780 (vtable 0x012B1A54), GFGetTime @ 0x009E1BC0,
//   CActiveSound ctor @ 0x00AAB8B0, CCountedPointer<CActiveSound>(T*) @ 0x00AAC820 / dtor @ 0x00AAC870, list push_back @ 0x00AACCC0.
// Layout: local model (the audio layer is not in Ego_r.pdb), as the landed vfunc_5.
struct C3DVector { float X, Y, Z; };
double __fastcall GFGetTime();                     // 0x009E1BC0
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A
long __fastcall FindFirstSetBit(unsigned long mask, unsigned long bytes);   // 0x00A3F860

struct SCharStringData { const char* m_pText; };
struct CCharString {
    SCharStringData* m_pData;
    const char* c_str() const { return m_pData ? m_pData->m_pText : ""; }
};
struct SSoundInstanceParams {                      // 0x5c bytes
    long           Owner;                          // +0x00
    long           _04;
    float          _08;
    float          Volume;                         // +0x0c
    bool           UseEmitterPosition;             // +0x10
    C3DVector      Position;                       // +0x14
    long           _20;
    bool           AtListener;                     // +0x24
    unsigned char  _pad25[0x5c - 0x25];
    SSoundInstanceParams();                        // 0x00C0A790
    void SetFlagA(unsigned long index, bool set);  // 0x00C0A8A0
    void SetFlagB(unsigned long index, bool set);  // 0x00C0A8C0
};
inline long FindFirstBit(unsigned long mask)
{
    for (unsigned long i = 0; i < 0x80; ++i)
        if (mask & (1 << i))
            return i;
    return -1;
}
struct CSoundRequest {                             // vtable 0x012B1A54
    virtual void v00();
    long  m_index;
    long  m_frame;
    const char* m_pName;
    long  m_sample;
    CSoundRequest(long sample, const char* name);  // 0x00C08720
    ~CSoundRequest();                              // 0x00C08780
};
struct CSoundSystem {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long Play(const CSoundRequest& request, const SSoundInstanceParams& params);   // slot 16 (+0x40)
};
struct CSoundDevice {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual CSoundSystem* GetSystem();             // slot 6 (+0x18)
};
struct CSoundBankLH {
    unsigned char _pad00[0x24];
    bool          Enabled;                         // +0x24
    unsigned char _pad25[0x40 - 0x25];
    CSoundDevice* m_pDevice;                       // +0x40
    unsigned char _pad44[0x74 - 0x44];
    C3DVector     ListenerPosition;                // +0x74
};
struct CSoundSampleRef {
    unsigned char _pad00[0x10];
    long          Sample;                          // +0x10
};
struct SSoundPlayParams {
    long          _00;
    float         Volume;                          // +0x04
    unsigned char _pad08[0x14 - 0x08];
    unsigned long MaskA;                           // +0x14
    unsigned long MaskB;                           // +0x18
    long          Owner;                           // +0x1c
};
struct SCountBlock;
template<class T> struct CCountedPointer {
    T*           m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer(T* object);                    // 0x00AAC820
    ~CCountedPointer();                            // 0x00AAC870
};
struct CSoundEmitterLH {
    struct CActiveSound {                          // 0x24 bytes
        unsigned char _body[0x24];
        CActiveSound(long handle, const CSoundSampleRef& sample, const CCharString& name, float time, bool useEmitter, bool atListener);   // 0x00AAB8B0
    };
    struct CActiveSoundList {
        void* m_pHead;
        void push_back(const CCountedPointer<CActiveSound>& sound);   // 0x00AACCC0
    };
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual long vfunc_4(const CSoundSampleRef& sample, const CCharString& name, bool ignoreActive, const SSoundPlayParams& params, bool atListener);
    CSoundBankLH*    m_pBank;                      // +0x04
    C3DVector        m_position;                   // +0x08
    CActiveSoundList m_sounds;                     // +0x14
    bool             m_active;                     // +0x18
    bool             m_started;                    // +0x19
    bool             m_finished;                   // +0x1a
    long             m_scriptCounter;              // +0x1c
};

long CSoundEmitterLH::vfunc_4(const CSoundSampleRef& sample, const CCharString& name, bool ignoreActive, const SSoundPlayParams& params, bool atListener)
{
    if (!m_pBank->m_pDevice || !m_pBank->Enabled)
        return -1;
    SSoundInstanceParams instance;
    instance.Volume = params.Volume;
    if (ignoreActive)
        instance.UseEmitterPosition = false;
    else
        instance.UseEmitterPosition = m_active;
    instance.AtListener = atListener;
    unsigned long bits = params.MaskA;
    while (bits) {
        long bit = FindFirstBit(bits);
        instance.SetFlagA(bit, true);
        bits ^= 1 << bit;
    }
    unsigned long bitsB = params.MaskB;
    if (bitsB) {
        long bit = FindFirstSetBit(bits, 0x10);
        do {
            instance.SetFlagB(bit, true);
            bitsB ^= 1 << bit;
        } while (bitsB);
    }
    if (instance.AtListener)
        instance.Position = m_pBank->ListenerPosition;
    else if (instance.UseEmitterPosition)
    {
        instance.Position.X = m_position.X;
        instance.Position.Y = m_position.Y;
        instance.Position.Z = m_position.Z;
    }
    long owner = params.Owner;
    if (owner == -1)
        owner = m_scriptCounter;
    instance.Owner = owner;
    long handle = m_pBank->m_pDevice->GetSystem()->Play(CSoundRequest(sample.Sample, name.c_str()), instance);
    if (handle >= 0)
        m_sounds.push_back(CCountedPointer<CActiveSound>(new CActiveSound(handle, sample, name, (float)GFGetTime(), instance.UseEmitterPosition, instance.AtListener)));
    m_started = true;
    return handle;
}
