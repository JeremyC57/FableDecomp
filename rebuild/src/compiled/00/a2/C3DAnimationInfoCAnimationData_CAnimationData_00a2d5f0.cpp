// C3DAnimationInfo::CAnimationData::CAnimationData(unsigned long size) -- base CResource(), own vtable, an empty retail map, four zeroed members, the size
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2D5F0, length 66 bytes.
// Callees: CResource::CResource @ 0x009FBEC0, malloc @ 0x00BFEA0E (the map's 0x1c-byte head node).
// This class's vtable is 0x0129CE50 (rtti-vtables.tsv: C3DAnimationInfo::CAnimationData).
// The member at +0x1c is the VC6-era Dinkumware red-black tree {head, size} (same shape as the
// landed CRetailTree_node0x1c constructors): head nulled, the head node allocated, size zeroed,
// the node's colour byte and parent cleared and left/right pointed at itself, re-reading `head`
// between the stores. +0x24 is untouched (the tree's comparator/allocator slot). The vtable store
// precedes everything, so the four zeroed pointers at +0x28..+0x34 are a second embedded member
// with an inline constructor (sharing the ebx zero register), and the argument is assigned to
// +0x38 in the body. The malloc argument is popped with `add esp,4` because the result is used.
extern "C" void* __cdecl malloc(unsigned int size);

struct CResource {
    virtual void v00();
    unsigned char _body[0x18];              // +0x04..+0x1c
    CResource();                            // 0x009FBEC0
};

struct CRetailTree_node0x1c {               // VC6-era map/set: {head, size}
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; unsigned char value[0x1c - 16]; };
    node*         head;
    unsigned long size;
    CRetailTree_node0x1c() : head(0)
    {
        head = (node*)malloc(sizeof(node));
        size = 0;
        head->colour = 0;
        head->parent = 0;
        head->left   = head;
        head->right  = head;
    }
};

struct SAnimationBuffers {                  // four raw pointers, nulled by the inline constructor
    void* m_pFrames;                        // +0x00
    void* m_pKeys;                          // +0x04
    void* m_pEvents;                        // +0x08
    void* m_pOwner;                         // +0x0c
    SAnimationBuffers() : m_pFrames(0), m_pKeys(0), m_pEvents(0), m_pOwner(0) { }
};

struct C3DAnimationInfo {
    struct CAnimationData : CResource {     // vtable 0x0129CE50
        CRetailTree_node0x1c m_tracks;      // +0x1c {head, size}
        unsigned char        _pad[4];       // +0x24 (map comparator/allocator slot, untouched)
        SAnimationBuffers    m_buffers;     // +0x28
        unsigned long        m_size;        // +0x38
        virtual void v00();
        CAnimationData(unsigned long size);
    };
};

C3DAnimationInfo::CAnimationData::CAnimationData(unsigned long size)
    : CResource()
    , m_tracks()
    , m_buffers()
{
    m_size = size;
}