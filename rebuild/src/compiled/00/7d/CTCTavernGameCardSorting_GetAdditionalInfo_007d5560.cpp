// CTCTavernGameCardSorting::GetAdditionalInfo() -- the def's "additional info" text followed by "<round>/<rounds>": the current card layout (layouts minus rounds left, plus one) and the card def's layout count
// Leaf: GetAdditionalInfo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D5560, 359 bytes (`ret 4`, CWideString by hidden pointer);
// CTCTavernGameCardSorting vtable 0x0126F5B4 slot 46 (GetAdditionalInfo, as ShoveHaPenny 0x007CFF40, landed).
// Callees: CTCTavernGameCardBase::GetCardDef(CDefPointer<const CTavernGameCardBaseDef>*) 0x008F0B20;
//          CTCTavernGame::GetTavernGameDef(CDefPointer*) 0x005EA000; GFIntToCharString(long) 0x0099F830 (`__fastcall`,
//          hidden result in ecx); CCharString::ToWideString() const 0x0099E480; CTCTavernGame::GetText(unsigned long)
//          0x005E9E70; operator+(const CWideString&, const CWideString&) 0x0099BE70 and operator+(const CWideString&,
//          const wchar_t*) 0x0099BF30 (both `__fastcall`); ~CWideString 0x0099B510; ~CCharString 0x0099EAE0; the defs'
//          virtual slot 1 on release. Literal L"/" 0x0122DE74.
// Layout (retail): CTCTavernGame NumRounds +0x24 (rounds left); CTavernGameDef::AdditionalInfo +0xc0;
//          CTavernGameCardBaseDef CardOrders +0x6c (0x48-byte CCardPositionsDef records, as the landed EndRound 0x007D6150).
// Shape: as the landed ShoveHaPenny GetAdditionalInfo: the `+` chain is evaluated right to left (the layout count is
//        formatted first); the tavern def's dtor clears the field after the release, the card def's does not; both
//        releases are the DecRef-then-GetRefCount form.
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

template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
};
class CCardPositionsDef { public:
    unsigned char _data[0x48];
};
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
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
class CTavernGameDef : public CDefBase { public:
    unsigned char _pad_0x08[0xb8];
    unsigned long AdditionalInfo;               // +0xc0
};
class CTavernGameCardBaseDef : public CDefBase { public:
    unsigned char _pad_0x08[0x64];
    CRetailVector<CCardPositionsDef> CardOrders;   // +0x6c
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
struct CCardDefPointer {                        // CDefPointer<const CTavernGameCardBaseDef>
    CTavernGameCardBaseDef* Object;
    CCardDefPointer() : Object(0) {}
    ~CCardDefPointer() { if (Object) Object->ReleaseTemporary(); }
    CTavernGameCardBaseDef* operator->() const { return Object; }
};

class CTCTavernGame { public:
    virtual void v00();
    unsigned char _pad_0x04[0x20];
    long          NumRounds;                    // +0x24
    void GetTavernGameDef(CDefPointer* out);    // 0x005EA000
    CWideString GetText(unsigned long id);      // 0x005E9E70
};
struct CTCTavernGameCardBase : CTCTavernGame {
    void GetCardDef(CCardDefPointer* out);      // 0x008F0B20
};
struct CTCTavernGameCardSorting : CTCTavernGameCardBase {
    CWideString GetAdditionalInfo();
};

CWideString CTCTavernGameCardSorting::GetAdditionalInfo()
{
    CCardDefPointer cardDef;
    GetCardDef(&cardDef);
    CDefPointer def;
    GetTavernGameDef(&def);
    return GetText(def->AdditionalInfo) + GFIntToCharString(cardDef->CardOrders.size() - NumRounds + 1).ToWideString()
         + L"/" + GFIntToCharString(cardDef->CardOrders.size()).ToWideString();
}