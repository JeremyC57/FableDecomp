// CSubtitledMessage::CSubtitledMessage(const CWideString& text) -- a message showing the given text: no speech bank or index yet, 3D speech on, the narrator "UNKNOWN", three empty tag vector maps; then the speech bank is fetched from the global sound-bank registry (bank 1)
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CDBD0, 101 bytes. Callees: CWideString(const CWideString&) @ 0x0099B720,
//          CCharString(const char*, long = -1) @ 0x0099EBF0 ("UNKNOWN" @ 0x01239E98), the registry lookup @ 0x00991840 on the global @ 0x013B8394.
// Shape: the landed default constructor 0x004CDB70 with the text copied instead of default-built: one zero register
//        for every zero store, each CVectorMap's inline constructor zeroes its vector and Dirty byte, the bank pointer a body assignment.
// Leaf: ??0CSubtitledMessage
class CWideString {
public:
    unsigned short* m_pData;
    CWideString(const CWideString& other);         // 0x0099B720
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
    CSubtitledMessage(const CWideString& text);
};

CSubtitledMessage::CSubtitledMessage(const CWideString& text)
    : PSpeechBank(0), SpeechIndex(0), Speech3D(true), Text(text), Narrator("UNKNOWN")
{
    PSpeechBank = g_pSoundBankRegistry->GetBank(1);
}
