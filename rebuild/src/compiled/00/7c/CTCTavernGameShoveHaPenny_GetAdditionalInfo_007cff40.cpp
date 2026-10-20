// CTCTavernGameShoveHaPenny::GetAdditionalInfo() -- the def's "additional info" text followed by "<round>/<rounds>": the current round (rounds played + 1) and the shove-ha'penny def's round count
// Leaf: GetAdditionalInfo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CFF40, 290 bytes (`ret 4`, CWideString by hidden pointer);
// CTCTavernGameShoveHaPenny vtable 0x0126F2F4 slot 46 (Ego_r.pdb `?GetAdditionalInfo@CTCTavernGameShoveHaPenny@@UAE?AVCWideString@@XZ`).
// The manifest row says "CTCTavernGameCoinGolf::GetAdditionalInfo" (propagated; the CoinGolf override is 0x007D3700).
// Callees: CTCTavernGame::GetTavernGameDef(CDefPointer*) 0x005EA000; CDefClassBase::GetSubDef<CTavernGameShoveHaPennyDef>
//          (CDefPointer<const CTavernGameShoveHaPennyDef>*) 0x007D1260 (on the owner's def class, owner +0x70; manifest
//          label "<CAICreatureWillPowerIndicatorDef>" is propagated, the landed body names the ShoveHaPenny def string);
//          GFIntToCharString(long) 0x0099F830 (`__fastcall`, hidden result in ecx); CCharString::ToWideString() const
//          0x0099E480; CTCTavernGame::GetText(unsigned long) 0x005E9E70; operator+(const CWideString&, const CWideString&)
//          0x0099BE70 and operator+(const CWideString&, const wchar_t*) 0x0099BF30 (both `__fastcall`); ~CWideString
//          0x0099B510; ~CCharString 0x0099EAE0; the defs' virtual slot 1 on release.
// Layout (retail): CTCBase::Owner +0x04, CThing def class +0x70; CTavernGameDef::AdditionalInfo +0xc0;
//          CTavernGameShoveHaPennyDef::Rounds +0x1fc (PDB +0x204, the Regions vector is 12 bytes in retail);
//          CTCTavernGameShoveHaPenny::RoundsPlayed +0x148 (as the landed EnterGame 0x007CF530).
// Shape: VC evaluates the `+` chain right to left (the rounds total is formatted first); the typed sub-def pointer is
//        dereferenced before its dtor, so its release (`dec [edi+4] ; jne`) has no null test; the tavern def's dtor
//        clears the field after the release (`mov edx,edi ; test edx,edx`, the dead store elided).
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t
struct CWideString {
    void* m_data;
    ~CWideString();                             // 0x0099B510
};
struct CCharString {
    void* m_data;
    ~CCharString();                             // 0x0099EAE0
    CWideString ToWideString() const;           // 0x0099E480
};
CCharString __fastcall GFIntToCharString(long value);                         // 0x0099F830
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);  // 0x0099BE70
CWideString __fastcall operator+(const CWideString& a, const wchar_t* b);      // 0x0099BF30

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CDefBase : CDefVirtual, CRefCounted {
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CTavernGameDef : CDefBase {
    unsigned char _pad_0x08[0xb8];
    unsigned long AdditionalInfo;               // +0xc0
};
struct CTavernGameShoveHaPennyDef : CDefBase {
    unsigned char _pad_0x08[0x1f4];
    long          Rounds;                       // +0x1fc
};
struct CDefPointer {                            // CDefPointer<const CTavernGameDef>
    CTavernGameDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        if (Object) {
            Object->ReleaseTemporary();
            Object = 0;
        }
    }
    CTavernGameDef* operator->() const { return Object; }
};
struct CShoveHaPennyDefPointer {                // CDefPointer<const CTavernGameShoveHaPennyDef>
    CTavernGameShoveHaPennyDef* Object;
    CShoveHaPennyDefPointer() : Object(0) {}
    ~CShoveHaPennyDefPointer() { if (Object) Object->Release(); }
    CTavernGameShoveHaPennyDef* operator->() const { return Object; }
};

struct CDefClassBase {
    bool GetSubDef(CShoveHaPennyDefPointer* out);   // 0x007D1260
};
CTavernGameShoveHaPennyDef* FindRef_CDefClassBase_GetSubDef_0(CDefClassBase* defClass);   // stand-in callee of the stand-in body below
// Stand-in body (the real 0x007D1260 is in this TU, out of line, see the landed CDefClassBase_GetSubDef007d1260): it only
// writes the def through the out parameter, so VC7.1 knows the caller's address-taken pointer does not escape the call
// (retail's same-TU knowledge: the sub-def stays in edi across every later call).
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CShoveHaPennyDefPointer* out)
{
    CTavernGameShoveHaPennyDef* def = FindRef_CDefClassBase_GetSubDef_0(this);
    if (!def)
        return false;
    out->Object = def;
    return true;
}
#pragma auto_inline(on)

struct CThing {
    unsigned char  _pad_0x00[0x70];
    CDefClassBase* DefClass;                    // +0x70
};

struct CTCTavernGame {
    virtual void v00();
    CThing*       Owner;                        // +0x04
    void GetTavernGameDef(CDefPointer* out);    // 0x005EA000
    CWideString GetText(unsigned long id);      // 0x005E9E70
    unsigned char _pad_0x08[0x140];
    long          RoundsPlayed;                 // +0x148
};

struct CTCTavernGameShoveHaPenny : CTCTavernGame {
    CWideString GetAdditionalInfo();
};

CWideString CTCTavernGameShoveHaPenny::GetAdditionalInfo()
{
    CDefPointer def;
    GetTavernGameDef(&def);
    CShoveHaPennyDefPointer shoveDef;
    Owner->DefClass->GetSubDef(&shoveDef);
    return GetText(def->AdditionalInfo) + GFIntToCharString(RoundsPlayed + 1).ToWideString() + L"/"
         + GFIntToCharString(shoveDef->Rounds).ToWideString();
}