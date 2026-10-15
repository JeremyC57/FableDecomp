// CSubtitledMessage::CSubtitledMessage() -- default constructor: no speech bank or index yet, 3D speech on, an empty wide text, the narrator "UNKNOWN", three empty tag vector maps (attitude, animation, camera); then the speech bank is fetched from the global sound-bank registry (bank 1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CDB70, length 94 bytes.
// Leaf: ??0CSubtitledMessage
// Callees: CWideString::CWideString() @ 0x0099AED0 (Text), CCharString::CCharString(const char*, long = -1)
// @ 0x0099EBF0 (Narrator, "UNKNOWN" @ 0x01239E98), the registry lookup @ 0x00991840 (`__fastcall` on the
// global @ 0x013B8394, id 1; manifest "MessageMap::FindMessageByID": a vector-map find at +0x30 returning the
// found entry's +4 word, else 0).
// Shape: one zero register (ebx) for every zero store; the init-list scalars (+0, +4, byte +8 = 1) stored
// around the first member constructor's `lea`; each CVectorMap's inline constructor zeroes its three vector
// words and its Dirty byte (+0x0d; the empty Compare byte at +0x0c is left alone); the bank pointer is a body
// assignment after every member, `mov eax,esi`, `ret`.
// Model: PDB CSubtitledMessage (0x50 bytes; retail 0x44: each CVectorMap is a 12-byte vector + Compare + Dirty).
class CWideString {
public:
    unsigned short* m_pData;
    CWideString();                                 // 0x0099AED0
};

class CCharString {
public:
    void* m_pStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
};

struct CVectorMapTags {                            // CVectorMap<float, T, CKeyPairCompareLess<float, T> >, 16 bytes
    void*         m_pFirst;                        // +0x00
    void*         m_pLast;                         // +0x04
    void*         m_pEnd;                          // +0x08
    unsigned char Compare;                         // +0x0c (empty comparator)
    bool          Dirty;                           // +0x0d
    CVectorMapTags() : m_pFirst(0), m_pLast(0), m_pEnd(0), Dirty(false) {}
};

class CASoundBank;

struct CSoundBankRegistry {
    CASoundBank* GetBank(long id);                 // 0x00991840
};
extern CSoundBankRegistry* g_pSoundBankRegistry;   // 0x013B8394

class CSubtitledMessage {
public:
    CASoundBank*   PSpeechBank;                    // +0x00
    long           SpeechIndex;                    // +0x04
    bool           Speech3D;                       // +0x08
    CWideString    Text;                           // +0x0c
    CCharString    Narrator;                       // +0x10
    CVectorMapTags AttitudeTags;                   // +0x14
    CVectorMapTags AnimationTags;                  // +0x24
    CVectorMapTags CameraTags;                     // +0x34
    CSubtitledMessage();
};

CSubtitledMessage::CSubtitledMessage()
    : PSpeechBank(0), SpeechIndex(0), Speech3D(true), Narrator("UNKNOWN")
{
    PSpeechBank = g_pSoundBankRegistry->GetBank(1);
}