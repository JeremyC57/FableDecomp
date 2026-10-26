// C3DAnimationManager::AddLipSyncAnimation(C3DAnimationBlendState& blend, const CLipSyncFrameData& frame, const CCharString& set) -- add one combination blend per lip-sync phoneme of the frame
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A9F0D0, 243 bytes (`ret 0Ch`).
// Ego_r.pdb: ?AddLipSyncAnimation@C3DAnimationManager@@QAEXAAVC3DAnimationBlendState@@ABVCLipSyncFrameData@@ABVCCharString@@@Z.
// Callees: CCharString copy ctor 0x0099EC30 / ~CCharString 0x0099EAE0, the phoneme animation lookup 0x00A26530
// (thiscall on the manager's +4 bank, (set, phoneme) -> animation index; manifest "animation_entry_search", its only
// caller is this function), C3DAnimationBlendState::AddCombinationBlend(C3DAnimationStateAlpha*) 0x00A3C310;
// vtables C3DAnimationStateAlpha 0x01265128 / CIVCountedPointeeBase 0x012354D0; 0.5 double pool 0x012316F0.
// Shape: `for (i < size())` over the 12-byte CLipSyncPhoneme array (/12 by `imul 2AAAAAABh`), each phoneme copied
// into a local (implicit copy ctor: Symbol by call, ID and Weight by dword), the symbol copied again into a
// named local in its own block (it lands in the dead `frame` argument slot; `lea ecx` re-addresses it -- an
// unnamed temporary pushes the ctor's eax) for the lookup; a found animation is added as a stack
// C3DAnimationStateAlpha(index, 0.0f, Weight * 0.5) whose implicit dtor leaves only the base's vtable store.
// Layout: Ego_r.pdb CLipSyncPhoneme {Symbol, ID, Weight}; C3DAnimationStateAlpha (0x14): CIVCountedPointeeBase
// {vptr, IVRefCount +4}, AnimationIndex +8, AnimationTime +0xc, AlphaWeight +0x10; retail CArray is 12 bytes.
// Leaf: AddLipSyncAnimation@C3DAnimationManager
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
};

class CLipSyncPhoneme { public:
    CCharString Symbol;                            // +0
    unsigned long ID;                              // +4
    float Weight;                                  // +8
};

template <class T> class CArray { public:                 // retail: begin, end, end of storage
    T* m_pBegin;
    T* m_pEnd;
    T* m_pCapacity;
    unsigned long size() const { return m_pEnd - m_pBegin; }
    const T& operator[](unsigned long i) const { return m_pBegin[i]; }
};

class CLipSyncFrameData { public:
    CArray<CLipSyncPhoneme> Phonemes;              // +0
};

class CIVCountedPointeeBase { public:
    unsigned long IVRefCount;                      // +0x04
    CIVCountedPointeeBase() : IVRefCount(1) {}
    virtual ~CIVCountedPointeeBase() {}
};
class C3DAnimationStateBase : public CIVCountedPointeeBase { public:
    virtual long GetType() const;
};
class C3DAnimationState : public C3DAnimationStateBase { public:
    long AnimationIndex;                           // +0x08
    float AnimationTime;                           // +0x0c
    C3DAnimationState(long index, float time) : AnimationIndex(index), AnimationTime(time) {}
};
class C3DAnimationStateAlpha : public C3DAnimationState { public:
    float AlphaWeight;                             // +0x10
    C3DAnimationStateAlpha(long index, float time, float alpha) : C3DAnimationState(index, time), AlphaWeight(alpha) {}
    virtual long GetType() const;
};

class C3DAnimationBlendState { public:
    void AddCombinationBlend(C3DAnimationStateAlpha* state);   // 0x00A3C310
};

struct CAnimationBank {
    long GetLipSyncAnimationIndex(const CCharString& set, const CCharString& phoneme);   // 0x00A26530
};

class C3DAnimationManager { public:
    virtual ~C3DAnimationManager();
    CAnimationBank* Bank;                          // +0x04
    void AddLipSyncAnimation(C3DAnimationBlendState& blend, const CLipSyncFrameData& frame, const CCharString& set);
};

void C3DAnimationManager::AddLipSyncAnimation(C3DAnimationBlendState& blend, const CLipSyncFrameData& frame, const CCharString& set)
{
    for (unsigned long i = 0; i < frame.Phonemes.size(); ++i) {
        CLipSyncPhoneme phoneme = frame.Phonemes[i];
        long index;
        {
            CCharString symbol(phoneme.Symbol);
            index = Bank->GetLipSyncAnimationIndex(set, symbol);
        }
        if (index) {
            C3DAnimationStateAlpha state(index, 0.0f, phoneme.Weight * 0.5);
            blend.AddCombinationBlend(&state);
        }
    }
}