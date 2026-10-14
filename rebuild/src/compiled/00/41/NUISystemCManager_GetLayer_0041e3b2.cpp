// NUISystem::CManager::GetLayer(long key) -- clamp the key to [-3, 13], look it up in the layer tree at +0x70 (0x0042A879; a hit returns the node's value at +0x14), otherwise return the entry for key 0 (the tree's operator[] @ 0x0042D246)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041E3B2, length 68 bytes.
// Callees: the tree's find @ 0x0042A879 (const long&; the manifest label "BinarySearchTree_FindNode" is generic), its operator[] @ 0x0042D246 (const long&, returns long&). Shape: a size-optimised TU with frame pointers (`push -3 ; pop eax` constants, the clamp stores sharing `mov [ebp+8],eax`), the key slot reused for the 0 key (`and dword ptr [ebp+8],0`), the VC6-style tree node (value at +0x14) and head at +0, `ret 4`. The retail layout (tree at +0x70) differs from engine/CManager.h: modelled locally.
#pragma optimize("s", on)
#pragma optimize("y", off)
struct SLayerNode {                                // VC6 Dinkumware tree node: links, colour, then the pair
    SLayerNode* m_pLeft;                           // +0x00
    SLayerNode* m_pParent;                         // +0x04
    SLayerNode* m_pRight;                          // +0x08
    long        m_colour;                          // +0x0c
    long        m_key;                             // +0x10
    long        m_value;                           // +0x14
};
struct CLayerTree {
    SLayerNode* m_pHead;                           // +0x00 (the sentinel: end())
    SLayerNode* Find(const long& key);             // 0x0042A879
    long& operator[](const long& key);             // 0x0042D246
};

namespace NUISystem {
struct CManager {
    unsigned char _pad[0x70];
    CLayerTree    m_layers;                        // +0x70
    long GetLayer(long key);
};
}

long NUISystem::CManager::GetLayer(long key)
{
    if (key < -3)
        key = -3;
    else if (key > 13)
        key = 13;
    SLayerNode* node = m_layers.Find(key);
    if (node != m_layers.m_pHead)
        return node->m_value;
    key = 0;
    return m_layers[key];
}