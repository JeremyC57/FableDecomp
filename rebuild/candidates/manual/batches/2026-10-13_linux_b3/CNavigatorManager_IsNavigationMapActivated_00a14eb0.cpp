// CNavigatorManager::IsNavigationMapActivated(long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A14EB0, oracle length 41 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  (same list walk as IsNavigationMapLoaded @ 0x00A14E80) ... found: mov eax,[eax+8] ; mov al,[eax+5] ; ret 4
// Finds the loaded CNavQuadTreeMap with the requested index (std::list node: PQuadTree counted
// pointer at +0x08, Index at +0x10) and returns the quad tree's activation flag (byte +0x05 of
// the CNavQuadTree); an unloaded map is not activated. Retail keeps the list at +0x24 (PDB +0x30).
// Layout note: in the shipped binary a std::vector / CArray / map / set / CVectorMap is 4 bytes
// smaller than in Ego_r.pdb (the editor build carries an extra word per container) and a
// std::list is 8 bytes smaller (retail keeps only the head pointer; the PDB list is 0xC bytes).
// A retail std::vector is {first, last, end} with `first` at +0. Members after such containers
// therefore sit earlier than the generated engine header says: 4 bytes per vector/map/set/CArray
// and 8 bytes per list that precede them.

struct CNavQuadTreeCalls {
    unsigned char _pad_0x00[5];
    bool          Activated;             // +0x05 (name inferred)
};

struct CNavQuadTreeMapNode {
    CNavQuadTreeMapNode* Next;           // +0x00
    CNavQuadTreeMapNode* Prev;           // +0x04
    CNavQuadTreeCalls*   PQuadTree_Data; // +0x08
    void*                PQuadTree_Info; // +0x0C
    long                 Index;          // +0x10
};

struct CNavigatorManager_Retail {
    unsigned char        _pad_0x00[0x24];
    CNavQuadTreeMapNode* QuadTrees_Head; // +0x24 (PDB +0x30)
    bool IsNavigationMapActivated(long mapIndex);
};

bool CNavigatorManager_Retail::IsNavigationMapActivated(long mapIndex)
{
    CNavQuadTreeMapNode* head = this->QuadTrees_Head;
    for (CNavQuadTreeMapNode* node = head->Next; node != head; node = node->Next) {
        if (node->Index == mapIndex)
            return node->PQuadTree_Data->Activated;
    }
    return false;
}
