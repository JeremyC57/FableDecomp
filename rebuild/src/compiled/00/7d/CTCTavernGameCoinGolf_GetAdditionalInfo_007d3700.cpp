// CTCTavernGameCoinGolf::GetAdditionalInfo() -- the def's "additional info" text followed by "<course>/<courses>": the current course + 1 and the coin-golf def's course count
// Leaf: GetAdditionalInfo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D3700, 287 bytes (`ret 4`, CWideString by hidden pointer);
// CTCTavernGameCoinGolf vtable 0x0126F49C slot 46 (Ego_r.pdb `?GetAdditionalInfo@CTCTavernGameCoinGolf@@UAE?AVCWideString@@XZ`;
// the manifest row says "vfunc_46").
// Callees: CTCTavernGame::GetTavernGameDef(CDefPointer*) 0x005EA000; CDefClassBase::GetSubDef<CTavernGameCoinGolfDef>
//          (CDefPointer<const CTavernGameCoinGolfDef>*) 0x007D4BD0 (on the owner's def class, owner +0x70; its body names
//          "CTavernGameCoinGolfDef"); GFIntToCharString(long) 0x0099F830 (`__fastcall`, hidden result in ecx);
//          CCharString::ToWideString() const 0x0099E480; CTCTavernGame::GetText(unsigned long) 0x005E9E70;
//          operator+(const CWideString&, const CWideString&) 0x0099BE70 and operator+(const CWideString&, const wchar_t*)
//          0x0099BF30 (both `__fastcall`); ~CWideString 0x0099B510; ~CCharString 0x0099EAE0; the defs' slot 1 on release.
// Layout (retail): CTCBase::Owner +0x04, CThing def class +0x70; CTavernGameDef::AdditionalInfo +0xc0;
//          CTavernGameCoinGolfDef::Courses +0x50 (PDB offset holds); CTCTavernGameCoinGolf::CurrentCourse +0x17c (PDB +0x194).
// Shape: as the ShoveHaPenny override 0x007CFF40: VC evaluates the `+` chain right to left; the dereferenced sub-def's
//        release has no null test; the tavern def's dtor clears the field (`mov edx,edi ; test edx,edx`).
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
struct CTavernGameCoinGolfDef : CDefBase {
    unsigned char _pad_0x08[0x48];
    long          Courses;                      // +0x50
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
struct CCoinGolfDefPointer {                // CDefPointer<const CTavernGameCoinGolfDef>
    CTavernGameCoinGolfDef* Object;
    CCoinGolfDefPointer() : Object(0) {}
    ~CCoinGolfDefPointer() { if (Object) Object->Release(); }
    CTavernGameCoinGolfDef* operator->() const { return Object; }
};

struct CDefClassBase {
    bool GetSubDef(CCoinGolfDefPointer* out);   // 0x007D4BD0
};
CTavernGameCoinGolfDef* FindRef_CDefClassBase_GetSubDef_0(CDefClassBase* defClass);   // stand-in callee of the stand-in body below
// Stand-in body (the real 0x007D4BD0 is in this TU, out of line, see the landed CDefClassBase_GetSubDef007d4bd0): it only
// writes the def through the out parameter, so VC7.1 knows the caller's address-taken pointer does not escape the call
// (retail's same-TU knowledge: the sub-def stays in edi across every later call).
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CCoinGolfDefPointer* out)
{
    CTavernGameCoinGolfDef* def = FindRef_CDefClassBase_GetSubDef_0(this);
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
    unsigned char _pad_0x08[0x174];
    long          CurrentCourse;                // +0x17c
};

struct CTCTavernGameCoinGolf : CTCTavernGame {
    CWideString GetAdditionalInfo();
};

CWideString CTCTavernGameCoinGolf::GetAdditionalInfo()
{
    CDefPointer def;
    GetTavernGameDef(&def);
    CCoinGolfDefPointer golfDef;
    Owner->DefClass->GetSubDef(&golfDef);
    return GetText(def->AdditionalInfo) + GFIntToCharString(CurrentCourse + 1).ToWideString() + L"/"
         + GFIntToCharString(golfDef->Courses).ToWideString();
}