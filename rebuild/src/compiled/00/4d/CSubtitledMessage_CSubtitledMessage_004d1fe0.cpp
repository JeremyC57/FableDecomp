// CSubtitledMessage::CSubtitledMessage(const CSubtitledMessage& other) -- memberwise copy: bank, index, 3D flag, text, narrator and the three tag vector maps
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D1FE0, 119 bytes. Callees: CWideString(const CWideString&) @ 0x0099B720,
//          CCharString(const CCharString&) @ 0x0099EC30, the three STLport tag-vector copy constructors
//          @ 0x004D07A0 (attitude), 0x004D0830 (animation), 0x004D08A0 (camera).
// Shape: the compiler-generated memberwise copy; each CVectorMap copies its vector, nothing for the empty comparator, then the Dirty byte (+0x0d).
// Model: retail CSubtitledMessage is 0x44 bytes (see the landed default ctor 0x004CDB70).
// Leaf: ??0CSubtitledMessage
class CWideString {
public:
    unsigned short* m_pData;
    CWideString(const CWideString& other);         // 0x0099B720
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};
class CCharString {
public:
    void* m_pStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);         // 0x0099EC30
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
enum EConversationAttitude { ATTITUDE_NONE };
struct CCameraTagRecord;                           // CSubtitledMessage::CCameraTag (opaque here)
template <class T> struct CTagVector {             // STLport vector<pair<float, T> >: first / last / end of storage
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    CTagVector(const CTagVector& other);           // out of line per element type
    CTagVector& operator=(const CTagVector& other);
};
template <class T> class CKeyPairCompareLess { public: };  // empty comparator (+0x0c)
template <class T> class CVectorMap { public:             // CVectorMap<float, T, CKeyPairCompareLess<float, T> >, 16 bytes
    CTagVector<T>          Vector;                 // +0x00
    CKeyPairCompareLess<T> Compare;                // +0x0c
    bool                   Dirty;                  // +0x0d
};
class CASoundBank;
class CSubtitledMessage {
public:
    CASoundBank*                       PSpeechBank;    // +0x00
    long                               SpeechIndex;    // +0x04
    bool                               Speech3D;       // +0x08
    CWideString                        Text;           // +0x0c
    CCharString                        Narrator;       // +0x10
    CVectorMap<EConversationAttitude>  AttitudeTags;   // +0x14
    CVectorMap<CCharString>            AnimationTags;  // +0x24
    CVectorMap<CCameraTagRecord>       CameraTags;     // +0x34
    CSubtitledMessage(const CSubtitledMessage& other);
};

CSubtitledMessage::CSubtitledMessage(const CSubtitledMessage& other)
    : PSpeechBank(other.PSpeechBank), SpeechIndex(other.SpeechIndex), Speech3D(other.Speech3D),
      Text(other.Text), Narrator(other.Narrator), AttitudeTags(other.AttitudeTags),
      AnimationTags(other.AnimationTags), CameraTags(other.CameraTags)
{
}