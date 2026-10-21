// CEngineLandscapePatch::RelocateData(long, void*, void*, long) -- the memory manager moved one of this patch's blocks: rebase whichever pointer referred to the old address and let the owner fix itself up
// Leaf: ?RelocateData@CEngineLandscapePatch@@EAEXJPAX0J@Z
// Retail: Fable.exe @ 0x00BF3980, 143 bytes, `ret 10h`; Ego_r.pdb same symbol.
// Body: if oldData is FastAccessTextureIDs (+0x0c): rebase it; else if it is WaterPatch (+0x14): rebase it and call
// CWaterPatchMesh::OnPostRelocation (0x00BEAA30, labelled CBankFileAsyncData::ChangeLoadingPriority by folding); else walk
// LayerMeshList (+0x08, CLandscapeLayerMesh::Next +0x3c) for the moved mesh, rebase the link that held it (the list head
// or the previous node's Next) and call CLandscapeLayerMesh::OnPostRelocation (0x00BFDD70).
// Rebase = `if (p) p = p - oldData + newData` (byte arithmetic, null-checked; the order is retail's `sub; add`).
// Layout: Ego_r.pdb CEngineLandscapePatch (0x48): vptr, Map +4, LayerMeshList +8, FastAccessTextureIDs +0xc,
// FastAccessTextureIDCount +0x10, WaterPatch +0x14; CLandscapeLayerMesh Next +0x3c. No shared header exists.
// Shape: /O2 (16-aligned entry).
class CEngineLandscapeMap;
class CWaterPatchMesh {
public:
    void OnPostRelocation();                             // 0x00BEAA30
};
class CLandscapeLayerMesh {
public:
    unsigned char        _pad00[0x3c];
    CLandscapeLayerMesh* Next;                           // +0x3c
    void OnPostRelocation();                             // 0x00BFDD70
};
template <class T> inline void RelocatePointer(T*& p, void* oldData, void* newData)
{
    if (p)
        p = (T*)((char*)p - (char*)oldData + (char*)newData);
}
class CEngineLandscapePatch {
public:
    CEngineLandscapeMap*  Map;                           // +0x04
    CLandscapeLayerMesh*  LayerMeshList;                 // +0x08
    unsigned short*       FastAccessTextureIDs;          // +0x0c
    unsigned short        FastAccessTextureIDCount;      // +0x10
    CWaterPatchMesh*      WaterPatch;                    // +0x14
private:
    virtual void RelocateData(long handle, void* oldData, void* newData, long size);
};

void CEngineLandscapePatch::RelocateData(long handle, void* oldData, void* newData, long size)
{
    if (oldData == FastAccessTextureIDs)
    {
        RelocatePointer(FastAccessTextureIDs, oldData, newData);
        return;
    }
    if (oldData == WaterPatch)
    {
        RelocatePointer(WaterPatch, oldData, newData);
        WaterPatch->OnPostRelocation();
        return;
    }
    CLandscapeLayerMesh* prev = 0;
    for (CLandscapeLayerMesh* mesh = LayerMeshList; mesh; mesh = mesh->Next)
    {
        if (oldData == mesh)
        {
            RelocatePointer(mesh, oldData, newData);
            if (prev)
                prev->Next = mesh;
            else
                LayerMeshList = mesh;
            mesh->OnPostRelocation();
            return;
        }
        prev = mesh;
    }
}