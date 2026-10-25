// CSubtitledMessage::GetMessageFromTextIndex(unsigned long index, const char* unused) -- build the subtitled message for a game-text index: empty when the bank has no such entry, else from the text entry, with the speech sample looked up in the entry's speech bank by its sound name and then by the entry symbol (each also tried with an "SND_" prefix)
// Grade: DIFFER(332v332) -- same instructions; retail keeps the text entry and speechIndex in esi and the speech bank in edi (ours swaps them) and pops edi later; 4 grades
// Leaf: ?GetMessageFromTextIndex@CSubtitledMessage@@
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690

class CASoundBank {
public:
    long GetSoundIndexFromSymbol(const CCharString& symbol) const;   // 0x00A01920
};

struct CIVCountedPointeeVirtual {
    virtual void Destroy(int flags);                   // slot 0
    virtual void Delete();                             // slot 1
};
struct CIVRefCounted {                                 // second base: the count at +4 behind the vptr
    unsigned long IVRefCount;
    __forceinline void DecRef() { --IVRefCount; }
    __forceinline unsigned long GetRefCount() const { return IVRefCount; }
};
struct CIVCountedPointeeBase : CIVCountedPointeeVirtual, CIVRefCounted {
    __forceinline void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Delete();
    }
};
template <class T> class CIVCountedPointer {           // 4 bytes
public:
    T* Object;
    __forceinline ~CIVCountedPointer()
    {
        if (Object)
            ((CIVCountedPointeeBase*)Object)->ReleaseTemporary();   // the count is mutable through a const pointee
    }
};

namespace NGameText {
class CTextEntry : public CIVCountedPointeeBase {      // Ego_r.pdb, 0x48 bytes
public:
    unsigned char _pad08[0x24 - 0x08];
    CCharString   SoundName;                           // +0x24
};
class CDataBankTextEntry;
class CBankFile {
public:
    CCharString GetEntrySymbol(unsigned long index) const;   // 0x009CC1D0
};
class CDataBank : public CBankFile {
public:
    const CDataBankTextEntry* GetTextInfo(unsigned long index) const;                          // 0x009C7D00
    CIVCountedPointer<const CTextEntry> GetPTextEntry(unsigned long index, bool load) const;   // 0x009C9070
};
}

class CMainGameComponent {
public:
    unsigned char         _pad00[0x14];
    NGameText::CDataBank* TextBank;                    // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;       // 0x013B86A0

class CSubtitledMessage {                              // retail 0x44 bytes (Ego_r.pdb 0x50 with 16-byte vectors)
public:
    CASoundBank*  PSpeechBank;                         // +0x00
    long          SpeechIndex;                         // +0x04
    bool          Speech3D;                            // +0x08
    unsigned char _rest[0x44 - 0x0c];                  // +0x0C Text, Narrator, the three tag maps
    CSubtitledMessage();                                       // 0x004CDB70
    CSubtitledMessage(const NGameText::CTextEntry& entry);     // 0x004CE550
    CSubtitledMessage(const CSubtitledMessage& other);         // 0x004D1FE0
    ~CSubtitledMessage();                                      // 0x004D1DB0
    static CSubtitledMessage __fastcall GetMessageFromTextIndex(unsigned long index, const char* unused);
};

__forceinline long GetSpeechIndexFromSymbol(const CASoundBank* bank, CCharString symbol)
{
    long index = bank->GetSoundIndexFromSymbol(symbol);
    if (index == 0)
        index = bank->GetSoundIndexFromSymbol("SND_" + symbol);
    return index;
}

CSubtitledMessage __fastcall CSubtitledMessage::GetMessageFromTextIndex(unsigned long index, const char* unused)
{
    NGameText::CDataBank* bank = g_pMainGameComponent->TextBank;
    if (bank->GetTextInfo(index)) {
        const NGameText::CTextEntry* entry = bank->GetPTextEntry(index, true).Object;
        CSubtitledMessage message(*entry);
        const CASoundBank* speechBank = message.PSpeechBank;
        long speechIndex = 0;
        if (speechBank)
            speechIndex = GetSpeechIndexFromSymbol(speechBank, entry->SoundName);
        if (speechIndex == 0)
            speechIndex = GetSpeechIndexFromSymbol(speechBank, bank->GetEntrySymbol(index));
        if (speechIndex > 0) {
            message.SpeechIndex = speechIndex;
            message.Speech3D = true;
        }
        return message;
    }
    return CSubtitledMessage();
}
