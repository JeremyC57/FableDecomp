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
// Behaviour test for 004c5da0: the reconstructed source below (copied verbatim from src/) runs against stub callees.
// NGraphicAppearance::CTCGraphicAppearance::~CTCGraphicAppearance() -- destroy the members in reverse order (the component list out of line, the primitive list cleared and its header freed, the tint-colour and mesh-effect maps emptied and their headers freed), then the CTCBase destructor as a tail jump
// Leaf: ??1CTCGraphicAppearance
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C5DA0, length 162 bytes. Called only by the deleting destructor 0x004C5D80
// (slot 0 of NGraphicAppearance::CTCGraphicAppearance's vtable 0x0123986C); the catalog label
// "CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip" is propagated.
// Callees: the Components list destructor @ 0x004C3550 (out of line), the Primitives list clear @ 0x004C5690, the two map
//          erase helpers @ 0x004C2950 (TintColours) and @ 0x004C4490 (MeshEffects) (landed as CRetailTree_erase004c2950 /
//          CRetailTree_erase004c4490), free @ 0x00BFEA14 (x3, list and tree headers), ~CTCBase @ 0x00686830 (tail jump).
// Layout: as the landed CTCGraphicAppearance::UnregisterComponent 0x004C0000 (retail 0x5c bytes: 12-byte STLport maps
// at +0x0c / +0x18, one-word lists at +0x38 / +0x3c). The class stores no vtable in its destructor (`novtable`).
// Shape: the map destructors are inline here (out of line elsewhere): erase from the root when non-empty, reset the
// header's links through the re-read header pointer, zero the size, free the header; the zero lives in ebx.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct CThing;

class CTCBase {                                    // engine/CTCBase.h offsets
public:
    virtual ~CTCBase();                            // 0x00686830
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
};

struct CRetailTree_erase004c2950 {                 // map<void*, CRGBColour>
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; };
    node* head;
    unsigned long size;
    unsigned long _comp;
    void Erase(node* root);                        // 0x004C2950
    ~CRetailTree_erase004c2950()
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

struct CRetailTree_erase004c4490 {                 // map<void*, CMeshEffectPriorityEntry>
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; };
    node* head;
    unsigned long size;
    unsigned long _comp;
    void Erase(node* root);                        // 0x004C4490
    ~CRetailTree_erase004c4490()
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

struct CPrimitiveListNode;
struct CPrimitiveListObject {                      // STLport list<CPrimitive>
    CPrimitiveListNode* m_head;
    void clear();                                  // 0x004C5690
    ~CPrimitiveListObject()
    {
        clear();
        if (m_head)
            free(m_head);
    }
};

struct CComponentListNode;
struct CComponentListObject {                      // STLport list<CAGraphicAppearanceComponent*>
    CComponentListNode* m_head;
    ~CComponentListObject();                       // 0x004C3550
};

struct C3DVector { float X, Y, Z; };
struct CTCAnimationBase;

namespace NGraphicAppearance {

class __declspec(novtable) CTCGraphicAppearance : public CTCBase {   // vtable 0x0123986C
public:
    CRetailTree_erase004c4490 MeshEffects;         // +0x0c
    CRetailTree_erase004c2950 TintColours;         // +0x18
    CTCAnimationBase*    PTCAnimation;             // +0x24
    C3DVector            CachedStaticFocalPosOffset;   // +0x28
    float                Scale;                    // +0x34
    CPrimitiveListObject Primitives;               // +0x38
    CComponentListObject Components;               // +0x3c
    unsigned char        _rest[0x1c];              // +0x40 .. +0x5c
    virtual ~CTCGraphicAppearance();
};

CTCGraphicAppearance::~CTCGraphicAppearance()
{
}
}

// ---- stubs and driver ----
#include <new>
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
CTCBase::~CTCBase() { Log("TCBase"); }
void CRetailTree_erase004c2950::Erase(node* root) { std::sprintf(g_num, "E2950:%d", (int)(root == (node*)FAKE(40))); Log(g_num); }
void CRetailTree_erase004c4490::Erase(node* root) { std::sprintf(g_num, "E4490:%d", (int)(root == (node*)FAKE(41))); Log(g_num); }
void CPrimitiveListObject::clear() { Log("PrimClear"); }
CComponentListObject::~CComponentListObject() { Log("CompList"); }
int main()
{
    static CRetailTree_erase004c2950::node tintHead;
    static CRetailTree_erase004c4490::node meshHead;
    static unsigned char buf[sizeof(NGraphicAppearance::CTCGraphicAppearance)];
    NGraphicAppearance::CTCGraphicAppearance* a = (NGraphicAppearance::CTCGraphicAppearance*)buf;
    tintHead.parent = (CRetailTree_erase004c2950::node*)FAKE(40);
    a->TintColours.head = &tintHead; a->TintColours.size = 2;
    meshHead.parent = 0;
    a->MeshEffects.head = &meshHead; a->MeshEffects.size = 0;     // empty map: no erase
    a->Primitives.m_head = (CPrimitiveListNode*)FAKE(1);
    g_armed = true;
    a->NGraphicAppearance::CTCGraphicAppearance::~CTCGraphicAppearance();
    g_armed = false;
    bool ok = std::strcmp(g_log, "CompList,PrimClear,E2950:1,TCBase,") == 0 && g_nfreed == 3 &&
              g_freed[0] == FAKE(1) && g_freed[1] == &tintHead && g_freed[2] == &meshHead &&
              tintHead.left == &tintHead && tintHead.right == &tintHead && tintHead.parent == 0 && a->TintColours.size == 0;
    if (!ok) { std::printf("004c5da0_TEST FAIL\n"); return 1; }
    std::printf("004c5da0_TEST PASS\n");
    return 0;
}
