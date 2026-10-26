// CTCBetting::GetBetText() -- while betting is active, the betting def's "Bet" text followed by the current bet amount; otherwise an empty string
// Leaf: GetBetText
// Grade: RELOCATION_MATCH (retry helper C) -- the REAL GetSubDef<CBettingDef> body 0x007E5820 in the TU (noinline: name
// lookup, manager fetch, assign into `out`) lets VC7.1 keep the out pointer as retail (zeroed through it, reloaded only on
// the miss, separate miss paths); then the thing def as a named local fixes the eax/ecx colouring (was DIFFER(207v177)).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E50C0, 175 bytes (`ret 4`: the CWideString result's hidden pointer).
// Label: the catalog says CTCSinglePersonOccupiable::IsOfType (propagated); the body sits between CTCBetting members
// (0x007E4E30 FrameUpdate, 0x007E51D0), is called from CTCBetting::DrawPreparePrimitivesForRendering (0x007E5704), reads the
// betting globals and the 'CBettingDef' sub def -- named GetBetText here (the PDB name is not known).
// Callees: CDefClassBase::GetSubDef_007e5820(CDefPointer&) @ 0x007E5820 (the 'CBettingDef' instantiation), IntToWideString(long)
// @ 0x00405F00 (__fastcall, hidden result in ecx), CTCBetting::GetText(unsigned long) @ 0x007E4D70, operator+(const CWideString&,
// const CWideString&) @ 0x0099BE70 (__fastcall), ~CWideString @ 0x0099B510, CWideString(const wchar_t*) @ 0x0099B6B0 (L"" @
// 0x0122D70C); globals g_bettingActive @ 0x013BAE1D, g_bettingAmount @ 0x013BAE20 (the landed GetBettingActive/Amount spellings).
// Layout: CThing PDef +0x70; CBettingDef (def at Ego_r.pdb offsets) Bet text id +0x44.
// Shape: nested tests (the two misses reach the empty-string path separately), then `return GetText(id) + IntToWideString(amount);` built straight into the hidden result (the right
// operand first); the def out pointer starts in the zero register, is released without a null test on the found path
// (already dereferenced) and with one on the empty-string path.
#include <stddef.h>
class CWideString {
public:
    void* PStringData;
    CWideString(const wchar_t* text);                   // 0x0099B6B0
    CWideString(const CWideString& other);
    ~CWideString();                                     // 0x0099B510
};
CWideString __fastcall IntToWideString(long value);     // 0x00405F00
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);   // 0x0099BE70
extern bool g_bettingActive;                            // 0x013BAE1D
extern long g_bettingAmount;                            // 0x013BAE20
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
class CBettingDef : public CDefBase { public:
    unsigned char _pad08[0x44 - 0x08];
    unsigned long Bet;                                  // +0x44 text id
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CDefSubRef { long DefIndex; };
class CDefinitionManager { public:
    CDefPointer<CDefBase> GetDef(long index);           // 0x009ADA10 (hidden result)
};
class CDefClassBase : public CDefBase { public:
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual const CDefSubRef* FindSubDef(const CCharString& typeName);   // slot 14 (+0x38)
    unsigned char _pad08[0x1c - 0x08];
    CDefinitionManager* DefManager;                     // +0x1c
    bool GetSubDef_007e5820(CDefPointer<CBettingDef>& out);   // 0x007E5820
};
// 0x007E5820 CDefClassBase::GetSubDef<CBettingDef>, the retail body: look the sub def up by its class name, fetch it from
// the definition manager and assign it into `out` (addref/release); only `out` is written, so the caller's out word
// does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef_007e5820(CDefPointer<CBettingDef>& out)
{
    const CDefSubRef* sub = FindSubDef(CCharString("CBettingDef"));
    if (!sub)
        return false;
    CBettingDef* def;
    {
        CDefPointer<CDefBase> fetched = DefManager->GetDef(sub->DefIndex);
        def = (CBettingDef*)fetched.Object;
        if (def)
            ++def->RefCount;
        if (fetched.Object) {
            fetched.Object->Release();
            fetched.Object = 0;
        }
    }
    if (out.Object != def) {
        if (out.Object) {
            out.Object->Release();
            out.Object = 0;
        }
        out.Object = def;
        if (def)
            ++def->RefCount;
    }
    if (def)
        def->Release();
    return true;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;                                // +0x70
};
class CTCBetting { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    CWideString GetText(unsigned long textId);          // 0x007E4D70
    CWideString GetBetText();
};

CWideString CTCBetting::GetBetText()
{
    CDefPointer<CBettingDef> bettingDef;
    if (g_bettingActive) {
        CDefClassBase* thingDef = ParentThing->PDef;
        if (thingDef->GetSubDef_007e5820(bettingDef))
            return GetText(bettingDef->Bet) + IntToWideString(g_bettingAmount);
    }
    return CWideString(L"");
}