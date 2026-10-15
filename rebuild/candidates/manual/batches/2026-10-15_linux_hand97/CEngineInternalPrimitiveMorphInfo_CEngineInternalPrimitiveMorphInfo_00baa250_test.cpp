// Behaviour test for CEngineInternalPrimitiveMorphInfo's constructor @ 0x00BAA250: empty containers, a self-linked set header.
#include <cstdio>
#include <cstdlib>
struct SRbNode { unsigned char color; SRbNode* parent; SRbNode* left; SRbNode* right; void* value; };
struct CTreeBase { SRbNode* header; CTreeBase() : header(0) { header = (SRbNode*)std::malloc(sizeof(SRbNode)); } };
struct CStringSet : CTreeBase {
    unsigned long count;
    CStringSet() : count(0) { header->color = 0; header->parent = 0; header->left = header; header->right = header; }
    ~CStringSet() { std::free(header); }
};
struct CVec { void* f; void* l; void* e; CVec() : f(0), l(0), e(0) {} };
struct CVectorMap { CVec entries; bool dirty; CVectorMap() : dirty(false) {} };
struct CMorphInfo {
    CVectorMap TextureDecals, CompositeSurfaces;
    CStringSet HiddenMaterialNames;
    CVec HiddenMaterials;
};
int main()
{
    CMorphInfo m;
    SRbNode* h = m.HiddenMaterialNames.header;
    if (m.TextureDecals.entries.f != 0 || m.TextureDecals.dirty || m.CompositeSurfaces.entries.e != 0 || h == 0 ||
        h->color != 0 || h->parent != 0 || h->left != h || h->right != h || m.HiddenMaterialNames.count != 0 || m.HiddenMaterials.f != 0)
    { std::printf("00baa250_TEST FAIL\n"); return 1; }
    std::printf("00baa250_TEST PASS\n");
    return 0;
}
