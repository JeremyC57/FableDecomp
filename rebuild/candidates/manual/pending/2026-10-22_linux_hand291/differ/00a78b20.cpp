// Grade: DIFFER(678v678) -- 28 masked lines: (1) retail builds the by-value CSaveNavigationPositionToFile functor in the argument area (sub esp,0Ch; mov eax,esp; stores) BEFORE loading MapPos through lea ebx,[edi+7]; here the MapPos loads hoist above the sub esp; (2) the NavigationLayers count is computed in eax and stored to its slot before `push 4` in retail (ecx, stored after, here). Tried 16 own variants plus 11 advisor grades (copy ctors on C2DVector rewrite the whole register plan to 643 bytes; named functor, by-value vector ctor, block-scoped/size-reusing count locals do not move either residue)
// Leaf: SaveToFile
// CNavQuadTree::SaveToFile(CAFile& file) const
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14
#pragma pack(push, 1)
struct C2DVector {                                     // 1-byte aligned in retail (CNavQuadTree::MapPos sits at +0x07)
    float X, Y;
};
#pragma pack(pop)
struct CAFile {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void Write(const void* data, long size, long flags);   // slot 4 (+0x10)
    virtual void Seek(long position);                  // slot 5 (+0x14)
    virtual void v06();
    virtual long GetPosition();                        // slot 7 (+0x1c)
};
struct CANavQuadTreeNode;
struct CNodeIndexMap {                                 // STLport map<const CANavQuadTreeNode*, long> (12 bytes)
    struct node {
        unsigned char colour; unsigned char _pad[3];
        node* parent; node* left; node* right;
        const CANavQuadTreeNode* key; long value;
    };
    struct value_type {
        const CANavQuadTreeNode* first;
        long second;
        value_type(const CANavQuadTreeNode* key, long value) : first(key), second(value) {}
    };
    struct insert_result {
        node* it;
        bool  inserted;
        insert_result() {}
    };
    node*         head;
    unsigned long size;
    unsigned char compare;
    __forceinline CNodeIndexMap()
        : head(0)
    {
        head = (node*)malloc(sizeof(node));
        size = 0;
        head->colour = 0;
        head->parent = 0;
        head->left   = head;
        head->right  = head;
    }
    void EraseSubtree(node* root);                      // 0x00A7E300 (_M_erase)
    __forceinline void Erase(node* x)
    {
        while (x != 0) {
            EraseSubtree(x->right);
            node* y = x->left;
            free(x);
            x = y;
        }
    }
    __forceinline ~CNodeIndexMap()
    {
        if (size) {
            Erase(head->parent);
            head->left = head;
            head->parent = 0;
            head->right = head;
            size = 0;
        }
        if (head)
            free(head);
    }
    insert_result insert(const value_type& value);      // 0x00A819B0
};
struct CANavQuadTreeNode {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void AddToIndexMap(long& nextIndex, CNodeIndexMap& indices) const;   // slot 20 (+0x50)
    virtual void SaveToFile(CAFile& file, const CNodeIndexMap& indices, bool recurse, const C2DVector& mapPos) const;   // slot 21 (+0x54)
};
struct CNavBlockedQuadTreeNode;
extern CANavQuadTreeNode g_BlockedNavNode;             // 0x013D2D2C
struct CNavigationPosition {                           // 12 bytes
    C2DVector Position;
    long      Layer;
};
struct CSaveNavigationPositionToFile {
    CAFile*   File;
    C2DVector MapPos;
    CSaveNavigationPositionToFile(CAFile& file, const C2DVector& mapPos) : File(&file), MapPos(mapPos) {}
};
CSaveNavigationPositionToFile __fastcall ForEachSaveNavigationPosition(const CNavigationPosition* first, const CNavigationPosition* last,
                                                                       CSaveNavigationPositionToFile function);   // 0x00A7E440 std::for_each
struct CNavigationLayer {                              // retail 0x28 bytes
    CANavQuadTreeNode** NodesBegin;                    // +0x00 CArray<CANavQuadTreeNode*>
    CANavQuadTreeNode** NodesEnd;                      // +0x04
    unsigned char       _rest[0x28 - 0x08];
};
struct CNavRegionNode { CNavRegionNode* next; CNavRegionNode* prev; };
extern const long g_NavQuadTreeVersion;                // 0x0129DCC8 (8)
extern const long g_NavQuadTreeEndMarker;              // 0x0129DCCC (0xACBDEF12)
struct CNavQuadTree {
    virtual void v00();
    bool              Initialised;                     // +0x04
    bool              Activated;                       // +0x05
    bool              Initialising;                    // +0x06
    C2DVector         MapPos;                          // +0x07 (packed)
    float             MapWidth;                        // +0x10
    float             MapHeight;                       // +0x14
    long              MapIndex;                        // +0x18
    CNavigationLayer* NavigationLayersBegin;           // +0x1c
    CNavigationLayer* NavigationLayersEnd;             // +0x20
    CNavigationLayer* NavigationLayersCapacity;        // +0x24
    unsigned char     _pad28[0x50 - 0x28];
    CNavigationPosition* ActionPointsBegin;            // +0x50
    CNavigationPosition* ActionPointsEnd;              // +0x54
    unsigned char     _pad58[0x74 - 0x58];
    CNavRegionNode*   Regions;                         // +0x74 list<CNavRegion> head node
    long GetNumRegions() const
    {
        long count = 0;
        for (CNavRegionNode* region = Regions->next; region != Regions; region = region->next)
            ++count;
        return count;
    }
    void SaveToFile(CAFile& file) const;
};
inline void WriteLong(CAFile& file, const long& value) { file.Write(&value, 4, 0); }

void CNavQuadTree::SaveToFile(CAFile& file) const
{
    long startPos = file.GetPosition();
    long size = 0;
    file.Write(&size, 4, 0);
    file.Write(&g_NavQuadTreeVersion, 4, 0);
    file.Write(&MapWidth, 4, 0);
    file.Write(&MapHeight, 4, 0);
    WriteLong(file, GetNumRegions());
    WriteLong(file, ActionPointsEnd - ActionPointsBegin);
    ForEachSaveNavigationPosition(ActionPointsBegin, ActionPointsEnd, CSaveNavigationPositionToFile(file, MapPos));
    WriteLong(file, NavigationLayersEnd - NavigationLayersBegin);
    CNodeIndexMap indices;
    indices.insert(CNodeIndexMap::value_type(&g_BlockedNavNode, 0));
    long nextIndex = 1;
    for (const CNavigationLayer* layer = NavigationLayersBegin; layer != NavigationLayersEnd; ++layer)
        for (CANavQuadTreeNode** it = layer->NodesBegin; it != layer->NodesEnd; ++it)
            (*it)->AddToIndexMap(nextIndex, indices);
    WriteLong(file, nextIndex - 1);
    for (const CNavigationLayer* layer2 = NavigationLayersBegin; layer2 != NavigationLayersEnd; ++layer2)
        for (CANavQuadTreeNode** it = layer2->NodesBegin; it != layer2->NodesEnd; ++it)
            (*it)->SaveToFile(file, indices, true, MapPos);
    file.Write(&g_NavQuadTreeEndMarker, 4, 0);
    size = file.GetPosition();
    file.Seek(startPos);
    file.Write(&size, 4, 0);
    file.Seek(size);
}
