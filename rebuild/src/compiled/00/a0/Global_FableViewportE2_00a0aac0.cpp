// CRenderManager::OnViewportChanged() -- empty viewport-change notification hook
// Retail 0x00A0AAC0, 1 byte (`ret`). The only caller is CDisplayManager::SetViewport
// (call site 0x009BF146), which tests its scoped_ptr<CRenderManager> PRenderManager
// (+0x08, Ego_r.pdb layout; the `neg ; sbb ; test edx,&get` is scoped_ptr's
// unspecified_bool_type) and then calls this with ecx = the render manager. The body is
// empty in the retail build (a release-stripped notification), so it is a plain __thiscall
// member with no parameters (no `ret N`). The catalog's "$E2" label is a propagated
// placeholder; the name here is descriptive (no public PDB symbol for this address).
// Leaf: OnViewportChanged

class CRenderManager
{
public:
    void OnViewportChanged();
};

void CRenderManager::OnViewportChanged()
{
}
