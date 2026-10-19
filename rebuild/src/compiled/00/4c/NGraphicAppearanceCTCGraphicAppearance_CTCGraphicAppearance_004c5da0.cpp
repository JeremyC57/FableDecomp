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