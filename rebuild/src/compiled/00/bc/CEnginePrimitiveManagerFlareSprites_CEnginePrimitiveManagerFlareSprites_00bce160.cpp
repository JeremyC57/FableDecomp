// CEnginePrimitiveManagerFlareSprites::~CEnginePrimitiveManagerFlareSprites() -- releases the flare-test vertex shader, clears and frees the vertex-buffer map, then the CBase destructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BCE160, length 115 bytes.
// Callees: the shader data's virtual slot 0 (scalar deleting destructor, flag 1) when its count (+0x10) drops to zero,
// 0x00BCE4B0 the map tree's `_M_erase(root)`, 0x00BFEA14 free (the header node), 0x0099A300 CBase::~CBase() (tail jump;
// stores the root vtable 0x01231710).
// Vtables: 0x012A6C18 CEnginePrimitiveManagerFlareSprites, 0x0129B3D0 CVertexShader, 0x0129A030 CShaderBase.
// Layout: PDB CEnginePrimitiveManagerFlareSprites (0x28) with retail's 12-byte STLport map (header node, count,
// compare): FlareVertexBuffers +0xc, VertexShaderFlareTest +0x18 (CVertexShader: vptr + counted shader data at +0x1c),
// FadeInSpeed +0x20; the engine header carries the PDB offsets (map 0x10 bytes), so it is not used. The CBase root
// (out-of-line destructor) owns the vptr; CEnginePrimitiveManager in between stores no vtable (`__declspec(novtable)`).
// Shape: members are destroyed in reverse order: the CVertexShader's inline destructor (its vtable, the release of
// its data, then the inline CShaderBase destructor's vtable store), the STLport tree's inline destructor (clear():
// erase from the root and reset the header only when the count is non-zero; then the null-tested free of the
// header node), then the out-of-line base destructor as a tail jump. The shader release goes through a pointer
// local (`dec [ecx+10h] ; jne` on the loaded copy; a member re-read reloads it after the decrement).
// Leaf: ??1CEnginePrimitiveManagerFlareSprites
extern "C" void __cdecl free(void* block);               // 0x00BFEA14

class CBase {                                            // vtable 0x01231710
public:
    virtual ~CBase();                                    // 0x0099A300
};
class __declspec(novtable) CEnginePrimitiveManager : public CBase {
public:
    void* SceneList;                                     // +0x04
    bool  Enable;                                        // +0x08
};

class CShaderData {
public:
    virtual ~CShaderData();                              // slot 0: scalar deleting destructor
    unsigned char _pad04[0xc];
    long          RefCount;                              // +0x10
};
class CShaderBase {                                      // vtable 0x0129A030
public:
    virtual void v00();
    ~CShaderBase() { }
};
class CVertexShader : public CShaderBase {               // vtable 0x0129B3D0
public:
    CShaderData* Data;                                   // +0x04
    virtual void v00();
    ~CVertexShader()
    {
        CShaderData* data = Data;
        if (data) {
            if (--data->RefCount == 0)
                delete data;
        }
    }
};

struct SVertexBufferTreeNode {                           // STLport _Rb_tree_node: colour, parent, left, right, value
    long                   m_colour;
    SVertexBufferTreeNode* m_parent;
    SVertexBufferTreeNode* m_left;
    SVertexBufferTreeNode* m_right;
};
struct CVertexBufferMap {                                // STLport map<unsigned long, CCountedPointer<CVertexBufferWin32>>
    SVertexBufferTreeNode* m_header;                     // +0x00
    unsigned long          m_nodeCount;                  // +0x04
    unsigned char          m_compare;                    // +0x08
    void Erase(SVertexBufferTreeNode* node);             // 0x00BCE4B0
    void clear()
    {
        if (m_nodeCount != 0) {
            Erase(m_header->m_parent);
            m_header->m_left = m_header;
            m_header->m_parent = 0;
            m_header->m_right = m_header;
            m_nodeCount = 0;
        }
    }
    ~CVertexBufferMap()
    {
        clear();
        if (m_header)
            free(m_header);
    }
};

class CEnginePrimitiveManagerFlareSprites : public CEnginePrimitiveManager {   // vtable 0x012A6C18
public:
    CVertexBufferMap FlareVertexBuffers;                 // +0x0c
    CVertexShader    VertexShaderFlareTest;              // +0x18
    float            FadeInSpeed;                        // +0x20
    virtual ~CEnginePrimitiveManagerFlareSprites();
};

CEnginePrimitiveManagerFlareSprites::~CEnginePrimitiveManagerFlareSprites()
{
}