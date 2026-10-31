// CNavQuadTree::UpdateLines(std::vector<std::list<C2DLineF> >& cells, const std::vector<C2DLineF>& lines, const C2DBoxI& region) const -- rebuild the per-cell line lists of a region: clear every cell list in the region (building each 8x8 cell's world box), then file every line into each cell whose box contains an end point or that the line crosses
// Grade: DIFFER(1244v1244) 145/428 -- first full reconstruction (was bodiless): same size, same control flow (STLport list clear/push_back inline, landed FloorToLong/CeilToLong inline asm, std::max/min over doubles converted back to float, the 64-bit index range check returning early, C2DBoxF::ContainsPoint x2 then C2DLineF::IntersectsWith); residue = register colouring (retail this in esi spilled to [ebp+3Ch], region in ebx; VC7.1 here gives this edi, region esi) and frame-slot order (boxes vector at [ebp-1Ch] in retail vs [ebp+34h]; the eight hoisted 0.5/1.0 helper constants in other slots)
// Leaf: ?UpdateLines@CNavQuadTree@@ABEXAAV?$vector@V?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@V?$allocator@V?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@2@@std@@ABV?$vector@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@3@ABVC2DBoxI@@@Z
// Retail-asm: 0x00A78359 `fld dword ptr [ebp+64h]` + 0x00A7835F `fistp dword ptr [ebp+30h]` -- the landed CeilToLong/FloorToLong inline-assembly helpers (4 sites); VC7.1 `(long)x` calls __ftol2 (which this function also calls once, for the cell count).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A781A0, 1244 bytes, `ret 0Ch` (ebp-biased frame `lea ebp,[esp-6Ch]`).
// Callees: __ftol2 0x00BFEA70, vector<C2DBoxF>(size_t) 0x00A80360 (manifest "Vector_ConstructInitialized_C2DLineF"; unproven
//   instantiation name), free 0x00BFEA14 / malloc 0x00BFEA0E (STLport node allocator), __allmul 0x00BFEB50,
//   C2DLineF::IntersectsWith(const C2DBoxF&) const 0x00A57BD0. C2DBoxF::Initialise / ContainsPoint are FableWin.pdb inline spellings.
// Flags: /O2 /Oy /W3
#include <stdlib.h>
#define CNavQuadTree CNavQuadTreeLayout
#include "engine/CNavQuadTree.h"
#undef CNavQuadTree

class C2DVector { public:
    float X, Y;
};
class C2DBoxI { public:
    long TLX, TLY, BRX, BRY;
};
class C2DBoxF { public:
    float TLX, TLY, BRX, BRY;
    void Initialise(float tlx, float tly, float brx, float bry) { TLX = tlx; TLY = tly; BRX = brx; BRY = bry; }
    bool ContainsPoint(const C2DVector& p) const { return p.X >= TLX && p.X < BRX && p.Y >= TLY && p.Y < BRY; }
};
class C2DLineF { public:
    C2DVector Point1, Point2;
    bool IntersectsWith(const C2DBoxF& box) const;          // 0x00A57BD0
};

namespace std {
template <class T> inline const T& max(const T& a, const T& b) { return a < b ? b : a; }
template <class T> inline const T& min(const T& a, const T& b) { return b < a ? b : a; }
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector { public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    explicit vector(unsigned long n);                       // 0x00A80360
    ~vector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long i) { return _M_start[i]; }
    const T& operator[](unsigned long i) const { return _M_start[i]; }
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
};
template <class T> struct _List_node { _List_node* _M_next; _List_node* _M_prev; T _M_data; };
template <class T, class A = allocator<T> > class list { public:
    _List_node<T>* _M_node;
    void clear()
    {
        _List_node<T>* cur = _M_node->_M_next;
        while (cur != _M_node) {
            _List_node<T>* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    void push_back(const T& x)
    {
        _List_node<T>* pos = _M_node;
        _List_node<T>* tmp = (_List_node<T>*)malloc(sizeof(_List_node<T>));
        T* p = &tmp->_M_data;
        if (p)
            *p = x;
        _List_node<T>* prev = pos->_M_prev;
        tmp->_M_next = pos;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        pos->_M_prev = tmp;
    }
};
}

static __forceinline long FloorToLong(float value)
{
    long  result;
    float one;
    float half;
    float next;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fsub  half
        fistp result
        fild  result
        fadd  one
        fstp  next
        mov   eax, value
        cmp   eax, next
        jne   done
        inc   result
    done:
    }
    return result;
}
static __forceinline long CeilToLong(float value)
{
    long  result;
    float one;
    float half;
    float back;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fadd  half
        fistp result
        fild  result
        fsub  one
        fstp  back
        mov   eax, value
        cmp   eax, back
        jne   done
        dec   result
    done:
    }
    return result;
}

class CNavQuadTree : public CNavQuadTreeLayout { public:
    const C2DVector& GetMapPos() const { return *(const C2DVector*)MapPos; }
    void UpdateLines(std::vector<std::list<C2DLineF> >& cells, const std::vector<C2DLineF>& lines, const C2DBoxI& region) const;
};

void CNavQuadTree::UpdateLines(std::vector<std::list<C2DLineF> >& cells, const std::vector<C2DLineF>& lines, const C2DBoxI& region) const
{
    long cellsWide = (long)(MapWidth * 0.125f);
    std::vector<C2DBoxF> boxes((region.BRX - region.TLX) * (region.BRY - region.TLY));
    for (unsigned long x = region.TLX; x < (unsigned long)region.BRX; ++x) {
        for (unsigned long y = region.TLY; y < (unsigned long)region.BRY; ++y) {
            float fx = (float)x * 8.0f + GetMapPos().X;
            float fy = (float)y * 8.0f + GetMapPos().Y;
            boxes[(y - region.TLY) * (region.BRX - region.TLX) - region.TLX + x].Initialise(fx, fy, fx + 8.0f, fy + 8.0f);
            cells[y * cellsWide + x].clear();
        }
    }
    for (const C2DLineF* line = lines.begin(); line != lines.end(); ++line) {
        long maxX = CeilToLong(((float)std::max<double>(line->Point1.X, line->Point2.X) - GetMapPos().X) * 0.125f);
        if (maxX > region.BRX)
            maxX = region.BRX;
        long maxY = CeilToLong(((float)std::max<double>(line->Point1.Y, line->Point2.Y) - GetMapPos().Y) * 0.125f);
        if (maxY > region.BRY)
            maxY = region.BRY;
        long minX = FloorToLong(((float)std::min<double>(line->Point1.X, line->Point2.X) - GetMapPos().X) * 0.125f);
        if (minX < region.TLX)
            minX = region.TLX;
        long minY = FloorToLong(((float)std::min<double>(line->Point1.Y, line->Point2.Y) - GetMapPos().Y) * 0.125f);
        if (minY < region.TLY)
            minY = region.TLY;
        for (long x = minX; x < maxX; ++x) {
            for (long y = minY; y < maxY; ++y) {
                __int64 wide = ((__int64)y - (__int64)region.TLY) * (__int64)(region.BRX - region.TLX) - (__int64)region.TLX + (__int64)x;
                if (wide < 0 || wide > 0x7fffffff)
                    return;
                long index = (y - region.TLY) * (region.BRX - region.TLX) - region.TLX + x;
                if ((unsigned long)index > boxes.size() || index < 0)
                    continue;
                if (boxes[index].ContainsPoint(line->Point1) || boxes[index].ContainsPoint(line->Point2) || line->IntersectsWith(boxes[index]))
                    cells[y * cellsWide + x].push_back(*line);
            }
        }
    }
}
