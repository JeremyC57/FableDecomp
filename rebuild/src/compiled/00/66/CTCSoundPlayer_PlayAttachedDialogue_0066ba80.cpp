// CTCSoundPlayer::PlayAttachedDialogue(unsigned long textID, bool flag1, bool flag2, bool altFader, bool flag4) -- play a dialogue line attached to the owner: resolve the text entry (a group entry directly, else the entry for the owner's talk voice), find its sound bank and the "SND_"-prefixed sample, then play it with the dialogue fader (0x20 when altFader, else 0x8040); 0 when anything is missing
// Leaf: ?PlayAttachedDialogue@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066BA80, 368 bytes (350 without padding), thiscall `ret 14h`, unsigned long in eax.
//   Ego_r.pdb `?PlayAttachedDialogue@CTCSoundPlayer@@QAEKK_N000@Z`; called from 0x0066BF23 and
//   CCreatureAction_PerformExpression::Initialise 0x008469B3.
// Callees: CBankFile::GetEntryDataType(unsigned long) const 0x009CC210, NGameText::CDataBank::GetPGroupEntry(unsigned long, bool)
//   const 0x009C9070 (CIVCountedPointer<const CGroupEntry> by hidden pointer into the dead textID slot) and
//   GetTextEntryFromGroup(unsigned long, const CCharString&, bool&, long*, bool) const 0x009C9280 (on g_pMainGameComponent
//   0x013B86A0 +0x14); CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const 0x004365B0 (interface 0xF CTCTalk, bit 15 of
//   +0x20); CCharString(const CCharString&) 0x0099EC30 / ~CCharString 0x0099EAE0 / operator+(const char*, const CCharString&)
//   0x0099F690 ("SND_" 0x01239ED4); the sound-bank symbol map lookup 0x009917E0 (g_pSoundBankMap 0x013B8394, label
//   "CSymbolMap::FindFirstSymbolWithVal"); CASoundBank::GetSoundIndexFromSymbol 0x00A01920; CSoundInit::CSoundInit 0x0041C930;
//   CTCSoundPlayer::InternalPlayAttachedSample(CASoundBank&, long, const CSoundInit&, bool, bool, bool) 0x0066B080.
// Label: catalog "Sound_InitializeFromTextEntry" -- this is CTCSoundPlayer::PlayAttachedDialogue(unsigned long, ...) (PDB name;
//   parameter names are descriptive).
// Layout: CTCSoundPlayer ParentThing +4; CTCTalk DialogueVoice +0x38 (landed 0x008330D0); the text entry's BankName +0x1c and
//   SoundName +0x24 (landed CTCVillage::FrameUpdate 0x0065C781); CSoundInit GainFaderBits +0x14; the counted pointee keeps its
//   count at +4 behind the vptr (CIVRefCounted second base: the split `dec` release, `Delete` slot 1).
// Shape: the group entry is a NAMED CIVCountedPointer (its hidden-return slot is the dead textID argument and the raw entry is
//   read back from it before the inline ReleaseTemporary); the talk component comes from the out-parameter GetInterface (bit test
//   + LowerBound, a same-TU noinline LowerBound body keeps the key uncaptured so it shares the dead textID slot);
//   GetTextEntryFromGroup is called through an inline two-argument overload holding `bool found = false` -- that puts the
//   `found` store after the voice copy and in the textID slot too (a caller-side `found` is stored before the copy and pushes
//   the key into the voice's frame word).
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;        // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const instantiation.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CTCTalk {                                        // interface 0xF
public:
    unsigned char _pad00[0x38];
    CCharString   DialogueVoice;                       // +0x38
    CCharString GetDialogueVoice() const { return DialogueVoice; }
};

class CIVCountedPointeeVirtual {
public:
    virtual void Destroy(int flags);                   // slot 0
    virtual void Delete();                             // slot 1
};
class CIVRefCounted {
public:
    unsigned long IVRefCount;                          // +0x04
    void DecRef() { --IVRefCount; }
    unsigned long GetRefCount() const { return IVRefCount; }
};
class CIVCountedPointeeBase : public CIVCountedPointeeVirtual, public CIVRefCounted {
public:
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Delete();
    }
};
template <class T> class CIVCountedPointer {
public:
    T* Object;
    ~CIVCountedPointer()
    {
        if (Object)
            Object->ReleaseTemporary();
    }
};
namespace NGameText {
class CTextEntry : public CIVCountedPointeeBase {
public:
    unsigned char _pad08[0x1c - 0x08];
    CCharString BankName;                              // +0x1c
    unsigned char _pad20[0x24 - 0x20];
    CCharString SoundName;                             // +0x24
};
class CGroupEntry : public CTextEntry { };
class CDataBank {
public:
    unsigned long GetEntryDataType(unsigned long id) const;   // 0x009CC210 (CBankFile)
    CIVCountedPointer<CGroupEntry> GetPGroupEntry(unsigned long id, bool load) const;   // 0x009C9070
    const CTextEntry* GetTextEntryFromGroup(unsigned long id, const CCharString& group, bool& found, long* index, bool load) const;   // 0x009C9280
    const CTextEntry* GetTextEntryFromGroup(unsigned long id, const CCharString& group) const
    {
        bool found = false;
        return GetTextEntryFromGroup(id, group, found, 0, true);
    }
};
}
class CMainGameComponent {
public:
    unsigned char _pad00[0x14];
    NGameText::CDataBank* PTextBank;                   // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;      // 0x013B86A0

class CASoundBank {
public:
    long GetSoundIndexFromSymbol(const CCharString& symbol) const;   // 0x00A01920
};
class CSymbolMap {
public:
    CASoundBank* FindFirstSymbolWithVal(const CCharString* value) const;   // 0x009917E0
};
extern CSymbolMap* g_pSoundBankMap;                    // 0x013B8394

class CSoundInit {
public:
    float Volume, Pitch, Pan;
    long NoPlays, PlaybackType;
    unsigned long GainFaderBits;                       // +0x14
    unsigned long PitchFaderBits;
    long OverrideID;
    CSoundInit();                                      // 0x0041C930
};

class CTCSoundPlayer {
public:
    void*   __vftable;                                 // +0x00
    CThing* ParentThing;                               // +0x04
    unsigned long PlayAttachedDialogue(unsigned long textID, bool flag1, bool flag2, bool altFader, bool flag4);
private:
    unsigned long InternalPlayAttachedSample(CASoundBank& bank, long sample, const CSoundInit& init, bool flag1, bool flag2, bool flag4);   // 0x0066B080
};

unsigned long CTCSoundPlayer::PlayAttachedDialogue(unsigned long textID, bool flag1, bool flag2, bool altFader, bool flag4)
{
    if (textID > 0) {
        NGameText::CDataBank* bank = g_pMainGameComponent->PTextBank;
        const NGameText::CTextEntry* entry;
        if (bank->GetEntryDataType(textID) == 0) {
            CIVCountedPointer<NGameText::CGroupEntry> group = bank->GetPGroupEntry(textID, true);
            entry = group.Object;
        } else {
            CTCTalk* talk;
            if (!ParentThing->GetInterface(&talk, 0xf))
                return 0;
            entry = bank->GetTextEntryFromGroup(textID, talk->GetDialogueVoice());
        }
        if (entry) {
            CASoundBank* soundBank = g_pSoundBankMap->FindFirstSymbolWithVal(&entry->BankName);
            if (soundBank) {
                long sample = soundBank->GetSoundIndexFromSymbol("SND_" + entry->SoundName);
                if (sample > 0) {
                    CSoundInit init;
                    init.GainFaderBits = altFader ? 0x20 : 0x8040;
                    return InternalPlayAttachedSample(*soundBank, sample, init, flag1, flag2, flag4);
                }
            }
        }
    }
    return 0;
}