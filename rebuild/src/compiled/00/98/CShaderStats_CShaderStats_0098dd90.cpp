// CShaderStats::~CShaderStats() -- erase the vertex- and pixel-shader change maps and destroy the two current-shader names in reverse member order, then the CBaseClass destructor (tail jump)
// Leaf: ??1CShaderStats
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098DD90, length 145 bytes.
// Label correction: catalogued as CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip (propagated); the only caller
// is the scalar deleting destructor @ 0x0098DE70, slot 0 of `anonymous namespace'::CShaderStats's vtable 0x0129A078.
// Callees: map<pair<CCharString,CCharString>,CInitialisedValue<unsigned long,0>>::_M_erase @ 0x0098D120 (x2),
// free @ 0x00BFEA14 (x2), CCharString::~CCharString @ 0x0099EAE0 (x2), CBaseClass::~CBaseClass @ 0x0099A2E0 (tail jump).
// Layout: Ego_r.pdb `anonymous namespace'::CShaderStats (PDB 44 bytes, retail 0x24 with 12-byte STLport maps):
// CurrentPixelShaderName +0x04, PixelShaderChanges +0x08, CurrentVertexShaderName +0x14, VertexShaderChanges +0x18.
// Shape: each map's inline destructor (erase the root, reset the head links, zero the count, free the head) with one
// shared zero register (edi); no vtable store (`__declspec(novtable)`); the base destructor is the tail jump.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct CBaseClass {                                // vtable 0x01231710
    virtual void v00();
    ~CBaseClass();                                 // 0x0099A2E0
};
struct CCharString {
    void* m_pData;
    ~CCharString();                                // 0x0099EAE0
};

struct CShaderChangeMap {                          // map<pair<CCharString,CCharString>,CInitialisedValue<unsigned long,0>>
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; };
    node*         head;
    unsigned long size;
    long          compare;
    void Erase(node* root);                        // 0x0098D120
    ~CShaderChangeMap()
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

namespace {
struct __declspec(novtable) CShaderStats : CBaseClass {   // vtable 0x0129A078, retail 0x24 bytes
    CCharString      CurrentPixelShaderName;       // +0x04
    CShaderChangeMap PixelShaderChanges;           // +0x08
    CCharString      CurrentVertexShaderName;      // +0x14
    CShaderChangeMap VertexShaderChanges;          // +0x18
    virtual void v00();
    ~CShaderStats();
};

CShaderStats::~CShaderStats()
{
}
}

void Kill_0098dd90(CShaderStats* stats)            // keeps the anonymous-namespace destructor emitted; not part of the retail body
{
    stats->~CShaderStats();
}