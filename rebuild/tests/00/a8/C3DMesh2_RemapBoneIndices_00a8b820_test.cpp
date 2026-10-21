#include <cstdio>
// Behaviour copy of C3DMesh2::RemapBoneIndices (0x00A8B820): STLport-shaped tree find + operator[] remapping.
struct Node { long color; Node* parent; Node* left; Node* right; unsigned long first; unsigned long second; };
struct Map {
    Node header; Node pool[16]; int used;
    Map() : used(0) { header.parent = 0; header.left = header.right = &header; }
    Node* end() { return &header; }
    Node* find(const unsigned long& key) {
        Node* y = &header; Node* x = header.parent;
        while (x) { if (!(x->first < key)) { y = x; x = x->left; } else x = x->right; }
        if (y == end() || key < y->first) return end();
        return y;
    }
    unsigned long& operator[](const unsigned long& key) {
        Node* f = find(key); if (f != end()) return f->second;
        Node* n = &pool[used++]; n->first = key; n->second = 0; n->left = n->right = 0;
        Node** link = &header.parent; Node* parent = &header;
        while (*link) { parent = *link; link = key < (*link)->first ? &(*link)->left : &(*link)->right; }
        *link = n; n->parent = parent; return n->second;
    }
};
struct Prim { int remapped; void RemapBoneIndices(Map&) { ++remapped; } };
struct Bone { unsigned long NameCRC; long Parent; };
struct Helper { unsigned long Key; long BoneIndex; };
struct Mesh {
    unsigned long PrimitiveCount; Prim* Primitives; unsigned long BoneCount; Bone* Bones;
    Helper* Helpers; unsigned long HelperCount; Helper* Dummies; unsigned long DummyCount;
    void RemapBoneIndices(Map& remap) {
        for (unsigned long n = 0; n < PrimitiveCount; ++n) Primitives[n].RemapBoneIndices(remap);
        for (unsigned long b = 0; b < BoneCount; ++b) { Bone& bone = Bones[b];
            if (remap.find(bone.Parent) != remap.end()) bone.Parent = remap[bone.Parent]; else bone.Parent = -1; }
        for (Helper* p = Helpers; p != Helpers + HelperCount; ++p) if (p->BoneIndex != -1) p->BoneIndex = remap[p->BoneIndex];
        for (Helper* d = Dummies; d != Dummies + DummyCount; ++d) if (d->BoneIndex != -1) d->BoneIndex = remap[d->BoneIndex];
    }
};
int main() {
    Map m; m[5] = 1; m[2] = 0; m[9] = 7;
    Prim prims[2] = { {0}, {0} };
    Bone bones[3] = { {0, 5}, {0, 3}, {0, 9} };
    Helper helpers[2] = { {0, 2}, {0, -1} }; Helper dummies[1] = { {0, 9} };
    Mesh mesh = { 2, prims, 3, bones, helpers, 2, dummies, 1 };
    mesh.RemapBoneIndices(m);
    if (prims[0].remapped != 1 || prims[1].remapped != 1 || bones[0].Parent != 1 || bones[1].Parent != -1 || bones[2].Parent != 7
        || helpers[0].BoneIndex != 0 || helpers[1].BoneIndex != -1 || dummies[0].BoneIndex != 7 || m.used != 3) {
        std::printf("00a8b820_TEST FAIL\n"); return 1; }
    std::printf("00a8b820_TEST PASS\n"); return 0;
}