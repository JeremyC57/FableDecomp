#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
#define free test_free
#define malloc test_malloc
// Behaviour test for 00706e00: the reconstructed source below (copied verbatim from src/) runs against stub callees.
// CTCHeroAttachableAppearanceModifiers::~CTCHeroAttachableAppearanceModifiers() -- unregister from the owner's graphic appearance (unless the owner is dead), destroy the modifier graphics, modifier lists, hidden material names and primitive handles in reverse order, then the appearance-component base and ~CTCBase
// Leaf: ??1CTCHeroAttachableAppearanceModifiers
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00706E00, length 192 bytes. Called only by the deleting destructor 0x00709970
// (slot 0 of CTCHeroAttachableAppearanceModifiers's vtable 0x01264F0C); the catalog label
// "CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip" is propagated.
// Callees: NGraphicAppearance::CTCGraphicAppearance::UnregisterComponent @ 0x004C0000 (the +0xc subobject), the
//          CModifierGraphic range destructor @ 0x00709350 (__fastcall first/last + tag; label "_Destroy"), the
//          vector<vector<CModifierEntry>> destructor @ 0x00709580, the set<CCharString> erase @ 0x004ABA70 (landed as
//          CRetailTree_erase004aba70), free @ 0x00BFEA14 (x4), CTCBase::~CTCBase @ 0x00686830.
// Layout: as the landed CTCLight destructor 0x00640F80 for the bases (CTCBase +0, CAGraphicAppearanceComponent vptr +0xc,
// vtables 0x01264F0C / 0x01264EF4); Ego_r.pdb members at retail offsets: FrameLastChanged +0x10, the flag bits +0x14,
// PrimHandles +0x18, HiddenMaterialNames +0x24, Modifiers +0x30, Graphics +0x3c (12-byte STLport containers).
// Shape: the Graphics vector's inline destructor destroys its range out of line (the tag temporary in the `push ecx`
// slot) then frees the storage; the set's destructor is the inline erase/reset/free.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
void __cdecl operator delete(void* block);                  // 0x00BFE9BC

class CThing;
namespace NGraphicAppearance {
class CPrimitive;
class CAGraphicAppearanceComponent;
class CTCGraphicAppearance { public:
    void UnregisterComponent(CAGraphicAppearanceComponent* component);   // 0x004C0000
};
class CAGraphicAppearanceComponent { public:                       // vtable 0x0125A170
    virtual void OnAppearanceDraw();
    ~CAGraphicAppearanceComponent() { }
};
}

class CThing { public:
    unsigned char _pad00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;      // +0x64
    unsigned char _pad68[0x29];
    unsigned char Dead : 1;                                 // +0x91 bit 0
};

class CTCBase { public:
    CThing* ParentThing;                                    // +0x04
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
    virtual ~CTCBase();                                     // 0x00686830
};

template<class T> struct CPtrVector {                       // STLport vector<T*> (12 bytes, malloc allocator)
    T** _M_start;
    T** _M_finish;
    T** _M_end_of_storage;
    ~CPtrVector() { if (_M_start) free(_M_start); }
};

struct CNonTrivialDestructorTag { CNonTrivialDestructorTag() { } };

struct CModifierGraphic;
void __fastcall DestroyModifierGraphics_00709350(CModifierGraphic* first, CModifierGraphic* last,
                                                 const CNonTrivialDestructorTag& tag);   // 0x00709350

struct CModifierGraphicVector {                             // vector<CTCHeroAttachableAppearanceModifiers::CModifierGraphic>
    CModifierGraphic* _M_start;
    CModifierGraphic* _M_finish;
    CModifierGraphic* _M_end_of_storage;
    ~CModifierGraphicVector()
    {
        DestroyModifierGraphics_00709350(_M_start, _M_finish, CNonTrivialDestructorTag());
        if (_M_start)
            free(_M_start);
    }
};

struct CModifierEntryVectorVector {                         // vector<vector<CModifierEntry>>
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    ~CModifierEntryVectorVector();                          // 0x00709580
};

struct CRetailTree_erase004aba70 {                          // set<CCharString>
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; };
    node* head;
    unsigned long size;
    unsigned long _comp;
    void Erase(node* root);                                 // 0x004ABA70
    ~CRetailTree_erase004aba70()
    {
        if (size) {
            Erase(head->parent);
            head->left = head;
            head->parent = 0;
            head->right = head;
            size = 0;
        }
        if (head)
            free(head);
    }
};

class CTCHeroAttachableAppearanceModifiers : public CTCBase, public NGraphicAppearance::CAGraphicAppearanceComponent { public:   // vtables 0x01264F0C / 0x01264EF4
    unsigned long FrameLastChanged;                         // +0x10
    bool  Changed : 1;                                      // +0x14
    bool  RecreatePrimitives : 1;
    bool  AppearanceModifiersLoaded : 1;
    CPtrVector<NGraphicAppearance::CPrimitive> PrimHandles; // +0x18
    CRetailTree_erase004aba70  HiddenMaterialNames;         // +0x24
    CModifierEntryVectorVector Modifiers;                   // +0x30
    CModifierGraphicVector     Graphics;                    // +0x3c
    virtual ~CTCHeroAttachableAppearanceModifiers();
    virtual void OnAppearanceDraw();
};

CTCHeroAttachableAppearanceModifiers::~CTCHeroAttachableAppearanceModifiers()
{
    if (!ParentThing->Dead)
        ParentThing->PTCGraphicAppearance->UnregisterComponent(this);
}

// ---- stubs and driver ----
static char g_log[1024];
static char g_num[64];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }
static bool g_armed = false;
static void* g_freed[32];
static int g_nfreed = 0;
extern "C" void test_free(void* p) { if (g_armed && g_nfreed < 32) g_freed[g_nfreed++] = p; }
static bool Freed(void* p) { for (int i = 0; i < g_nfreed; ++i) if (g_freed[i] == p) return true; return false; }
static long g_dummy[64];
#define FAKE(i) ((void*)&g_dummy[i])
static NGraphicAppearance::CAGraphicAppearanceComponent* g_unregistered = 0;
void NGraphicAppearance::CTCGraphicAppearance::UnregisterComponent(CAGraphicAppearanceComponent* c) { g_unregistered = c; Log("Unreg"); }
CTCBase::~CTCBase() { Log("TCBase"); }
void NGraphicAppearance::CAGraphicAppearanceComponent::OnAppearanceDraw() {}
void CTCHeroAttachableAppearanceModifiers::OnAppearanceDraw() {}
void __fastcall DestroyModifierGraphics_00709350(CModifierGraphic* first, CModifierGraphic* last, const CNonTrivialDestructorTag&)
{ std::sprintf(g_num, "G%d", (int)((char*)last - (char*)first)); Log(g_num); }
CModifierEntryVectorVector::~CModifierEntryVectorVector() { Log("Modifiers"); }
void CRetailTree_erase004aba70::Erase(node*) { Log("EraseNames"); }
int main()
{
    static CThing owner;
    static NGraphicAppearance::CTCGraphicAppearance appearance;
    owner.PTCGraphicAppearance = &appearance;
    owner.Dead = 0;
    static CRetailTree_erase004aba70::node namesHead;
    static unsigned char buf[sizeof(CTCHeroAttachableAppearanceModifiers)];
    CTCHeroAttachableAppearanceModifiers* m = (CTCHeroAttachableAppearanceModifiers*)buf;
    m->ParentThing = &owner;
    m->PrimHandles._M_start = (NGraphicAppearance::CPrimitive**)FAKE(1);
    m->HiddenMaterialNames.head = &namesHead; m->HiddenMaterialNames.size = 1;
    m->Graphics._M_start = (CModifierGraphic*)FAKE(10); m->Graphics._M_finish = (CModifierGraphic*)FAKE(12);
    g_armed = true;
    m->CTCHeroAttachableAppearanceModifiers::~CTCHeroAttachableAppearanceModifiers();
    g_armed = false;
    char expect[64]; std::sprintf(expect, "Unreg,G%d,Modifiers,EraseNames,TCBase,", (int)(2 * sizeof(long)));
    bool ok = std::strcmp(g_log, expect) == 0 && g_unregistered == static_cast<NGraphicAppearance::CAGraphicAppearanceComponent*>(m) &&
              g_nfreed == 3 && g_freed[0] == FAKE(10) && g_freed[1] == &namesHead && g_freed[2] == FAKE(1);
    // a dead owner is not asked to unregister
    g_log[0] = 0; owner.Dead = 1; g_nfreed = 0;
    std::memset(buf, 0, sizeof(buf));
    CTCHeroAttachableAppearanceModifiers* m2 = (CTCHeroAttachableAppearanceModifiers*)buf;
    m2->ParentThing = &owner;
    m2->CTCHeroAttachableAppearanceModifiers::~CTCHeroAttachableAppearanceModifiers();
    ok = ok && std::strcmp(g_log, "G0,Modifiers,TCBase,") == 0;
    if (!ok) { std::printf("00706e00_TEST FAIL\n"); return 1; }
    std::printf("00706e00_TEST PASS\n");
    return 0;
}