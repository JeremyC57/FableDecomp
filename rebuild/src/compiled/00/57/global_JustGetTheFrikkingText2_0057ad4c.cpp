// JustGetTheFrikkingText2(const CCharString& textId) -- the game text for a text tag: the data bank's entry text when the tag's CRC is known, else the tag itself widened
// Leaf: ?JustGetTheFrikkingText2@@YI?AVCWideString@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057AD4C, 108 bytes, `CWideString __fastcall JustGetTheFrikkingText2(const
//   CCharString&)` (FableWin.pdb `?JustGetTheFrikkingText2@@YI?AVCWideString@@ABVCCharString@@@Z`; hidden result in
//   ecx, the tag in edx). No direct caller in retail (a per-TU copy in the CTCHeroStats module).
// Callees: CCharString::GetCRC() const @ 0x00404310 (out-of-line copy), NGameText::CDataBank::LookupValue @ 0x009CC410
//   on the world's data bank (0x013B86A0 +0x14), CCharString::ToWideString() const @ 0x0099E480 (unknown tag),
//   CDataBank::GetPTextEntry(id, true) const @ 0x009C9070 (CIVCountedPointer<CTextEntry> by hidden pointer), the
//   entry's text getter @ 0x009C7A40, and the counted pointer's inline release (virtual slot 1 at zero).
// Shape: `CDataBank* bank = g_pWorld->PDataBank; id = bank->LookupValue(tag.GetCRC()); if (!id) return
//   tag.ToWideString(); return g_pWorld->GetPDataBank()->GetPTextEntry(id, true)->GetText();` -- the first bank
//   read is a named local (loaded into esi BEFORE the CRC call; the inline expression evaluates the CRC first), the
//   second goes through an inline accessor (straight into ecx before the pushes; the direct member chain loads
//   eax after them); CWideString needs its user copy ctor/dtor for the hidden-pointer returns; the counted-pointer
//   temporary is homed in the frame word and released after the text is copied out (landed model:
//   CTCRegionDisplay::SetCurrentAreaName 0x00639BA0). Frame-pointer size TU.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CWideStringData;
class CWideString {
public:
    CWideStringData* m_pData;
    CWideString(const CWideString& other);                  // 0x0099B720
    ~CWideString();                                         // 0x0099B510
};
class CCharString {
public:
    struct CStringData* PStringData;
    unsigned long GetCRC() const;                           // 0x00404310
    CWideString ToWideString() const;                       // 0x0099E480
};

struct CCounted {
    virtual void v00();
    virtual void Destroy();
    long RefCount;
    void DecRef() { --RefCount; }
    long GetRefCount() const { return RefCount; }
    void Release()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CTextEntry : CCounted {
    CWideString GetText() const;                            // 0x009C7A40
};
template <class T> class CIVCountedPointer { public:
    T* Data;
    __forceinline ~CIVCountedPointer()
    {
        if (Data) {
            Data->Release();
            Data = 0;
        }
    }
    T* operator->() const { return Data; }
};
struct CDataBank {
    unsigned long LookupValue(unsigned long crc);           // 0x009CC410
    CIVCountedPointer<CTextEntry> GetPTextEntry(unsigned long id, bool flag) const;   // 0x009C9070
};
struct CWorldGlobal {
    unsigned char _pad00[0x14];
    CDataBank* PDataBank;                                   // +0x14
    CDataBank* GetPDataBank() const { return PDataBank; }
};
extern CWorldGlobal* g_pWorld;                              // 0x013B86A0

CWideString __fastcall JustGetTheFrikkingText2(const CCharString& textId)
{
    CDataBank* bank = g_pWorld->PDataBank;
    unsigned long id = bank->LookupValue(textId.GetCRC());
    if (!id)
        return textId.ToWideString();
    return g_pWorld->GetPDataBank()->GetPTextEntry(id, true)->GetText();
}