// NUISystem::CManager::Clear() -- destroy the UI manager singleton and forget it
// Leaf: ?Clear@CManager@NUISystem@@SIXXZ
// Retail: Fable.exe @ 0x0042FB40, 39 bytes, static __fastcall; Ego_r.pdb ?Clear@CManager@NUISystem@@SIXXZ. Catalog label
// std::pair<CCharString, CTCBase::CRegisteredTCInfo>::swap is wrong: the body is `delete Instance; Instance = 0` on the
// singleton pointer 0x013B8710 -- the out-of-line non-virtual ~CManager (0x0041E641, stores the NUISystem::CManager vptr
// 0x01230134; Ego_r.pdb ??1CManager@NUISystem@@IAE@XZ, protected and non-virtual) then operator delete 0x00BFE9BC.
// Callers 0x0042EA1E / 0x005954E5.
// Layout: the singleton member name is behavioural (Ego_r.pdb's GetInstance reads it); no fields are touched.
namespace NUISystem {
class CObservable {
public:
    virtual void _slot00();
};
class CManager : public CObservable {
public:
    static void __fastcall Clear();
    static CManager* Instance;                           // 0x013B8710
protected:
    ~CManager();                                         // 0x0041E641
};
void __fastcall CManager::Clear()
{
    delete Instance;
    Instance = 0;
}
}