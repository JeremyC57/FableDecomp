// C3DMeshFileLoader::CreatePhysicsFacesSegmentFromPhysicsChunks(CIPhysicsFacesSegmentSource* source) -- builds the counted physics-faces segment for a source through the root chunk reached from the member at +0x98 (virtual slot 6)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9BA40, length 48 bytes.
// Callees: virtual slot 6 of the object at +0x98 (returns a C3DMeshFileRootChunk*), the by-value helper @ 0x00A9AD00
// (manifest "CCountedPointer_Construct": CCountedPointer<const CIPhysicsFacesSegmentBase> CX::MakeSegment(source, root),
// which Resets the pointer @ 0x00A9CF40 and registers it with the root chunk @ 0x00AA87E0).
// Class attribution: manifest label "CGameCameraManager::HasCameraMode" is propagated over five bodies (0x00A9B900..0x00A9BA40,
// members +0x58/+0x88/+0x98); the surrounding module is the C3DMeshFile loader (0x00A9A900 ~C3DMeshFileLoader,
// 0x00A9B640 LoadMeshFile), so the loader is the best-evidenced owner -- treat the class name as provisional.
// Twin of 0x00A9BA10 (member +0x88); the names distinguish the two chunk owners.
// Shape: a class returned by value through the hidden pointer (`mov eax,edi ; ret 8`): the virtual call result is evaluated
// first (rightmost argument), then `push eax ; push source ; push hidden` and the helper's result pointer is returned (NRVO).
struct CIPhysicsFacesSegmentBase;
struct CIPhysicsFacesSegmentSource;
struct SCountBlock;

struct C3DMeshFileRootChunk;

struct C3DMeshFileChunkOwner {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual C3DMeshFileRootChunk* GetRootChunk();    // slot 6
};

template <class T>
struct CCountedPointer {
    T* m_pObject;                                   // +0
    SCountBlock* m_pCount;                          // +4
    CCountedPointer(const CCountedPointer& other);  // out of line (never emitted here: the result is forwarded)
    ~CCountedPointer();
};

struct C3DMeshFileLoader {
    unsigned char _pad[0x98];
    C3DMeshFileChunkOwner* m_pPhysicsChunks;        // +0x98
    CCountedPointer<const CIPhysicsFacesSegmentBase> MakeSegment(CIPhysicsFacesSegmentSource* source, C3DMeshFileRootChunk* root);   // 0x00A9AD00
    CCountedPointer<const CIPhysicsFacesSegmentBase> CreatePhysicsFacesSegmentFromPhysicsChunks(CIPhysicsFacesSegmentSource* source);
};

CCountedPointer<const CIPhysicsFacesSegmentBase> C3DMeshFileLoader::CreatePhysicsFacesSegmentFromPhysicsChunks(CIPhysicsFacesSegmentSource* source)
{
    return MakeSegment(source, m_pPhysicsChunks->GetRootChunk());
}
