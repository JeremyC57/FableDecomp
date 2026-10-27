// CTCFireheartMinigame::GetText(unsigned long textId) -- the game text `textId` from the main component's text bank, or "Missing text entry"
// Leaf: ?GetText@CTCFireheartMinigame@@QAE?AVCWideString@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E7580, 181 bytes, thiscall, `ret 8` (CWideString by hidden pointer; `this` unused).
// Identity (proven): Ego_r.pdb ?GetText@CTCFireheartMinigame@@QAE?AVCWideString@@K@Z (each minigame class has its own copy); its callers are CTCFireheartMinigame::DelayedQuit 0x007E7AC0 (three texts).
//   The manifest's "CTCBetting::GetText" is propagated across byte-identical bodies.
// Callees: CWideString(const wchar_t*) 0x0099B6B0 and atexit 0x004012BC (the function-local static at 0x013BAEA4, guard bit
//   0x013BAEA8), NGameText::CDataBank::GetPTextEntry(unsigned long, bool) const 0x009C9070 (CIVCountedPointer by hidden pointer),
//   NGameText::CTextEntry::GetText() const 0x009C7A40, CWideString(const CWideString&) 0x0099B720, ~CWideString 0x0099B510.
// Shape (family M162, representative 0x005E9E70): `static CWideString missing(L"Missing text entry"); CIVCountedPointer<const
//   CTextEntry> entry = bank->GetPTextEntry(textId, true); return entry.Object ? entry.Object->GetText() : missing;` -- the
//   conditional's temporary carries a construction flag (bl) and lives in the dead textId slot; the entry's release is the
//   landed CTCInventoryBase::GetInventoryText 0x005BD273 ReleaseTemporary. Speed TU.
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t
class CWideString {
public:
    void* PStringData;
    CWideString(const wchar_t* text);                     // 0x0099B6B0
    CWideString(const CWideString& other);                // 0x0099B720
    ~CWideString();                                       // 0x0099B510
};
struct CIVRefCounted {                                    // the count sits at +4 behind the vptr
    mutable unsigned long IVRefCount;
    void DecRef() const { --IVRefCount; }
    unsigned long GetRefCount() const { return IVRefCount; }
};
struct CIVVirtual {
    virtual void Destroy(int flags) const;                // slot 0
    virtual void Delete() const;                          // slot 1
};
class CIVCountedPointeeBase : public CIVVirtual, public CIVRefCounted {
public:
    void ReleaseTemporary() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Delete();
    }
};
template <class T> class CIVCountedPointer {             // 4 bytes
public:
    T* Object;
    __forceinline ~CIVCountedPointer()
    {
        if (Object)
            Object->ReleaseTemporary();
    }
};
namespace NGameText {
class CTextEntry : public CIVCountedPointeeBase {
public:
    CWideString GetText() const;                          // 0x009C7A40
};
class CDataBank {
public:
    CIVCountedPointer<const CTextEntry> GetPTextEntry(unsigned long index, bool load) const;   // 0x009C9070
};
}
class CMainGameComponent {
public:
    unsigned char         _pad00[0x14];
    NGameText::CDataBank* TextBank;                       // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;          // 0x013B86A0
class CTCFireheartMinigame {
public:
    CWideString GetText(unsigned long textId);
};
CWideString CTCFireheartMinigame::GetText(unsigned long textId)
{
    static CWideString missing(L"Missing text entry");    // 0x013BAEA4
    CIVCountedPointer<const NGameText::CTextEntry> entry = g_pMainGameComponent->TextBank->GetPTextEntry(textId, true);
    return entry.Object ? entry.Object->GetText() : missing;
}