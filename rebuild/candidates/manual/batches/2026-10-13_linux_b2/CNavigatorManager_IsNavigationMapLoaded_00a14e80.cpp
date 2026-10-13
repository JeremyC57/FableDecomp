// CNavigatorManager::IsNavigationMapLoaded(long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A14E80, oracle length 37 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov ecx,[ecx+24h] ; mov eax,[ecx] ; cmp eax,ecx ; je F ; mov edx,[esp+4] ; L: cmp [eax+10h],edx ; je T ; mov eax,[eax] ; cmp eax,ecx ; jne L ; F: xor al,al ; ret 4 ; T: mov al,1 ; ret 4
// Walks the std::list<CNavQuadTreeMap> (PDB: CNavQuadTreeMap = {CCountedPointer<CNavQuadTree>
// PQuadTree (8 bytes), long Index}) looking for a loaded quad tree with the requested map index.
// Retail keeps the list head pointer at +0x24 where engine/CNavigatorManager.h (Ego_r.pdb) has the
// list at +0x30: the shipped manager is 0xC bytes smaller before it, hence the retail-offset view.

struct CNavQuadTreeMapNode {
    CNavQuadTreeMapNode* Next;          // +0x00
    CNavQuadTreeMapNode* Prev;          // +0x04
    void*                PQuadTree_Data; // +0x08 CCountedPointer<CNavQuadTree>::Data
    void*                PQuadTree_Info; // +0x0C
    long                 Index;         // +0x10 map index
};

struct CNavigatorManager_Retail {
    unsigned char        _pad_0x00[0x24];
    CNavQuadTreeMapNode* QuadTrees_Head;   // +0x24 list sentinel (PDB +0x30)
    bool IsNavigationMapLoaded(long mapIndex);
};

bool CNavigatorManager_Retail::IsNavigationMapLoaded(long mapIndex)
{
    CNavQuadTreeMapNode* head = this->QuadTrees_Head;
    for (CNavQuadTreeMapNode* node = head->Next; node != head; node = node->Next) {
        if (node->Index == mapIndex)
            return true;
    }
    return false;
}
