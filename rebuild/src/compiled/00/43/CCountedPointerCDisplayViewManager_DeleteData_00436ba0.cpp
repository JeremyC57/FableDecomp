// CCountedPointer<CDisplayViewManager>::DeleteData(void* data) -- the counted pointer's per-type deleter: destroy the view manager (releasing its current-view pointer) and free it
// Leaf: DeleteData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00436BA0, length 66 bytes, plain `ret` (static __fastcall: the object in ecx).
// Name: the shape of FableWin.pdb `?DeleteData@?$CCountedPointer@...@@SIXPAX@Z`; its address is stored as CCPPointerInfo::DeleteFunc
// by CCountedPointer<CDisplayViewManager>::Reset 0x00436CC0 (landed; `mov dword ptr [eax+4], 0x436BA0`, the only reference,
// 0x00436D10), which CDisplayEngine::Init 0x004350D0 calls for PViewManager. The catalog's "DeleteData" is right.
// Callees: the inner pointer's info block DeleteFunc (+4, the data at +8 in ecx), operator delete @ 0x00BFE9BC (twice).
// Shape: /O2; `delete static_cast<T*>(data)` = null test, the inlined implicit ~CDisplayViewManager, i.e. ~CCountedPointer on
// PCurrentView (--count re-read through the info pointer, at zero DeleteFunc(info->Data) and operator delete(info); then
// Data = Info = 0 -- the two zero stores scheduled after `push esi`), then operator delete(data). Same destructor model as the
// matched CGraphicFrameData::Release 0x0041C090.
// Layout: Ego_r.pdb CDisplayViewManager (8 bytes) = CCountedPointer<NDisplayView::CViewBase> PCurrentView; CCPPointerInfo
// {RefCount +0, DeleteFunc +4, Data +8}.
// Model: local -- engine/CDisplayViewManager.h exists (PCurrentView +0, same offset), but it flattens the counted pointer into
// two raw words; the inlined destructor needs ~CCountedPointer, so the class is local.
void __cdecl operator delete(void* block);               // 0x00BFE9BC
class CCPPointerInfo { public:
    long  RefCount;                                      // +0x00
    void  (__fastcall* DeleteFunc)(void* data);          // +0x04
    void* Data;                                          // +0x08
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                // +0x00
    CCPPointerInfo* Info;                                // +0x04
    __forceinline ~CCountedPointer()
    {
        if (Info)
        {
            --Info->RefCount;
            if (Info->RefCount == 0)
            {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
    static void __fastcall DeleteData(void* data)
    {
        delete static_cast<T*>(data);
    }
};
namespace NDisplayView { class CViewBase; }
class CDisplayViewManager {                              // 8 bytes
public:
    CDisplayViewManager();                               // 0x00640040
    CCountedPointer<NDisplayView::CViewBase> PCurrentView;   // +0x00
};
template class CCountedPointer<CDisplayViewManager>;