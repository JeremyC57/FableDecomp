// CSimVoice::CSimVoice() -- CInitBaseClass base called, this class's vtable, then the Voices vector map built inline (its sorted vector's three pointers and the dirty flag cleared; the empty comparator byte untouched)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00455E39, length 32 bytes.
// Leaf: ??0CSimVoice
// Callees: CInitBaseClass::CInitBaseClass() @ 0x0099A310 (vtable 0x0129A7CC, its flag byte +4 cleared, a global
// instance counter bumped). Vtable 0x012337AC = CSimVoice (rtti-vtables.tsv); the landed CSimVoicesDef constructor
// 0x00455E72 builds this object at +0x3c, and the landed CSimVoice::HasVoice 0x00680910 has the map layout.
// Shape: the vtable store between `xor eax,eax` and the member stores -- the zeroes belong to the map member's
// inline constructor (vector +8/+0xc/+0x10, dirty flag +0x15).
class CInitBaseClass { public:                         // vtable 0x0129A7CC
    virtual ~CInitBaseClass();
    bool m_initialised;                         // +0x04
    CInitBaseClass();                           // 0x0099A310
};

enum EReactionSpeechType { REACTION_SPEECH_NONE = 0 };

struct SVoiceEntry {                            // pair<EReactionSpeechType, long>
    EReactionSpeechType first;
    long                second;
};

struct CKeyPairCompareLess { };                 // empty comparator (one byte)

struct CVoiceMap {                              // CVectorMap<EReactionSpeechType,long,CKeyPairCompareLess<..>>
    SVoiceEntry*        m_pFirst;               // +0x00
    SVoiceEntry*        m_pLast;                // +0x04
    SVoiceEntry*        m_pEnd;                 // +0x08
    CKeyPairCompareLess m_compare;              // +0x0c
    bool                m_dirty;                // +0x0d
    CVoiceMap() : m_pFirst(0), m_pLast(0), m_pEnd(0), m_dirty(false) { }
};

class CSimVoice : public CInitBaseClass { public:             // vtable 0x012337AC
    CVoiceMap Voices;                           // +0x08
    CSimVoice();
    virtual ~CSimVoice();
};

CSimVoice::CSimVoice()
{
}