// CTCTalk::IsSpeechSamplePlaying() const -- whether the current speech sample is playing on the parent's attached sound emitter: false without a sample, an attached emitter or its low-level sound source, else the source's query for (speech bank, sample)
// Leaf: ?IsSpeechSamplePlaying@CTCTalk@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E2D30, 137 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity: Ego_r.pdb name set `?IsSpeechSamplePlaying@CTCTalk@@QBE_NXZ` (it sits between the landed CTCTalk rows
//   0x006E2CF0 SetAsUsingNarrator and 0x006E2DC0 IsSpeechSampleLoadedAndPlaying); callers 0x006E5AFB, 0x006E5DC1,
//   0x00716F5F. The catalog's "CAnimateThing_GetSpeechSourceHeight" is descriptive.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const @ 0x004365B0 (the inlined GetInterface of interface 16 =
//   CTCSoundPlayer), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x2), the default speech bank getter @ 0x00686D70
//   (the inlined GetSpeechBank), the sound source's virtual slot 23 (+0x5C, bool(const CASoundBank&, long)).
// Layout: CTCTalk (Ego_r.pdb) ParentThing +0x04, SpeechSample +0x0C, PSpeechBank +0x24; retail CThing interface bitset
//   +0x20 and map +0x44; CTCSoundPlayer PAttachedEmitter +0x0C (CIntelligentPointer<CThingSoundEmitter>, Ego_r); the
//   CThingSoundEmitter's low-level sound source pointer at +0x98 (right after the 0x98-byte retail CThing).
// Shape: the unchecked out-parameter GetInterface (the absent arm reads the never-written out pointer from the lookup
//   key's frame word, `mov eax,[esp+8]`); nested ifs with one shared `xor al,al` exit; GetPItem() called twice; the
//   sample re-read from the member for the push.
class CTCBase;
class CASoundBank;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CSoundSource {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual bool IsSamplePlaying(const CASoundBank& bank, long sample);   // slot 23 (+0x5C)
};

class CThingSoundEmitter {
public:
    unsigned char _thing[0x98];                             // retail CThing
    CSoundSource* PSoundSource;                             // +0x98
};

class CBaseIntelligentPointer {
public:
    virtual void v00();
    void* PItem;                                            // +0x04
    CThingSoundEmitter* GetPItem() const;                   // 0x00A01B50
};

class CTCSoundPlayer {
public:
    void* m_pVtable;                                        // +0x00 CTCBase
    CThing* ParentThing;                                    // +0x04
    unsigned char _base08[4];
    CBaseIntelligentPointer PAttachedEmitter;               // +0x0C CIntelligentPointer<CThingSoundEmitter>
};

class CTCTalk {
public:
    void* m_pVtable;                                        // +0x00 CTCBase
    const CThing* ParentThing;                              // +0x04
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
    long SpeechSample;                                      // +0x0C
    long SampleStartedFrame;                                // +0x10
    long SampleDurationFrames;                              // +0x14
    long Attitude;                                          // +0x18
    float MimeStartTime;                                    // +0x1C
    float SampleLoadedTime;                                 // +0x20
    CASoundBank* PSpeechBank;                               // +0x24

    CASoundBank* GetDefaultSpeechBank() const;              // 0x00686D70
    bool IsSpeechSamplePlaying() const;
private:
    const CASoundBank& GetSpeechBank() const { return PSpeechBank ? *PSpeechBank : *GetDefaultSpeechBank(); }
};

bool CTCTalk::IsSpeechSamplePlaying() const
{
    CTCSoundPlayer* player;
    ParentThing->GetInterface(&player, 16);
    if (SpeechSample > 0 && player->PAttachedEmitter.GetPItem()) {
        CSoundSource* source = player->PAttachedEmitter.GetPItem()->PSoundSource;
        if (source)
            return source->IsSamplePlaying(GetSpeechBank(), SpeechSample);
    }
    return false;
}