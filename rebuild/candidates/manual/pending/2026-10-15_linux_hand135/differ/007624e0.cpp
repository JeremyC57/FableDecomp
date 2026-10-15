// NShape::CShapeManager::CreateShape(EShapeType type, const CPointList& points) -- builds a CBasicShape (type 0) or a CTrack (type 1) owned by this manager and returns it in a counted pointer
// Grade: DIFFER(ours v retail) 167v160 -- (1) register colouring: retail loads `type` first and keeps the zero/block in esi and `this` in edi, ours `mov esi,ecx` first; (2) the return copy: retail reads the local's block once after Reset (esi) and folds add-ref and release into `mov ecx,[esi] ; inc ecx ; mov eax,ecx ; dec eax ; mov [esi],ecx ; mov [esi],eax`, ours does `inc [eax]` then reloads the address-taken local and re-tests it. Tried: member/local-pointer release, `count = count + 1` add-ref, /Oa.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007624E0, length 160 bytes. The manifest's "CEngineLightingManager::
// CreateInternalPrimitive" label is propagated from a byte-similar factory; the callees are the NShape constructors.
// Callees: operator new @ 0x00BFEA1A, NShape::CBasicShape::CBasicShape(manager, points) @ 0x00762440 (0x2C bytes, landed),
// NShape::CTrack::CTrack(manager, points) @ 0x00761F40 (0x28 bytes), CCountedPointer<CShape>::Reset @ 0x007642C0 (landed),
// the count block's `__fastcall` destroy function at +4 and operator delete @ 0x00BFE9BC.
// Shape: the result counted pointer is a local default-constructed to {0, 0} (the block word through the `xor esi,esi`
// register), filled by `Reset(new X(this, points))` inside a two-case `switch` (`sub eax,esi ; je ; dec eax ; jne`;
// the default arm jumps straight to the return with esi = 0), and returned by value through the hidden pointer
// (`ret 0Ch`): the copy constructor's add-ref and the local's release share one null test and keep both count stores.
extern "C" void __cdecl operator_delete_00bfe9bc(void* block);   // 0x00BFE9BC

namespace NShape {

struct CPointList;                                 // the retail 12-byte C3DVector vector
struct CShapeManager;

struct CShape {
    virtual ~CShape();
};

struct CBasicShape : CShape {                      // 0x2C bytes
    unsigned char _body[0x28];
    CBasicShape(CShapeManager* manager, const CPointList& points);   // 0x00762440
};

struct CTrack : CShape {                           // 0x28 bytes
    unsigned char _body[0x24];
    CTrack(CShapeManager* manager, const CPointList& points);        // 0x00761F40
};

struct SCountBlock {                               // CCountedPointer's shared block
    long         m_count;                          // +0x00
    void (__fastcall* m_pfnDestroy)(void*);        // +0x04
    void*        m_pObject;                        // +0x08
};

struct CShapePointer {                             // CCountedPointer<CShape>
    CShape*      m_pObject;                        // +0x00
    SCountBlock* m_pBlock;                         // +0x04
    CShapePointer() : m_pObject(0), m_pBlock(0) { }
    CShapePointer(const CShapePointer& other) : m_pObject(other.m_pObject), m_pBlock(other.m_pBlock)
    {
        if (m_pBlock)
            m_pBlock->m_count = m_pBlock->m_count + 1;
    }
    ~CShapePointer()
    {
        SCountBlock* block = m_pBlock;
        if (block) {
            if (--block->m_count == 0) {
                block->m_pfnDestroy(block->m_pObject);
                operator_delete_00bfe9bc(block);
            }
        }
    }
    void Reset(CShape* object);                    // 0x007642C0
};

enum EShapeType { SHAPE_BASIC = 0, SHAPE_TRACK = 1 };

struct CShapeManager {
    CShapePointer CreateShape(EShapeType type, const CPointList& points);
};

CShapePointer CShapeManager::CreateShape(EShapeType type, const CPointList& points)
{
    CShapePointer shape;
    switch (type) {
    case SHAPE_BASIC:
        shape.Reset(new CBasicShape(this, points));
        break;
    case SHAPE_TRACK:
        shape.Reset(new CTrack(this, points));
        break;
    }
    return shape;
}

} // namespace NShape
