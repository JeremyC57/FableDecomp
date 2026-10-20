// CTCTalk::OnCreate() -- a talker without a dialogue voice picks one at random from its creature def's dialogue voices
// Leaf: OnCreate
// Grade: DIFFER(166v165) -- retail keeps the creature def in edi, re-reads the voice count for the random range, shares the out slot with the string temporary (`push ecx` frame) and sinks `push esi` past the found test; the lane CSEs the count, swaps esi/edi and opens an 8-byte frame (a CDefStringVector member struct strength-reduces &DialogueVoices instead)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E2C00, 165 bytes (plain `ret`); CTCTalk vtable slot 4 (the catalog's "vfunc_4").
// Callees: CDefClassBase::GetSubDef_00485815(CDefPointer&) @ 0x00485815 (the 'CCreatureDef' instantiation), CTCBase::
// GetPRandomSeed @ 0x00686DA0, GFRandom(unsigned long range, unsigned long* seed) @ 0x004CDAE0 (__fastcall),
// CDefStringTable::GetString(long) @ 0x009D49B0 on g_DefStringTable @ 0x013CA828 (CCharString by hidden pointer),
// CCharString::operator=(const CCharString&) @ 0x0099EFB0, ~CCharString @ 0x0099EAE0; the def's virtual slot 1.
// Layout: CTCTalk DialogueVoice (CCharString) +0x38 (as the landed Persist 0x006E2CB0); CCharString data block length +0x04;
// CThing PDef +0x70; CCreatureDef (retail def at PDB - 4) DialogueVoices (vector<CDefString>) +0x6c.
// Shape: the inline IsEmpty (null block or zero length); the creature def is read once from its out slot into a holder (edi);
// the voice count is read twice (the test and the random range); the looked-up string is a temporary assigned and destroyed.
struct CCharStringData {
    char* PData;                                        // +0x00
    unsigned long StringLength;                         // +0x04
};
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool IsEmpty() const { return PStringData == 0 || PStringData->StringLength == 0; }
};
struct CDefStringTable {
    CCharString GetString(long id);                     // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828
unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefOut {                     // the out slot (released by the holder below)
    T* Object;
    CDefOut() : Object(0) { }
};
template <class T> struct CDefHolder {                  // the def read once from the out slot, released at scope end
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CCreatureDef : CDefBase {
    unsigned char _pad08[0x6c - 0x08];
    long* DialogueVoices_first;                         // +0x6c DialogueVoices (vector<CDefString>, 12 bytes)
    long* DialogueVoices_last;                          // +0x70
    long* DialogueVoices_end;                           // +0x74
    unsigned long GetNumDialogueVoices() const { return DialogueVoices_last - DialogueVoices_first; }
    long GetDialogueVoice(unsigned long i) const { return *(DialogueVoices_first + i); }
};
struct CDefClassBase : CDefBase {
    bool GetSubDef_00485815(CDefOut<CCreatureDef>& out);    // 0x00485815
};
class CThing {
public:
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;                                // +0x70
};
struct CTCTalk {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x38 - 0x08];
    CCharString DialogueVoice;                          // +0x38
    unsigned long* GetPRandomSeed();                    // 0x00686DA0 (CTCBase)
    __forceinline CCreatureDef* FetchCreatureDef(bool& found)
    {
        CDefOut<CCreatureDef> creatureDefOut;
        found = ParentThing->PDef->GetSubDef_00485815(creatureDefOut);
        return creatureDefOut.Object;
    }
    void OnCreate();
};

void CTCTalk::OnCreate()
{
    if (DialogueVoice.IsEmpty()) {
        bool found;
        CDefHolder<CCreatureDef> creatureDef(FetchCreatureDef(found));
        if (found && creatureDef->GetNumDialogueVoices() > 0) {
            unsigned long index = GFRandom(creatureDef->GetNumDialogueVoices(), GetPRandomSeed());
            DialogueVoice = g_DefStringTable.GetString(creatureDef->GetDialogueVoice(index));
        }
    }
}
