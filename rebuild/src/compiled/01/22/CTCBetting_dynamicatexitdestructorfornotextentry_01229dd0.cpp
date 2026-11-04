// CTCBetting::GetText'::`2'::`dynamic atexit destructor for 'no_text_entry''() -- destroy the function-local static CWideString no_text_entry at exit
// Leaf: _$E2
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x01229DD0, 10 bytes: `mov ecx, offset 0x013BAE84 ; jmp 0x0099B510` (~CWideString on the static).
// Identity (two sources): (1) the only reference to 0x01229DD0 is the `push offset 0x01229DD0 ; call atexit 0x004012BC` at 0x007E4DA1 inside
//   CTCBetting::GetText 0x007E4D70, right after the guarded construction of the static at 0x013BAE84 (guard 0x013BAE88 bit 0, L"Missing text entry");
//   (2) both PDB name sets list `CTCBetting::GetText'::`2'::`dynamic atexit destructor for 'no_text_entry'' (donor 0400FD80 / 0101CA40),
//   scope 2 = the static at function scope (?no_text_entry@?1??GetText@CTCBetting@@QAE?AVCWideString@@K@Z@4V3@A). VC7.1 emits the stub as the
//   TU-local `_$E2` in .text$yd, past the code, where retail's sits (precedent: landed 0x01229170).
//   The landed owner spells the static `missing`; both PDB name sets spell it `no_text_entry`, so this TU uses the PDB
//   name (NAME_FIXES.tsv: 0x013BAE84); the owner's bytes do not change.
// TU: the landed owner 0x007E4D70 (rebuild/src/compiled/00/7e/CTCBetting_GetText_007e4d70.cpp) verbatim apart from the static's name; only the thunk is graded.
// Callees: ??1CWideString@@QAE@XZ 0x0099B510.
// ---- owner TU (from the landed row, minus its Leaf/Flags lines) ----
// CTCBetting::GetText(unsigned long textId) -- the game text `textId` from the main component's text bank, or "Missing text entry"
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E4D70, 181 bytes, thiscall, `ret 8` (CWideString by hidden pointer; `this` unused).
// Identity (proven): Ego_r.pdb ?GetText@CTCBetting@@QAE?AVCWideString@@K@Z (each minigame class has its own copy); its callers are CTCBetting::FrameUpdate 0x007E4E30, the betting GetBetText 0x007E50C0 and CTCBetting::GetPlayersMoney 0x007E5570.
//   The manifest's "CTCBetting::GetText" is propagated across byte-identical bodies.
// Callees: CWideString(const wchar_t*) 0x0099B6B0 and atexit 0x004012BC (the function-local static at 0x013BAE84, guard bit
//   0x013BAE88), NGameText::CDataBank::GetPTextEntry(unsigned long, bool) const 0x009C9070 (CIVCountedPointer by hidden pointer),
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
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private:
public:
    unsigned char         _pad00[0x14];
    NGameText::CDataBank* TextBank;                       // +0x14
};
class CTCBetting {
public:
    CWideString GetText(unsigned long textId);
};
CWideString CTCBetting::GetText(unsigned long textId)
{
    static CWideString no_text_entry(L"Missing text entry");    // 0x013BAE84
    CIVCountedPointer<const NGameText::CTextEntry> entry = CMainGameComponent::Get()->TextBank->GetPTextEntry(textId, true);
    return entry.Object ? entry.Object->GetText() : no_text_entry;
}