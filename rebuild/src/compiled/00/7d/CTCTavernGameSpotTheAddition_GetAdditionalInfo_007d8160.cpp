// CTCTavernGameSpotTheAddition::GetAdditionalInfo() -- the def's "additional info" text followed by "<round>/<rounds>": the current round + 1 and the number of rounds in the spot-the-addition def
// Leaf: GetAdditionalInfo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D8160, 299 bytes (`ret 4`, CWideString by hidden pointer);
// CTCTavernGameSpotTheAddition vtable 0x0126F814 slot 46 (Ego_r.pdb `?GetAdditionalInfo@CTCTavernGameSpotTheAddition@@UAE?AVCWideString@@XZ`;
// the manifest row says "vfunc_46").
// Callees: CTCTavernGame::GetTavernGameDef(CDefPointer*) 0x005EA000; CDefClassBase::GetSubDef<CTavernGameSpotTheAdditionDef>
//          (CDefPointer<const CTavernGameSpotTheAdditionDef>*) 0x007D9A80 (on the owner's def class, owner +0x70; its body
//          names "CTavernGameSpotTheAdditionDef"); GFIntToCharString(long) 0x0099F830 (`__fastcall`, hidden result in ecx);
//          CCharString::ToWideString() const 0x0099E480; CTCTavernGame::GetText(unsigned long) 0x005E9E70;
//          operator+(const CWideString&, const CWideString&) 0x0099BE70 and operator+(const CWideString&, const wchar_t*)
//          0x0099BF30 (both `__fastcall`); ~CWideString 0x0099B510; ~CCharString 0x0099EAE0; the defs' slot 1 on release.
// Layout (retail): CTCBase::Owner +0x04, CThing def class +0x70; CTavernGameDef::AdditionalInfo +0xc0;
//          CTavernGameSpotTheAdditionDef::ItemsPerRound (STLport vector<long>, 12 bytes) +0x44 (PDB +0x48: Knicknacks
//          is 12 bytes in retail); CTCTavernGameSpotTheAddition::CurrentRound +0xf0 (PDB +0x108).
// Shape: as the ShoveHaPenny override 0x007CFF40, but the round count is the inline `size()` of a vector member, which
//        is not a dereference of the def pointer, so the sub-def's release keeps its null test (`test edi,edi ; je`).
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t
class CWideString { public:
    void* m_data;
    ~CWideString();                             // 0x0099B510
};
class CCharString { public:
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
struct CLongVector {                            // STLport vector<long> (12 bytes in retail)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
};
class CTavernGameDef : public CDefBase { public:
    unsigned char _pad_0x08[0xb8];
    unsigned long AdditionalInfo;               // +0xc0
};
class CTavernGameSpotTheAdditionDef : public CDefBase { public:
    unsigned char _pad_0x08[0x3c];
    CLongVector   ItemsPerRound;                // +0x44
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
struct CSpotTheAdditionDefPointer {                // CDefPointer<const CTavernGameSpotTheAdditionDef>
    CTavernGameSpotTheAdditionDef* Object;
    CSpotTheAdditionDefPointer() : Object(0) {}
    ~CSpotTheAdditionDefPointer() { if (Object) Object->Release(); }
    CTavernGameSpotTheAdditionDef* operator->() const { return Object; }
};

class CDefClassBase { public:
    bool GetSubDef(CSpotTheAdditionDefPointer* out);   // 0x007D9A80
};
CTavernGameSpotTheAdditionDef* FindRef_CDefClassBase_GetSubDef_0(CDefClassBase* defClass);   // stand-in callee of the stand-in body below
// Stand-in body (the real 0x007D9A80 is in this TU, out of line, see the landed CDefClassBase_GetSubDef007d9a80): it only
// writes the def through the out parameter, so VC7.1 knows the caller's address-taken pointer does not escape the call
// (retail's same-TU knowledge: the sub-def stays in edi across every later call).
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CSpotTheAdditionDefPointer* out)
{
    CTavernGameSpotTheAdditionDef* def = FindRef_CDefClassBase_GetSubDef_0(this);
    if (!def)
        return false;
    out->Object = def;
    return true;
}
#pragma auto_inline(on)

class CThing { public:
    unsigned char  _pad_0x00[0x70];
    CDefClassBase* DefClass;                    // +0x70
};

class CTCTavernGame { public:
    virtual void v00();
    CThing*       Owner;                        // +0x04
    void GetTavernGameDef(CDefPointer* out);    // 0x005EA000
    CWideString GetText(unsigned long id);      // 0x005E9E70
    unsigned char _pad_0x08[0xe8];
    long          CurrentRound;                 // +0xf0
};

struct CTCTavernGameSpotTheAddition : CTCTavernGame {
    CWideString GetAdditionalInfo();
};

CWideString CTCTavernGameSpotTheAddition::GetAdditionalInfo()
{
    CDefPointer def;
    GetTavernGameDef(&def);
    CSpotTheAdditionDefPointer spotDef;
    Owner->DefClass->GetSubDef(&spotDef);
    return GetText(def->AdditionalInfo) + GFIntToCharString(CurrentRound + 1).ToWideString() + L"/"
         + GFIntToCharString(spotDef->ItemsPerRound.size()).ToWideString();
}