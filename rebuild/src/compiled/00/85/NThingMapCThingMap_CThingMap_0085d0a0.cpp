// NThingMap::CThingMap::~CThingMap() -- thing map dtor: unlink every mapwho node from every cell of every level (zero each node's cell and links, then the cell head), then the member dtors (the cell grid vector of vectors, the level-info vector) and the tail jump to ~CBaseClassNonCopyable
// Leaf: ??1CThingMap@NThingMap@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085D0A0, 213 bytes, tail jump to the base destructor.
// Identity: the one vptr store 0x01276B00 carries RTTI `.?AVCThingMap@NThingMap@@` (rtti_bases: CBaseClassNonCopyable at +0);
//   Ego_r.pdb has `??1CThingMap@NThingMap@@UAE@XZ`. The catalog class "CThingMap" lacks the namespace.
// Callees: free 0x00BFEA14 (STLport allocator::deallocate, inner vectors then both outer blocks), CBaseClassNonCopyable::
//   ~CBaseClassNonCopyable 0x0099A300 (`??1CBaseClassNonCopyable@@MAE@XZ`, settled, tail jump).
// Globals: NThingMap::g_NoMapwhoLevels 0x013BA848 (`?g_NoMapwhoLevels@NThingMap@@3JA`, as the landed Clear 0x0085CEC0).
// Layout (Ego_r.pdb NThingMap::CThingMap with retail 12-byte vectors, as the landed ctor 0x0085D370 and Clear 0x0085CEC0):
//   MapwhoLevelInfo +0x04, Manager +0x10, Cells +0x14 (vector<vector<CThingMapCell> >), Area +0x20, NoThings +0x30.
//   CThingMapCell {PFirstNode}; CMapwhoNode {PMapwho, PNext, PPrevious, PCell} (PDB). No engine header.
// Shape: the level loop is signed against the global (`jl`), the cell loop unsigned against the recomputed size() (`jb`);
//   the node walk zeroes PCell, PNext, PPrevious in that order; then STLport's vector dtors (_Destroy loop + deallocate).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x8
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { public:
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
    _Tp& operator[](unsigned int __n) { return *(begin() + __n); }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

class CBaseClass { public:
    virtual ~CBaseClass();                                  // 0x0099A2E0
};
class CBaseClassNonCopyable : public CBaseClass { public:
protected:
    virtual ~CBaseClassNonCopyable();                       // 0x0099A300
};

class C2DBoxI { public:
    long X0, Y0, X1, Y1;
};
class C2DExtentsI { public:
    long X, Y;
};

namespace NThingMap {
extern long g_NoMapwhoLevels;                               // 0x013BA848
class CThingMapManager;
class CTCMapwho;
class CMapwhoLevelInfo { public:
    long        Size;                                       // +0x00
    C2DExtentsI Extents;                                    // +0x04
    long        ScaleShift;                                 // +0x0c
    long        NoThings;                                   // +0x10
};
class CMapwhoNode;
class CThingMapCell { public:
    CMapwhoNode* PFirstNode;                                // +0x00
};
class CMapwhoNode { public:
    CTCMapwho*     PMapwho;                                 // +0x00
    CMapwhoNode*   PNext;                                   // +0x04
    CMapwhoNode*   PPrevious;                               // +0x08
    CThingMapCell* PCell;                                   // +0x0c
};
class CThingMap : public CBaseClassNonCopyable { public:    // vtable 0x01276B00
    std::vector<CMapwhoLevelInfo>               MapwhoLevelInfo;    // +0x04
    CThingMapManager*                           Manager;            // +0x10
    std::vector<std::vector<CThingMapCell> >    Cells;              // +0x14
    C2DBoxI                                     Area;               // +0x20
    long                                        NoThings;           // +0x30
    virtual ~CThingMap();
};

CThingMap::~CThingMap()
{
    for (long level = 0; level < g_NoMapwhoLevels; ++level) {
        for (unsigned int i = 0; i < Cells[level].size(); ++i) {
            CThingMapCell& cell = Cells[level][i];
            CMapwhoNode* node = cell.PFirstNode;
            while (node) {
                CMapwhoNode* next = node->PNext;
                node->PCell = 0;
                node->PNext = 0;
                node->PPrevious = 0;
                node = next;
            }
            cell.PFirstNode = 0;
        }
    }
}
} // namespace NThingMap