// Grade: DIFFER(43v43) -- the dirty-flag copy after the vector assignment: retail `mov al,[edi+0Dh] ; mov [ebx+0Dh],al` before `pop edi ; mov eax,esi`, the lane loads into dl and sinks the store below `pop edi ; mov eax,esi ; pop esi` (explicit, implicit-COMDAT, STLport-vector map and optimize("s") forms tried)
// CSimVoice::operator=(const CSimVoice& other) -- member-wise assignment: the CInitBaseClass flag byte, then the Voices map (its sorted vector assigned out of line, the dirty flag copied; the empty comparator copies nothing)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046313E, length 43 bytes.
// Leaf: ??4CSimVoice
// Callees: vector<pair<EReactionSpeechType,long>>::operator= @ 0x00463169 (8-byte elements).
// Called through the landed CSimVoicesDef::Copy 0x0046311F on its +0x3c member.
// Shape: the map's inline assignment keeps both map addresses in registers (`lea edi,[eax+8]` for the source, `lea
// ebx,[esi+8]` for the destination) because the dirty flag is read and written through them after the call; the
// base's flag byte goes first through cl.
struct CInitBaseClass {                         // vtable 0x0129A7CC
    virtual ~CInitBaseClass();
    bool m_initialised;                         // +0x04
};

enum EReactionSpeechType { REACTION_SPEECH_NONE = 0 };

struct SVoiceEntry {                            // pair<EReactionSpeechType, long>
    EReactionSpeechType first;
    long                second;
};

struct CVoiceVector {                           // VC6-era vector<SVoiceEntry>: first, last, end
    SVoiceEntry* m_pFirst;
    SVoiceEntry* m_pLast;
    SVoiceEntry* m_pEnd;
    CVoiceVector& operator=(const CVoiceVector& other);   // 0x00463169
};

struct CKeyPairCompareLess { };                 // empty comparator (one byte)

struct CVoiceMap {                              // CVectorMap<EReactionSpeechType,long,CKeyPairCompareLess<..>>
    CVoiceVector        m_entries;              // +0x00
    CKeyPairCompareLess m_compare;              // +0x0c
    bool                m_dirty;                // +0x0d
    CVoiceMap& operator=(const CVoiceMap& other)
    {
        m_entries = other.m_entries;
        m_dirty = other.m_dirty;
        return *this;
    }
};

struct CSimVoice : CInitBaseClass {             // vtable 0x012337AC
    CVoiceMap Voices;                           // +0x08
    CSimVoice& operator=(const CSimVoice& other);
    virtual ~CSimVoice();
};

CSimVoice& CSimVoice::operator=(const CSimVoice& other)
{
    CInitBaseClass::operator=(other);
    Voices = other.Voices;
    return *this;
}
