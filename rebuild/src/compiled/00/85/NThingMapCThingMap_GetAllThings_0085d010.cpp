// NThingMap::CThingMap::GetAllThings(set<CTCMapwho*>* out) -- inserts every mapwho component held in any cell of any mapwho level into the set
// Leaf: GetAllThings
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085D010, length 138 bytes (`ret 4`, + int3 padding).
// Callees: set<CTCMapwho*>::insert(const value_type&) @ 0x0085E140 (pair<iterator,bool> by hidden pointer; manifest
// "CMemoryAllocatorVariableSize::FindInsertPosition" is a propagated label).
// Globals: the mapwho level count @ 0x013BA848 (as in the landed GetMemoryFootprint 0x0085CFE0).
// Layout: as the landed NThingMap::CThingMap::GetMemoryFootprint: Cells (vector<vector<CThingMapCell>>, retail 12-byte vectors)
// at +0x14; a mapwho node holds its CTCMapwho* at +0 and the next node at +4.
// Shape: three nested loops (level `jl`, cell `jb` with size re-read, node walk); the insert result is an unused
// pair<iterator,bool> temporary; `this` and the level counter live in the frame.
namespace NThingMap {
struct CTCMapwho;
class CMapwhoNode { public:
    CTCMapwho*   PThing;                        // +0x00
    CMapwhoNode* PNext;                         // +0x04
};
class CThingMapCell { public:
    CMapwhoNode* PFirstNode;
};
template<class T> struct CRetailVector {           // STLport vector: {start, finish, end_of_storage}
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
struct CSetIterator { void* _M_node; };
struct CInsertResult {                          // pair<iterator, bool>
    CSetIterator first;
    bool         second;
};
struct CTCMapwhoSet {                           // STLport set<CTCMapwho*>
    CInsertResult insert(CTCMapwho* const& value);   // 0x0085E140
};
extern long g_NoMapwhoLevels;                      // 0x013BA848
class CThingMap { public:
    virtual void v00();
    CRetailVector<long>                          MapwhoLevelInfo;   // +0x04
    void*                                        Manager;           // +0x10
    CRetailVector< CRetailVector<CThingMapCell> > Cells;            // +0x14
    void GetAllThings(CTCMapwhoSet* out);
};

void CThingMap::GetAllThings(CTCMapwhoSet* out)
{
    for (long level = 0; level < g_NoMapwhoLevels; ++level) {
        for (unsigned long i = 0; i < Cells[level].size(); ++i) {
            for (CMapwhoNode* node = Cells[level][i].PFirstNode; node; node = node->PNext)
                out->insert(node->PThing);
        }
    }
}
}