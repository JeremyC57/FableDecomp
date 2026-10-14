#include <cstdio>
#include <cstdlib>
struct CResource { virtual void v00() {} unsigned char _body[0x18]; CResource() {} };
struct CRetailTree_node0x1c {
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; unsigned char value[0x1c - 16]; };
    node* head; unsigned long size;
    CRetailTree_node0x1c() : head(0) { head = (node*)std::malloc(sizeof(node)); size = 0; head->colour = 0; head->parent = 0; head->left = head; head->right = head; }
};
struct SAnimationBuffers { void* m_pFrames; void* m_pKeys; void* m_pEvents; void* m_pOwner; SAnimationBuffers() : m_pFrames(0), m_pKeys(0), m_pEvents(0), m_pOwner(0) { } };
struct C3DAnimationInfo { struct CAnimationData : CResource { CRetailTree_node0x1c m_tracks; unsigned char _pad[4]; SAnimationBuffers m_buffers; unsigned long m_size; virtual void v00() {} CAnimationData(unsigned long size); }; };
C3DAnimationInfo::CAnimationData::CAnimationData(unsigned long size) : CResource(), m_tracks(), m_buffers() { m_size = size; }
int main()
{
    C3DAnimationInfo::CAnimationData d(99);
    CRetailTree_node0x1c::node* h = d.m_tracks.head;
    bool ok = h != 0 && d.m_tracks.size == 0 && h->colour == 0 && h->parent == 0 && h->left == h && h->right == h
        && d.m_buffers.m_pFrames == 0 && d.m_buffers.m_pKeys == 0 && d.m_buffers.m_pEvents == 0 && d.m_buffers.m_pOwner == 0 && d.m_size == 99;
    std::free(h);
    if (!ok) { std::printf("00a2d5f0_TEST FAIL\n"); return 1; }
    std::printf("00a2d5f0_TEST PASS\n");
    return 0;
}