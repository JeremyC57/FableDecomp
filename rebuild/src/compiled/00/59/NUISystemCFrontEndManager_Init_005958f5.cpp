// NUISystem::CFrontEndManager::Init(CCountedPointer<CGraphicDataBank>, CCountedPointer<CMeshDataBank>, CInputManager*, CDefinitionManager*) -- boot the front end: clear signatures, hand the UI manager its mesh bank, input and definitions, reset screen transitions and register the existing profiles
// Leaf: ?Init@CFrontEndManager@NUISystem@@QAEXV?$CCountedPointer@VCGraphicDataBank@@@@V?$CCountedPointer@VCMeshDataBank@@@@PAVCInputManager@@PAVCDefinitionManager@@@Z
// Retail: Fable.exe @ 0x005958F5, 182 bytes, `ret 18h`; Ego_r.pdb same symbol.
// Body: Signatures.clear() (map<CWideString,bool> clear 0x00485AEF); CManager::GetInstance() (0x0041E5F2)
// ->SetMeshBank(meshes) (0x0041D198, by-value CCountedPointer built in the argument area by the copy ctor 0x00428905);
// GetInstance()->PInputManager (+0x20) = input; GetInstance()->SetDefinitionManager(definitions) (0x0041CD89, stores +0x64);
// PScreenToActivate = PScreenToDeactivate = 0; CTBaseSingleton<CUserProfileManager>::Get() (0x0040D2A0)->SetDefaultValues()
// (0x00409B70); vector<CWideString> names; GetProfileNames(names) (0x005955AB); for each name Get().DeleteTempSavesForProfile
// (name) (0x00406770, an empty body on PC); ~vector 0x00412130; then the by-value parameters die: ~CCountedPointer
// <CGraphicDataBank> 0x00419108 (graphics is otherwise unused) and ~CCountedPointer<CMeshDataBank> 0x0042918C.
// Names: CManager / CFrontEndManager names are Ego_r.pdb symbols; SetDefaultValues and DeleteTempSavesForProfile are
// chosen among Ego_r.pdb CUserProfileManager QAEXXZ / QAEXABVCWideString members (bodies: default-value stores / empty).
// Layout: retail CFrontEndManager PScreenToDeactivate +0x98, PScreenToActivate +0x9c, Signatures +0xc4 (Ego_r.pdb +0xd8,
// the same -0x14 shift); CManager input pointer +0x20 (stored inline).
// Opt: size TU (unaligned entry) with a frame pointer (`leave`). Shape: the count and the element pointer are read once
// before the loop (retail `dec edi / add esi,4`); re-reading names.size() or names[i] each pass reloads both (192v182).
#pragma optimize("s", on)
#pragma optimize("y", off)
class CWideString { public: void* PStringData; };
class CGraphicDataBank;
class CMeshDataBank;
class CInputManager;
class CDefinitionManager;
template <class T> class CCountedPointer {
public:
    T*    Object;
    void* Info;
    CCountedPointer(const CCountedPointer& other);       // 0x00428905 for CMeshDataBank
    ~CCountedPointer();                                  // 0x00419108 (CGraphicDataBank) / 0x0042918C (CMeshDataBank)
};
class CUserProfileManager {
public:
    void SetDefaultValues();                             // 0x00409B70
    void DeleteTempSavesForProfile(const CWideString& name);   // 0x00406770
};
template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                          // 0x0040D2A0 for CUserProfileManager
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector();                                           // 0x00412130 for CWideString
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
template <class K, class V> class map {
public:
    void* _M_header;
    unsigned int _M_node_count;
    void clear();                                        // 0x00485AEF for <CWideString, bool>
};
}
namespace NUISystem {
class CComponent;
class CManager {
public:
    unsigned char  _pad00[0x20];
    CInputManager* PInputManager;                        // +0x20
    static CManager* __fastcall GetInstance();           // 0x0041E5F2
    void SetMeshBank(CCountedPointer<CMeshDataBank> bank);          // 0x0041D198
    void SetDefinitionManager(CDefinitionManager* definitions);    // 0x0041CD89
};
class CFrontEndManager {
public:
    unsigned char                    _pad00[0x98];
    CComponent*                      PScreenToDeactivate;   // +0x98
    CComponent*                      PScreenToActivate;     // +0x9c
    unsigned char                    _padA0[0xc4 - 0xa0];
    std::map<CWideString, bool>      Signatures;            // +0xc4
    void GetProfileNames(std::vector<CWideString>& names);   // 0x005955AB
    void Init(CCountedPointer<CGraphicDataBank> graphics, CCountedPointer<CMeshDataBank> meshes, CInputManager* input,
              CDefinitionManager* definitions);
};
void CFrontEndManager::Init(CCountedPointer<CGraphicDataBank> graphics, CCountedPointer<CMeshDataBank> meshes,
                            CInputManager* input, CDefinitionManager* definitions)
{
    Signatures.clear();
    CManager::GetInstance()->SetMeshBank(meshes);
    CManager::GetInstance()->PInputManager = input;
    CManager::GetInstance()->SetDefinitionManager(definitions);
    PScreenToActivate = 0;
    PScreenToDeactivate = 0;
    CTBaseSingleton<CUserProfileManager>::Get().SetDefaultValues();
    std::vector<CWideString> names;
    GetProfileNames(names);
    const unsigned int count = names.size();
    CWideString* name = names.begin();
    for (unsigned int i = 0; i < count; ++i)
        CTBaseSingleton<CUserProfileManager>::Get().DeleteTempSavesForProfile(name[i]);
}
}