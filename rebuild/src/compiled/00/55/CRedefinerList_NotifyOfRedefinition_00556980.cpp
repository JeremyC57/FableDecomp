// CRedefinerList::NotifyOfRedefinition(const CActionInputControl& control, const CKeyRedefiner* source) -- a key was redefined on one redefiner: every OTHER redefiner of the list drops its duplicate definitions of that control, then the undefined-controls warning is refreshed (0.2 s)
// Leaf: NotifyOfRedefinition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00556980, length 97 bytes.
// Identity: the manifest label "CManager::DeregisterAllScreens" is wrong. The callees carry NUISystem::CRedefinerList / CKeyRedefiner
//   labels (CKeyRedefiner::ClearDuplicateDefinitions @ 0x005580B0, CRedefinerList::UpdateUndefinedWarning(float) @ 0x00556810) and the
//   PDB symbol ?NotifyOfRedefinition@CRedefinerList@NUISystem@@QAEXABVCActionInputControl@@PBVCKeyRedefiner@2@@Z has exactly this
//   argument shape (control in ebp, the source redefiner in ebx).
// Layout: the redefiner vector (STLport, 8-byte entries, redefiner pointer first) at +0xb0 in retail (the PDB layout is padding only here).
// Shape: an unsigned index loop over size() (`sar 3` of the byte span, `jb`), size re-read every iteration; the arguments are loaded
//   inside the non-empty guard; the float 0.2f is pushed as an immediate.
class CActionInputControl;
struct CKeyRedefiner {
    void ClearDuplicateDefinitions(const CActionInputControl& control);   // 0x005580B0
};
struct SRedefinerEntry {                           // 8-byte element of the redefiner vector
    CKeyRedefiner* m_pRedefiner;
    long           m_data;
};
struct CRedefinerVector {                          // STLport vector: begin / end / end of storage
    SRedefinerEntry* _M_start;
    SRedefinerEntry* _M_finish;
    SRedefinerEntry* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    SRedefinerEntry& operator[](unsigned long n) { return *(_M_start + n); }
};
struct CRedefinerList {
    unsigned char    _pad00[0xb0];
    CRedefinerVector m_redefiners;                 // +0xb0
    void UpdateUndefinedWarning(float delay);      // 0x00556810
    void NotifyOfRedefinition(const CActionInputControl& control, const CKeyRedefiner* source);
};
void CRedefinerList::NotifyOfRedefinition(const CActionInputControl& control, const CKeyRedefiner* source)
{
    for (unsigned long i = 0; i < m_redefiners.size(); ++i) {
        CKeyRedefiner* redefiner = m_redefiners[i].m_pRedefiner;
        if (redefiner && redefiner != source)
            redefiner->ClearDuplicateDefinitions(control);
    }
    UpdateUndefinedWarning(0.2f);
}