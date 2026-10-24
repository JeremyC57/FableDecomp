// CEngineInternalPrimitiveAnimatedMesh::~CEngineInternalPrimitiveAnimatedMesh() -- compiler-generated destructor: frees the bone-scaling cache map, destroys the orientation/position/animation interpolators, then the mesh base
// Leaf: ??1CEngineInternalPrimitiveAnimatedMesh@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BD9250, 113 bytes, ends in a tail jump to the base destructor.
// Identity: NOT LTextSymbol::~LTextSymbol (propagated label). It is called by the scalar deleting destructors 0x00BD94C0
// (slot 0 of CEngineInternalPrimitiveAnimatedMesh's vtable 0x012A79B4), 0x00BD96F0 (CEngineInternalPrimitive2DAnimatedMesh,
// 0x012A7A44) and 0x00BD9920 (CEngineInternalPrimitiveAnimatedMeshFromFile, 0x012A7B74) -- the derived classes add no
// destructible members, so their generated destructors fold into this one (/OPT:ICF). It is the class's own.
// Callees: CRetailTree_erase00bd8a10::Erase @ 0x00BD8A10 (map<unsigned long, CArray<C3DVector>>::_M_erase), free @ 0x00BFEA14,
// CBaseClass::~CBaseClass @ 0x0099A2E0 (the two novtable interpolators), C3DAnimationInterpolator::~C3DAnimationInterpolator
// @ 0x004C2E90, CEngineInternalPrimitiveMeshBase::~CEngineInternalPrimitiveMeshBase @ 0x00BEBC50 (tail jump).
// Layout: Ego_r.pdb CEngineInternalPrimitiveAnimatedMesh: base CEngineInternalPrimitiveMeshBase 0x70, Anim +0x70
// (C3DAnimationInterpolator), WorldPos +0x84 (C3DPositionInterpolator), Orientation +0xA0 (C3DOrientationInterpolator),
// Cache +0xD4 (CCache: map BoneScalings, 12-byte STLport map in retail), RenderInfo after it (nothing to destroy).
// engine/CEngineInternalPrimitiveAnimatedMeshFromFile.h is a flattened opaque layout (no members to use); local model.
// Shape: no vtable store -- the destructor is compiler-generated (declared by no class in the chain below the base), emitted
// out of line for the deleting destructor that the inline-constructor factory pulls in; members destroyed in reverse order.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

class CEngineInternalPrimitiveMeshBase {           // 0x70 bytes, polymorphic
public:
    CEngineInternalPrimitiveMeshBase();            // out of line
    virtual ~CEngineInternalPrimitiveMeshBase();   // 0x00BEBC50
    unsigned char _body[0x6C];
};

class CBaseClass {
public:
    CBaseClass() { }
    virtual void v00();
    ~CBaseClass();                                 // 0x0099A2E0
};

class C3DAnimationInterpolator {
public:
    C3DAnimationInterpolator() { }
    ~C3DAnimationInterpolator();                   // 0x004C2E90
    unsigned char _body[0x14];
};

class C3DPositionInterpolator : public CBaseClass {        // 0x1C bytes
public:
    unsigned char _body[0x18];
};

class C3DOrientationInterpolator : public CBaseClass {     // 0x34 bytes
public:
    unsigned char _body[0x30];
};

struct CRetailTree_erase00bd8a10 {                 // STLport map<unsigned long, CArray<C3DVector> >
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; };
    node*         head;                            // +0x00
    unsigned long size;                            // +0x04
    unsigned long _comp;                           // +0x08
    CRetailTree_erase00bd8a10() { }
    void Erase(node* root);                        // 0x00BD8A10
    ~CRetailTree_erase00bd8a10()
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

class CCache {
public:
    CRetailTree_erase00bd8a10 BoneScalings;        // +0x00
};

class CEngineInternalPrimitiveAnimatedMesh : public CEngineInternalPrimitiveMeshBase {
public:
    CEngineInternalPrimitiveAnimatedMesh() { }
    C3DAnimationInterpolator   Anim;               // +0x70
    C3DPositionInterpolator    WorldPos;           // +0x84
    C3DOrientationInterpolator Orientation;        // +0xA0
    CCache                     Cache;              // +0xD4
    unsigned char              RenderInfo[0x48];   // +0xE0 CEngineRenderInfoAnimatedMesh (trivially destroyed)
};

CEngineInternalPrimitiveAnimatedMesh* NewAnimatedMesh()     // pulls the vtable / deleting destructor into the TU
{
    return new CEngineInternalPrimitiveAnimatedMesh;
}