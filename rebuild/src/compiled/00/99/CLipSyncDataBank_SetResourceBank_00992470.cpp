// CLipSyncDataBank::SetResourceBank(const CCountedPointer<CResourceBank>& bank) -- share the lip-sync bank's resource bank (counted-pointer assignment into ResourceBank at retail +0x170)
// Leaf: ?SetResourceBank@CLipSyncDataBank@@QAEXABV?$CCountedPointer@VCResourceBank@@@@@Z
// Flags: /O2 /Oy /W3
// Retail: Fable.exe @ 0x00992470, 86 bytes, `ret 4`; one caller 0x00991B32 (the "Register Sound Bank 1/2" routine at
// 0x009919C0). Byte family: 0x00669280 / 0x00992470 / 0x009FD870 (counted-pointer setters of three different classes) and
// the real NUISystem::CFrontEndScreen::AddActionOnLeftClicked 0x0054E4F0 (vtable 0x012497E4 slot 71, member +0x160);
// the manifest propagated that label onto all four -- 0x00992470 is NOT in any vtable.
// Owner evidence: the caller allocates 0x180 bytes, constructs them with 0x00A42B80 (stores vtable 0x0129D3D4 = RTTI
// .?AVCLipSyncDataBank@@, base ctor CBankFile::ConstructorInitialize 0x009D5F80), then calls this on that object with its
// own CCountedPointer member (esi+0x28). The ctor clears +0x164..+0x16C, the counted pointer at +0x170/+0x174, a byte at
// +0x178 and a dword at +0x17C, then Reset()s +0x170 with a new CResourceBank (0x009FC5F0, 0x7C bytes): Ego_r.pdb
// CLipSyncDataBank (0x200 bytes) ends ResourceBank +0x1E8 / SpeechAnalyser +0x1F0 / SkipUpdate +0x1F8 / PhonemeBank +0x1FC,
// so retail (0x180 bytes, no SpeechAnalyser) has ResourceBank +0x170 / SkipUpdate +0x178 / PhonemeBank +0x17C.
// The method name is descriptive (FableWin.pdb has only ?GetResourceBank@CLipSyncDataBank@@QAEAAVCResourceBank@@XZ and
// SetPhonemeBank 0x00B54310); the class is proven, the setter name is not.
// Body: Member = bank -- the inline CCountedPointer::operator= / ShareData: unless the info blocks match, release the old
// one (`--count`, re-read; at zero call its destroy thunk (+4) on the item (+8), then operator delete 0x00BFE9BC on the
// block), copy Data / Info, `++count` on the new block.
// Layout: CCountedPointer {T* Data; CCPPointerInfo* Info}; CCPPointerInfo {long Count; destroy thunk +4; item +8}
// (the landed 0x00695350 ShareData model).
// Shape: /O2; the source Data / Info bound to locals first (ebx / edi loaded before the compare).
class CCPPointerInfo { public:
    long  Count;                                         // +0x00
    void (__fastcall *Destroy)(void* item);              // +0x04
    void* Item;                                          // +0x08
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer& operator=(const CCountedPointer& o)
    {
        T* data = o.Data;
        CCPPointerInfo* info = o.Info;
        if (Info != info)
        {
            if (Info)
            {
                if (--Info->Count == 0)
                {
                    Info->Destroy(Info->Item);
                    ::operator delete(Info);
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->Count;
        }
        return *this;
    }
};
class CResourceBank;
class CLipSyncDataBank {
public:
    unsigned char                  _pad00[0x170];
    CCountedPointer<CResourceBank> ResourceBank;         // +0x170 (Ego_r.pdb +0x1E8)
    bool                           SkipUpdate;           // +0x178 (Ego_r.pdb +0x1F8)
    void*                          PhonemeBank;          // +0x17C (Ego_r.pdb +0x1FC)
    void SetResourceBank(const CCountedPointer<CResourceBank>& bank);
};
void CLipSyncDataBank::SetResourceBank(const CCountedPointer<CResourceBank>& bank)
{
    ResourceBank = bank;
}
