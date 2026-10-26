// CSoundEmitterLH::vfunc_5(long soundId) -- play a sound of the emitter's bank at the emitter position: look the sample up, start it (script counter, volume 1, the position), and on success track it as a new active sound; returns the play handle or -1
// Leaf: vfunc_5
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAC1D0, length 264 bytes (`ret 4`), vtable 0x0129E6EC (CSoundEmitterLH) slot 5.
// The audio layer is not in Ego_r.pdb: method and member names are descriptive.
// Callees: the bank lookup @ 0x00A3ED30 (a CVectorMap find returning the sample or 0; manifest "VectorMap_LookupByKey"),
// this->GetPosition (slot 17, +0x44, C3DVector by hidden pointer, called once per component), the sample's slot 2 (+0x08, Play),
// operator new @ 0x00BFEA1A (0x24 bytes), GFGetTime @ 0x009E1BC0 (`__fastcall`, double), CSoundEmitterLH::CActiveSound::
// CActiveSound(long, CSoundEmitterSource*, float) @ 0x00AAB910 (landed), CCountedPointer<CActiveSound>(CActiveSound*)
// @ 0x00AAC820 (out of line), the sound list's push_back(const CCountedPointer&) @ 0x00AACCC0, operator delete @ 0x00BFE9BC.
// Layout as the landed CSoundEmitterLH ctor 0x00AAC580 / vfunc_8 0x00AAC340: +0x04 the owning bank/manager, +0x14 the VC6 list
// of counted active sounds, +0x19 a flag byte set once a sound has been started, +0x1c the script counter.
// Shape: the position is a zero-initialised C3DVector local filled one component per GetPosition() call (the zero stores are
// immediates); the counted pointer is an unnamed temporary handed to push_back (it takes the first GetPosition temporary's
// frame slot) and released inline (block re-read from the frame); the success path is nested so both failures share the final
// `return -1` tail.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

double __fastcall GFGetTime();                     // 0x009E1BC0
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A

struct CSoundEmitterSource {                       // the bank sample
    virtual void v00(); virtual void v01();
    virtual long Play(long owner, float volume, const C3DVector* position, long a, long b);   // slot 2 (+0x08)
};
struct CSoundBankLH {
    CSoundEmitterSource* FindSample(long soundId); // 0x00A3ED30
};

struct SCountBlock {
    long m_references;                             // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
template<class T> class CCountedPointer { public:
    T*           m_pObject;                        // +0
    SCountBlock* m_pCount;                         // +4
    CCountedPointer(T* object);                    // 0x00AAC820 (T = CSoundEmitterLH::CActiveSound)
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                operator delete(m_pCount);
            }
        }
    }
};

struct CSoundEmitterLH {
    struct CActiveSound {                          // 0x24 bytes (vtable 0x0129E67C)
        unsigned char _body[0x24];
        CActiveSound(long handle, CSoundEmitterSource* source, float volume);   // 0x00AAB910
    };
    struct CActiveSoundList {                      // VC6 list<CCountedPointer<CActiveSound> >: the sentinel pointer
        void* m_pHead;
        void push_back(const CCountedPointer<CActiveSound>& sound);            // 0x00AACCC0
    };
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual long vfunc_5(long soundId);
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual C3DVector GetPosition();              // slot 17 (+0x44)
    CSoundBankLH*    m_pBank;                      // +0x04
    float            m_position[3];                // +0x08
    CActiveSoundList m_sounds;                     // +0x14
    bool             m_active;                     // +0x18
    bool             m_started;                    // +0x19
    bool             m_finished;                   // +0x1a
    long             m_scriptCounter;              // +0x1c
};

long CSoundEmitterLH::vfunc_5(long soundId)
{
    CSoundEmitterSource* sample = m_pBank->FindSample(soundId);
    if (sample) {
        C3DVector position(0.0f, 0.0f, 0.0f);
        position.X = GetPosition().X;
        position.Y = GetPosition().Y;
        position.Z = GetPosition().Z;
        long handle = sample->Play(m_scriptCounter, 1.0f, &position, 0, 0);
        if (handle >= 0) {
            m_sounds.push_back(CCountedPointer<CActiveSound>(new CActiveSound(handle, sample, (float)GFGetTime())));
            m_started = true;
            return handle;
        }
    }
    return -1;
}