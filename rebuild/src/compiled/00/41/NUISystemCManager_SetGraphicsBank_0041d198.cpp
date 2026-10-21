// NUISystem::CManager::SetGraphicsBank(CCountedPointer<CGraphicDataBank> bank) -- stores the shared graphics bank pointer
// Retail: Fable.exe @ 0x0041D198, 28 bytes, `ret 8` (the counted pointer is passed by value and destroyed by the callee).
// Body: PGraphicsBank = bank -> CCountedPointer::ShareData(bank.Data, bank.Info) (0x004291B8) on the member, then the
// by-value parameter's destructor (0x0042918C).
// Layout: retail PGraphicsBank at +0x18; engine/CManager.h has the Ego_r.pdb +0x20 (the two maps ahead of it are
// 4 bytes smaller each in the STLport retail build), so the class is modelled locally.
// Opt: size TU (`push dword ptr [esp+8]` forwarding twice).
#pragma optimize("s", on)
class CGraphicDataBank;
struct CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& o);
    ~CCountedPointer();                                          // 0x0042918C
    void ShareData(T* data, CCPPointerInfo* info);               // 0x004291B8
    CCountedPointer& operator=(const CCountedPointer& o) { ShareData(o.Data, o.Info); return *this; }
};
namespace NUISystem {
class CManager {
public:
    void*                             __vftable;                 // +0x00
    unsigned char                     _pad04[0x14];              // +0x04
    CCountedPointer<CGraphicDataBank> PGraphicsBank;             // +0x18
    void SetGraphicsBank(CCountedPointer<CGraphicDataBank> bank);
};
void CManager::SetGraphicsBank(CCountedPointer<CGraphicDataBank> bank)
{
    PGraphicsBank = bank;
}
}