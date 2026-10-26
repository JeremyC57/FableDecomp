// Behaviour test for 0059d1b1: src/0059d1b1.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCInventoryTrade::FinalRefreshSubtitles(NUISystem::CComponent* component, CSubtitleMap& subtitles)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCInventoryTrade::FinalRefreshSubtitles(NUISystem::CComponent* component, CSubtitleMap& subtitles) -- rewrites the one-off subtitle child of `component`: the component's own type-6 child text when the component has no entry in `subtitles`, otherwise the entry's text followed by " x N" (the localised multiplier and the count) when the count is positive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059D1B1, length 191 bytes (`ret 8`). Frame-pointer size TU: optimize("y",off) + ("s",on).
// A byte twin of 0x0059D761 (the PC_SUBTITLE child); the oracle labels both FinalRefreshSubtitles.
// Callees: map<CComponent*,...>::find @ 0x005B1A8D (catalog "std::set_FindLE_Duplicate"; STLport node: key +0x10, text +0x14, count
// +0x18; end() is the header at +0), CWideString::CWideString() @ 0x0099AED0, operator=(const CWideString&) @ 0x0099B7D0, operator+=
// (const CWideString&) @ 0x0099B8D0, ~CWideString @ 0x0099B510, GetXString() @ 0x005CC772 and IntToWideString(long) @ 0x00405F00
// (`__fastcall`, CWideString by value through the hidden pointer in ecx), CCharString(const char*, long) @ 0x0099EBF0 ("UI_ONE_OFF_SUBTITLE"
// 0x0124E72C), NUISystem::CComponent virtual slots 9 (+0x24, the typed child getter), 142 (+0x238, the child's text), 3 (+0x0C, FindChild
// by name, CCharString by value) and 144 (+0x240, SetText(const CWideString&, bool, bool)).
// Shape: the map reference is read into edi before the find and the component replaces it there afterwards; the text local lives in
// the dead map argument slot ([ebp+0Ch]); the two by-value temporaries of the count arm are homed in the dead component slot ([ebp+8])
// and destroyed right after each `+=`; the FindChild name is built in place on the stack (`push ecx ; mov ecx,esp`).
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
};
CWideString __fastcall GetXString();                    // 0x005CC772
CWideString __fastcall IntToWideString(long value);     // 0x00405F00
namespace NUISystem {
struct CTextSource {                                    // the typed child: only its text slot is used here
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77();
    virtual void v78(); virtual void v79(); virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83();
    virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88(); virtual void v89();
    virtual void v90(); virtual void v91(); virtual void v92(); virtual void v93(); virtual void v94(); virtual void v95();
    virtual void v96(); virtual void v97(); virtual void v98(); virtual void v99(); virtual void v100(); virtual void v101();
    virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113();
    virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125();
    virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131();
    virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137();
    virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141();
    virtual const CWideString& GetText();               // slot 142 (+0x238)
};
class CComponent { public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CComponent* FindChild(CCharString name);    // slot 3 (+0x0c)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual CTextSource* GetChildOfType(long type);     // slot 9 (+0x24)
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57();
    virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75();
    virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79(); virtual void v80(); virtual void v81();
    virtual void v82(); virtual void v83(); virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87();
    virtual void v88(); virtual void v89(); virtual void v90(); virtual void v91(); virtual void v92(); virtual void v93();
    virtual void v94(); virtual void v95(); virtual void v96(); virtual void v97(); virtual void v98(); virtual void v99();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105();
    virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111();
    virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117();
    virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123();
    virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135();
    virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141();
    virtual void v142(); virtual void v143();
    virtual void SetText(const CWideString& text, bool a, bool b);   // slot 144 (+0x240)
};
}
struct SSubtitleNode {                                  // STLport _Rb_tree_node<pair<CComponent* const, pair<CWideString,long>>>
    unsigned char          _base[0x10];
    NUISystem::CComponent* Component;                   // +0x10
    CWideString            Text;                        // +0x14
    long                   Count;                       // +0x18
};
struct CSubtitleMap {                                   // std::map<NUISystem::CComponent*, ...> (12 bytes)
    SSubtitleNode* _M_header;
    unsigned long  _M_node_count;
    long           _M_key_compare;
    SSubtitleNode* find(NUISystem::CComponent* const& key);   // 0x005B1A8D
    SSubtitleNode* end() const { return _M_header; }
};
// Same-TU stand-in for the STLport map::find 0x005B1A8D: reads the key through its const reference (lower_bound walk), so
// VC7.1 knows the component argument's address goes no further and reuses its dead slot.
#pragma auto_inline(off)
__declspec(noinline) SSubtitleNode* CSubtitleMap::find(NUISystem::CComponent* const& key)
{
    SSubtitleNode* y = _M_header;
    SSubtitleNode* x = *(SSubtitleNode**)((unsigned char*)_M_header + 4);
    while (x != 0) {
        if (!(x->Component < key)) { y = x; x = *(SSubtitleNode**)((unsigned char*)x + 8); }
        else x = *(SSubtitleNode**)((unsigned char*)x + 0xc);
    }
    if (y == _M_header || key < y->Component) return _M_header;
    return y;
}
#pragma auto_inline(on)

namespace NInventory {
struct CTCInventoryTrade {
    void FinalRefreshSubtitles(NUISystem::CComponent* component, CSubtitleMap& subtitles);
};
void CTCInventoryTrade::FinalRefreshSubtitles(NUISystem::CComponent* component, CSubtitleMap& subtitles)
{
    SSubtitleNode* found = subtitles.find(component);
    NUISystem::CComponent* owner = component;
    CWideString text;
    if (found == subtitles.end()) {
        text = owner->GetChildOfType(6)->GetText();
    } else {
        text = found->Text;
        if (found->Count > 0) {
            text += GetXString();
            text += IntToWideString(found->Count);
        }
    }
    owner->FindChild("UI_ONE_OFF_SUBTITLE")->SetText(text, true, true);
}
}

// ---- stubs and driver ----
void NUISystem::CTextSource::v00() {}
void NUISystem::CTextSource::v01() {}
void NUISystem::CTextSource::v02() {}
void NUISystem::CTextSource::v03() {}
void NUISystem::CTextSource::v04() {}
void NUISystem::CTextSource::v05() {}
void NUISystem::CTextSource::v06() {}
void NUISystem::CTextSource::v07() {}
void NUISystem::CTextSource::v08() {}
void NUISystem::CTextSource::v09() {}
void NUISystem::CTextSource::v10() {}
void NUISystem::CTextSource::v11() {}
void NUISystem::CTextSource::v12() {}
void NUISystem::CTextSource::v13() {}
void NUISystem::CTextSource::v14() {}
void NUISystem::CTextSource::v15() {}
void NUISystem::CTextSource::v16() {}
void NUISystem::CTextSource::v17() {}
void NUISystem::CTextSource::v18() {}
void NUISystem::CTextSource::v19() {}
void NUISystem::CTextSource::v20() {}
void NUISystem::CTextSource::v21() {}
void NUISystem::CTextSource::v22() {}
void NUISystem::CTextSource::v23() {}
void NUISystem::CTextSource::v24() {}
void NUISystem::CTextSource::v25() {}
void NUISystem::CTextSource::v26() {}
void NUISystem::CTextSource::v27() {}
void NUISystem::CTextSource::v28() {}
void NUISystem::CTextSource::v29() {}
void NUISystem::CTextSource::v30() {}
void NUISystem::CTextSource::v31() {}
void NUISystem::CTextSource::v32() {}
void NUISystem::CTextSource::v33() {}
void NUISystem::CTextSource::v34() {}
void NUISystem::CTextSource::v35() {}
void NUISystem::CTextSource::v36() {}
void NUISystem::CTextSource::v37() {}
void NUISystem::CTextSource::v38() {}
void NUISystem::CTextSource::v39() {}
void NUISystem::CTextSource::v40() {}
void NUISystem::CTextSource::v41() {}
void NUISystem::CTextSource::v42() {}
void NUISystem::CTextSource::v43() {}
void NUISystem::CTextSource::v44() {}
void NUISystem::CTextSource::v45() {}
void NUISystem::CTextSource::v46() {}
void NUISystem::CTextSource::v47() {}
void NUISystem::CTextSource::v48() {}
void NUISystem::CTextSource::v49() {}
void NUISystem::CTextSource::v50() {}
void NUISystem::CTextSource::v51() {}
void NUISystem::CTextSource::v52() {}
void NUISystem::CTextSource::v53() {}
void NUISystem::CTextSource::v54() {}
void NUISystem::CTextSource::v55() {}
void NUISystem::CTextSource::v56() {}
void NUISystem::CTextSource::v57() {}
void NUISystem::CTextSource::v58() {}
void NUISystem::CTextSource::v59() {}
void NUISystem::CTextSource::v60() {}
void NUISystem::CTextSource::v61() {}
void NUISystem::CTextSource::v62() {}
void NUISystem::CTextSource::v63() {}
void NUISystem::CTextSource::v64() {}
void NUISystem::CTextSource::v65() {}
void NUISystem::CTextSource::v66() {}
void NUISystem::CTextSource::v67() {}
void NUISystem::CTextSource::v68() {}
void NUISystem::CTextSource::v69() {}
void NUISystem::CTextSource::v70() {}
void NUISystem::CTextSource::v71() {}
void NUISystem::CTextSource::v72() {}
void NUISystem::CTextSource::v73() {}
void NUISystem::CTextSource::v74() {}
void NUISystem::CTextSource::v75() {}
void NUISystem::CTextSource::v76() {}
void NUISystem::CTextSource::v77() {}
void NUISystem::CTextSource::v78() {}
void NUISystem::CTextSource::v79() {}
void NUISystem::CTextSource::v80() {}
void NUISystem::CTextSource::v81() {}
void NUISystem::CTextSource::v82() {}
void NUISystem::CTextSource::v83() {}
void NUISystem::CTextSource::v84() {}
void NUISystem::CTextSource::v85() {}
void NUISystem::CTextSource::v86() {}
void NUISystem::CTextSource::v87() {}
void NUISystem::CTextSource::v88() {}
void NUISystem::CTextSource::v89() {}
void NUISystem::CTextSource::v90() {}
void NUISystem::CTextSource::v91() {}
void NUISystem::CTextSource::v92() {}
void NUISystem::CTextSource::v93() {}
void NUISystem::CTextSource::v94() {}
void NUISystem::CTextSource::v95() {}
void NUISystem::CTextSource::v96() {}
void NUISystem::CTextSource::v97() {}
void NUISystem::CTextSource::v98() {}
void NUISystem::CTextSource::v99() {}
void NUISystem::CTextSource::v100() {}
void NUISystem::CTextSource::v101() {}
void NUISystem::CTextSource::v102() {}
void NUISystem::CTextSource::v103() {}
void NUISystem::CTextSource::v104() {}
void NUISystem::CTextSource::v105() {}
void NUISystem::CTextSource::v106() {}
void NUISystem::CTextSource::v107() {}
void NUISystem::CTextSource::v108() {}
void NUISystem::CTextSource::v109() {}
void NUISystem::CTextSource::v110() {}
void NUISystem::CTextSource::v111() {}
void NUISystem::CTextSource::v112() {}
void NUISystem::CTextSource::v113() {}
void NUISystem::CTextSource::v114() {}
void NUISystem::CTextSource::v115() {}
void NUISystem::CTextSource::v116() {}
void NUISystem::CTextSource::v117() {}
void NUISystem::CTextSource::v118() {}
void NUISystem::CTextSource::v119() {}
void NUISystem::CTextSource::v120() {}
void NUISystem::CTextSource::v121() {}
void NUISystem::CTextSource::v122() {}
void NUISystem::CTextSource::v123() {}
void NUISystem::CTextSource::v124() {}
void NUISystem::CTextSource::v125() {}
void NUISystem::CTextSource::v126() {}
void NUISystem::CTextSource::v127() {}
void NUISystem::CTextSource::v128() {}
void NUISystem::CTextSource::v129() {}
void NUISystem::CTextSource::v130() {}
void NUISystem::CTextSource::v131() {}
void NUISystem::CTextSource::v132() {}
void NUISystem::CTextSource::v133() {}
void NUISystem::CTextSource::v134() {}
void NUISystem::CTextSource::v135() {}
void NUISystem::CTextSource::v136() {}
void NUISystem::CTextSource::v137() {}
void NUISystem::CTextSource::v138() {}
void NUISystem::CTextSource::v139() {}
void NUISystem::CTextSource::v140() {}
void NUISystem::CTextSource::v141() {}
void NUISystem::CComponent::v00() {}
void NUISystem::CComponent::v01() {}
void NUISystem::CComponent::v02() {}
void NUISystem::CComponent::v04() {}
void NUISystem::CComponent::v05() {}
void NUISystem::CComponent::v06() {}
void NUISystem::CComponent::v07() {}
void NUISystem::CComponent::v08() {}
void NUISystem::CComponent::v10() {}
void NUISystem::CComponent::v11() {}
void NUISystem::CComponent::v12() {}
void NUISystem::CComponent::v13() {}
void NUISystem::CComponent::v14() {}
void NUISystem::CComponent::v15() {}
void NUISystem::CComponent::v16() {}
void NUISystem::CComponent::v17() {}
void NUISystem::CComponent::v18() {}
void NUISystem::CComponent::v19() {}
void NUISystem::CComponent::v20() {}
void NUISystem::CComponent::v21() {}
void NUISystem::CComponent::v22() {}
void NUISystem::CComponent::v23() {}
void NUISystem::CComponent::v24() {}
void NUISystem::CComponent::v25() {}
void NUISystem::CComponent::v26() {}
void NUISystem::CComponent::v27() {}
void NUISystem::CComponent::v28() {}
void NUISystem::CComponent::v29() {}
void NUISystem::CComponent::v30() {}
void NUISystem::CComponent::v31() {}
void NUISystem::CComponent::v32() {}
void NUISystem::CComponent::v33() {}
void NUISystem::CComponent::v34() {}
void NUISystem::CComponent::v35() {}
void NUISystem::CComponent::v36() {}
void NUISystem::CComponent::v37() {}
void NUISystem::CComponent::v38() {}
void NUISystem::CComponent::v39() {}
void NUISystem::CComponent::v40() {}
void NUISystem::CComponent::v41() {}
void NUISystem::CComponent::v42() {}
void NUISystem::CComponent::v43() {}
void NUISystem::CComponent::v44() {}
void NUISystem::CComponent::v45() {}
void NUISystem::CComponent::v46() {}
void NUISystem::CComponent::v47() {}
void NUISystem::CComponent::v48() {}
void NUISystem::CComponent::v49() {}
void NUISystem::CComponent::v50() {}
void NUISystem::CComponent::v51() {}
void NUISystem::CComponent::v52() {}
void NUISystem::CComponent::v53() {}
void NUISystem::CComponent::v54() {}
void NUISystem::CComponent::v55() {}
void NUISystem::CComponent::v56() {}
void NUISystem::CComponent::v57() {}
void NUISystem::CComponent::v58() {}
void NUISystem::CComponent::v59() {}
void NUISystem::CComponent::v60() {}
void NUISystem::CComponent::v61() {}
void NUISystem::CComponent::v62() {}
void NUISystem::CComponent::v63() {}
void NUISystem::CComponent::v64() {}
void NUISystem::CComponent::v65() {}
void NUISystem::CComponent::v66() {}
void NUISystem::CComponent::v67() {}
void NUISystem::CComponent::v68() {}
void NUISystem::CComponent::v69() {}
void NUISystem::CComponent::v70() {}
void NUISystem::CComponent::v71() {}
void NUISystem::CComponent::v72() {}
void NUISystem::CComponent::v73() {}
void NUISystem::CComponent::v74() {}
void NUISystem::CComponent::v75() {}
void NUISystem::CComponent::v76() {}
void NUISystem::CComponent::v77() {}
void NUISystem::CComponent::v78() {}
void NUISystem::CComponent::v79() {}
void NUISystem::CComponent::v80() {}
void NUISystem::CComponent::v81() {}
void NUISystem::CComponent::v82() {}
void NUISystem::CComponent::v83() {}
void NUISystem::CComponent::v84() {}
void NUISystem::CComponent::v85() {}
void NUISystem::CComponent::v86() {}
void NUISystem::CComponent::v87() {}
void NUISystem::CComponent::v88() {}
void NUISystem::CComponent::v89() {}
void NUISystem::CComponent::v90() {}
void NUISystem::CComponent::v91() {}
void NUISystem::CComponent::v92() {}
void NUISystem::CComponent::v93() {}
void NUISystem::CComponent::v94() {}
void NUISystem::CComponent::v95() {}
void NUISystem::CComponent::v96() {}
void NUISystem::CComponent::v97() {}
void NUISystem::CComponent::v98() {}
void NUISystem::CComponent::v99() {}
void NUISystem::CComponent::v100() {}
void NUISystem::CComponent::v101() {}
void NUISystem::CComponent::v102() {}
void NUISystem::CComponent::v103() {}
void NUISystem::CComponent::v104() {}
void NUISystem::CComponent::v105() {}
void NUISystem::CComponent::v106() {}
void NUISystem::CComponent::v107() {}
void NUISystem::CComponent::v108() {}
void NUISystem::CComponent::v109() {}
void NUISystem::CComponent::v110() {}
void NUISystem::CComponent::v111() {}
void NUISystem::CComponent::v112() {}
void NUISystem::CComponent::v113() {}
void NUISystem::CComponent::v114() {}
void NUISystem::CComponent::v115() {}
void NUISystem::CComponent::v116() {}
void NUISystem::CComponent::v117() {}
void NUISystem::CComponent::v118() {}
void NUISystem::CComponent::v119() {}
void NUISystem::CComponent::v120() {}
void NUISystem::CComponent::v121() {}
void NUISystem::CComponent::v122() {}
void NUISystem::CComponent::v123() {}
void NUISystem::CComponent::v124() {}
void NUISystem::CComponent::v125() {}
void NUISystem::CComponent::v126() {}
void NUISystem::CComponent::v127() {}
void NUISystem::CComponent::v128() {}
void NUISystem::CComponent::v129() {}
void NUISystem::CComponent::v130() {}
void NUISystem::CComponent::v131() {}
void NUISystem::CComponent::v132() {}
void NUISystem::CComponent::v133() {}
void NUISystem::CComponent::v134() {}
void NUISystem::CComponent::v135() {}
void NUISystem::CComponent::v136() {}
void NUISystem::CComponent::v137() {}
void NUISystem::CComponent::v138() {}
void NUISystem::CComponent::v139() {}
void NUISystem::CComponent::v140() {}
void NUISystem::CComponent::v141() {}
void NUISystem::CComponent::v142() {}
void NUISystem::CComponent::v143() {}
#include <wchar.h>
static CWideString* g_emptyText = 0;
const CWideString& NUISystem::CTextSource::GetText() { return *g_emptyText; }
NUISystem::CComponent* NUISystem::CComponent::FindChild(CCharString) { return 0; }
NUISystem::CTextSource* NUISystem::CComponent::GetChildOfType(long) { return 0; }
void NUISystem::CComponent::SetText(const CWideString&, bool, bool) {}
// strings live in a never-freed pool so the TU's implicit (shallow) CWideString copies stay valid
static wchar_t g_pool[4096]; static int g_poolUsed = 0; static int g_dtors = 0;
static void* wdup(const wchar_t* s) { size_t n = wcslen(s) + 1; wchar_t* p = g_pool + g_poolUsed; g_poolUsed += (int)n; wcscpy(p, s); return p; }
CCharString::CCharString(const char* text, long) { PStringData = (void*)text; }
CCharString::~CCharString() {}
CWideString::CWideString() { PStringData = wdup(L""); }
CWideString::~CWideString() { ++g_dtors; }
CWideString& CWideString::operator=(const CWideString& other) { if (this != &other) PStringData = wdup((const wchar_t*)other.PStringData); return *this; }
const CWideString& CWideString::operator+=(const CWideString& other)
{
    wchar_t buf[256]; wcscpy(buf, (const wchar_t*)PStringData); wcscat(buf, (const wchar_t*)other.PStringData);
    PStringData = wdup(buf); return *this;
}
static CWideString make(const wchar_t* s) { CWideString w; w.PStringData = wdup(s); return w; }
CWideString __fastcall GetXString() { return make(L" x"); }
CWideString __fastcall IntToWideString(long value) { wchar_t buf[16]; swprintf(buf, L"%ld", value); return make(buf); }
struct CText : NUISystem::CTextSource { CWideString t; const CWideString& GetText() { return t; } };
static CText g_text;
static const char* g_found = 0; static wchar_t g_set[64]; static bool g_a = false, g_b = false; static long g_childType = -1;
struct CTarget : NUISystem::CComponent {
    void SetText(const CWideString& text, bool a, bool b) { wcscpy(g_set, (const wchar_t*)text.PStringData); g_a = a; g_b = b; }
};
static CTarget g_target;
struct COwner : NUISystem::CComponent {
    NUISystem::CComponent* FindChild(CCharString name) { g_found = (const char*)name.PStringData; return &g_target; }
    NUISystem::CTextSource* GetChildOfType(long type) { g_childType = type; return &g_text; }
};
int main()
{
    g_text.t = make(L"plain");
    COwner owner;
    unsigned char header[0x20], nodeRaw[0x20]; memset(header, 0, sizeof(header)); memset(nodeRaw, 0, sizeof(nodeRaw));
    CSubtitleMap map; map._M_header = (SSubtitleNode*)header; map._M_node_count = 0; map._M_key_compare = 0;
    NInventory::CTCInventoryTrade trade;
    // not in the map: the typed child's text
    trade.FinalRefreshSubtitles(&owner, map);
    CHECK(g_childType == 6); CHECK(strcmp(g_found, "UI_ONE_OFF_SUBTITLE") == 0); CHECK(wcscmp(g_set, L"plain") == 0); CHECK(g_a && g_b);
    // in the map with a count: "text x3"
    SSubtitleNode* n = (SSubtitleNode*)nodeRaw; n->Component = &owner; n->Text.PStringData = wdup(L"name"); n->Count = 3;
    *(SSubtitleNode**)(header + 4) = n;
    g_childType = -1; trade.FinalRefreshSubtitles(&owner, map); CHECK(g_childType == -1); CHECK(wcscmp(g_set, L"name x3") == 0);
    // count 0: the stored text alone
    n->Count = 0; trade.FinalRefreshSubtitles(&owner, map); CHECK(wcscmp(g_set, L"name") == 0);
    // another component misses
    COwner other; trade.FinalRefreshSubtitles(&other, map); CHECK(g_childType == 6); CHECK(wcscmp(g_set, L"plain") == 0);
    CHECK(g_dtors > 0);
    if (fails == 0) printf("0059d1b1_TEST PASS\n"); else printf("0059d1b1_TEST FAIL\n");
    return fails;
}