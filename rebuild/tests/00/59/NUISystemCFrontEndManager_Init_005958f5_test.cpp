#include <cstdio>
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

static int g_log[32], g_n; static void Log(int x) { if (g_n < 32) g_log[g_n++] = x; }
// 1 clear, 2 copy mesh, 3 SetMeshBank, 4 SetDefs, 5 Get, 6 SetDefaultValues, 7 GetProfileNames, 8 DeleteTemp, 9 ~vector, 10 ~graphics, 11 ~mesh
static CWideString g_names[3]; static const CWideString* g_deleted[3]; static int g_del;
template <class T> CCountedPointer<T>::CCountedPointer(const CCountedPointer& o) { Object = o.Object; Info = o.Info; Log(2); }
template <class T> CCountedPointer<T>::~CCountedPointer() { Log(Info == (void*)1 ? 10 : Info == (void*)2 ? 11 : 12); }
static CUserProfileManager g_upm;
template <class T> T& __fastcall CTBaseSingleton<T>::Get() { Log(5); return g_upm; }
void CUserProfileManager::SetDefaultValues() { Log(6); }
void CUserProfileManager::DeleteTempSavesForProfile(const CWideString& n) { Log(8); g_deleted[g_del++] = &n; }
template <class T> std::vector<T>::~vector() { Log(9); }
template <class K, class V> void std::map<K, V>::clear() { Log(1); _M_node_count = 0; }
static unsigned char g_mgrRaw[sizeof(NUISystem::CManager)];
NUISystem::CManager* __fastcall NUISystem::CManager::GetInstance() { return (NUISystem::CManager*)g_mgrRaw; }
static CMeshDataBank* g_meshSeen; static CDefinitionManager* g_defs;
void NUISystem::CManager::SetMeshBank(CCountedPointer<CMeshDataBank> b) { Log(3); g_meshSeen = b.Object; b.Info = (void*)3; }
void NUISystem::CManager::SetDefinitionManager(CDefinitionManager* d) { Log(4); g_defs = d; }
static int g_count = 3;
void NUISystem::CFrontEndManager::GetProfileNames(std::vector<CWideString>& n) { Log(7); n._M_start = g_names; n._M_finish = g_names + g_count; }
static void Call(NUISystem::CFrontEndManager& m)
{
    CCountedPointer<CGraphicDataBank>* g = (CCountedPointer<CGraphicDataBank>*)new char[8]; g->Object = (CGraphicDataBank*)0x10; g->Info = (void*)1;
    CCountedPointer<CMeshDataBank>* s = (CCountedPointer<CMeshDataBank>*)new char[8]; s->Object = (CMeshDataBank*)0x20; s->Info = (void*)2;
    m.Init(*g, *s, (CInputManager*)0x30, (CDefinitionManager*)0x40);
}
int main()
{
    static unsigned char raw[sizeof(NUISystem::CFrontEndManager)]; NUISystem::CFrontEndManager& m = *(NUISystem::CFrontEndManager*)raw;
    m.PScreenToActivate = (NUISystem::CComponent*)1; m.PScreenToDeactivate = (NUISystem::CComponent*)1;
    Call(m);
    NUISystem::CManager* mgr = (NUISystem::CManager*)g_mgrRaw;
    bool ok = m.PScreenToActivate == 0 && m.PScreenToDeactivate == 0 && mgr->PInputManager == (CInputManager*)0x30
           && g_defs == (CDefinitionManager*)0x40 && g_meshSeen == (CMeshDataBank*)0x20 && g_del == 3
           && g_deleted[0] == &g_names[0] && g_deleted[1] == &g_names[1] && g_deleted[2] == &g_names[2];
    // order (caller-side copies of the by-value args add their own 2s before Init's body)
    static const int want[] = { 1, 2, 3, 12, 4, 5, 6, 7, 5, 8, 5, 8, 5, 8, 9, 10, 11 };
    int start = 0; while (start < g_n && g_log[start] != 1) ++start;
    for (int i = 0; ok && i < 17; ++i) ok = g_log[start + i] == want[i];
    std::printf(ok ? "005958f5_TEST PASS\n" : "005958f5_TEST FAIL\n");
    return ok ? 0 : 1;
}