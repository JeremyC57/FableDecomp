// CGraphicDataBank::SetTextureManager(const CCountedPointer<CTextureManager>& manager) -- share the texture manager with this graphic bank (counted-pointer assignment into TextureManager at retail +0x280)
// Leaf: ?SetTextureManager@CGraphicDataBank@@QAEXABV?$CCountedPointer@VCTextureManager@@@@@Z
// Flags: /O2 /Oy /W3
// Retail: Fable.exe @ 0x009FD870, 86 bytes, `ret 4`; callers 0x00B2C88C (CEngineSurfaceCompositionManager::
// AddCustomMorphTexture) and 0x00B2CAE7 (CopyIntermediateBank, landed 0x00B2C8E0, which already names this callee
// CGraphicDataBank::SetTextureManager); ghidra_out/labels_chunk286_corrections_2026-10-22.tsv carries the same correction.
// Byte family: 0x00669280 / 0x00992470 / 0x009FD870 differ only in the member offset; the manifest propagated NUISystem::
// CFrontEndScreen::AddActionOnLeftClicked (really 0x0054E4F0, vtable slot 71, +0x160) onto them -- 0x009FD870 is in no
// vtable. Identity: FableWin.pdb / Ego_r.pdb ?SetTextureManager@CGraphicDataBank@@QAEXABV?$CCountedPointer@VCTextureManager@@@@@Z
// (Ego_r.pdb 0x00B57DD0); Ego_r.pdb CGraphicDataBank has CCountedPointer<CTextureManager> TextureManager at +0x2FC in a 0x388
// class, retail keeps it at +0x280 (the debug build's larger CArray/StateBlock members ahead of it).
// Body: Member = manager -- the inline CCountedPointer::operator= / ShareData: unless the info blocks match, release the old
// one (`--count`, re-read; at zero call its destroy thunk (+4) on the item (+8), then operator delete 0x00BFE9BC on the
// block), copy Data / Info, `++count` on the new block.
// Layout: CCountedPointer {T* Data; CCPPointerInfo* Info}; CCPPointerInfo {long Count; destroy thunk +4; item +8}.
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
class CTextureManager;
class CGraphicDataBank {
public:
    unsigned char                    _pad00[0x280];
    CCountedPointer<CTextureManager> TextureManager;     // retail +0x280 (Ego_r.pdb +0x2FC)
    void SetTextureManager(const CCountedPointer<CTextureManager>& manager);
};
void CGraphicDataBank::SetTextureManager(const CCountedPointer<CTextureManager>& manager)
{
    TextureManager = manager;
}
