#pragma optimize("s",on)
// Leaf: GetMeshEffect
struct RCObj {
    void (__fastcall **vtbl)(RCObj*);
    int refcount;
};

class CEngineInternalPrimitiveMeshBase { public:
    RCObj* ptr;
    void GetMeshEffect(RCObj** src);
};

void CEngineInternalPrimitiveMeshBase::GetMeshEffect(RCObj** src)
{
    RCObj* cur = ptr;
    if (cur != *src) {
        if (cur) {
            if (--cur->refcount == 0) {
                cur->vtbl[1](cur);
            }
            ptr = 0;
        }
        RCObj* n = *src;
        ptr = n;
        if (n) {
            ++n->refcount;
        }
    }
}