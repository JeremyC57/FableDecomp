// Grade: DIFFER(637v610) -- structure right, register allocation and the inlined overflow copy differ: retail keeps the
//   zero in ebx (reusing it for new_start inside the inlined _M_insert_overflow and re-zeroing after), `this` in ebp
//   (spilled once), the child cursor in esi and the collection in edi, and walks the old storage with a src+8 cursor;
//   ours zeroes esi, spills new_start and the collection, and copies with plain cursors (+27 bytes). Without
//   __forceinline on the collection-site push_back VC7.1 calls the overflow out of line (448). Facts for the next
//   attempt: 0x00BE0140 is vector::resize(n) (out of line), 0x00BDFE70 _M_insert_overflow(pos, x, __false_type, 1,
//   true); the static is `sphere_list` (atexit dtor 0x0122ADB0).
// CLocalDetailCacheMap::CQuadTreeElement::CalcBoundingSphereAndFadeDistanceFromChildren() -- gather the non-empty bounding spheres of the four children and of the element's own object collections (tracking the largest fade distance and OR-ing the primitive-type masks), then fit the element's bounding sphere around them (radius 0 when there are none)
// Leaf: CalcBoundingSphereAndFadeDistanceFromChildren
// Retail 0x00BDF100, 610 bytes, __thiscall, plain `ret` (Ego_r.pdb
// ?CalcBoundingSphereAndFadeDistanceFromChildren@CQuadTreeElement@CLocalDetailCacheMap@@QAEXXZ).
// Callees: atexit 0x004012BC (the static `sphere_list`'s dynamic atexit destructor 0x0122ADB0), vector::resize
// 0x00BE0140, vector::_M_insert_overflow 0x00BDFE70 (child spheres; inlined for the collection spheres), malloc
// 0x00BFEA0E, free 0x00BFEA14, C3DBoundingSphere's fit-around-spheres 0x00ADDBE0 (name unconfirmed). 0.0f at 0x0122DEDC.
// Static: `sphere_list` (STLport vector<C3DBoundingSphere>) at 0x0143B978, guard word 0x0143B984 bit 0.
// Layout: PDB CQuadTreeElement (landed dtor 0x00BDDFB0): ObjectCacheGroupCollections +4, Children +8, BoundingSphere
// +0x18, MaxFadeDistance +0x2c, PrimitiveTypeMask +0x30; CObjectCacheGroupCollection Next +4, sphere +8, fade +0x18,
// mask +0x1c.
// Shape: STLport push_back; retail inlined _M_insert_overflow only at the collection site, so the child site calls a
// declared out-of-line copy and the collection site an inline copy of the same body.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

struct __false_type {};
struct C3DBoundingSphere {
    float X, Y, Z, Radius;
    void SetFromSpheres(const C3DBoundingSphere* spheres, unsigned int count);   // 0x00ADDBE0
};
inline const unsigned int& max_size(const unsigned int& a, const unsigned int& b) { return a < b ? b : a; }
struct CSphereVector {                               // STLport vector<C3DBoundingSphere>
    C3DBoundingSphere* _M_start;
    C3DBoundingSphere* _M_finish;
    C3DBoundingSphere* _M_end_of_storage;
    CSphereVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CSphereVector();
    unsigned int size() const { return _M_finish - _M_start; }
    void resize(unsigned int n);                     // 0x00BE0140
    void _M_insert_overflow(C3DBoundingSphere* position, const C3DBoundingSphere& x, const __false_type&,
                            unsigned int fill_len, bool atend);   // 0x00BDFE70
    __forceinline void _M_insert_overflow_inline(C3DBoundingSphere* position, const C3DBoundingSphere& x, const __false_type&,
                                   unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + max_size(old_size, fill_len);
        C3DBoundingSphere* new_start = len ? (C3DBoundingSphere*)malloc(len * sizeof(C3DBoundingSphere)) : 0;
        C3DBoundingSphere* new_finish = new_start;
        for (C3DBoundingSphere* p = _M_start; p != position; ++p, ++new_finish)
            new (new_finish) C3DBoundingSphere(*p);
        new (new_finish) C3DBoundingSphere(x);
        ++new_finish;
        if (_M_start)
            free(_M_start);
        _M_start = new_start;
        _M_finish = new_finish;
        _M_end_of_storage = new_start + len;
    }
    void push_back(const C3DBoundingSphere& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) C3DBoundingSphere(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);
    }
    __forceinline void push_back_inline(const C3DBoundingSphere& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) C3DBoundingSphere(x);
            ++_M_finish;
        } else
            _M_insert_overflow_inline(_M_finish, x, __false_type(), 1UL, true);
    }
};

struct CObjectCacheGroupCollection {
    virtual ~CObjectCacheGroupCollection();
    CObjectCacheGroupCollection* Next;               // +0x04
    C3DBoundingSphere BoundingSphere;                // +0x08
    float MaxFadeDistance;                           // +0x18
    unsigned long PrimitiveTypeMask;                 // +0x1c
};
struct CLocalDetailCacheMap;
struct CMovableResource { virtual ~CMovableResource() {} };
struct CQuadTreeElement : CMovableResource {         // CLocalDetailCacheMap::CQuadTreeElement (PDB, 76 bytes)
    virtual ~CQuadTreeElement();
    CObjectCacheGroupCollection* ObjectCacheGroupCollections;   // +0x04
    CQuadTreeElement*  Children[4];                  // +0x08
    C3DBoundingSphere  BoundingSphere;               // +0x18
    CLocalDetailCacheMap* Map;                       // +0x28
    float              MaxFadeDistance;              // +0x2c
    unsigned long      PrimitiveTypeMask;            // +0x30
    void CalcBoundingSphereAndFadeDistanceFromChildren();
};

void CQuadTreeElement::CalcBoundingSphereAndFadeDistanceFromChildren()
{
    static CSphereVector sphere_list;
    sphere_list.resize(0);
    MaxFadeDistance = 0;
    PrimitiveTypeMask = 0;
    for (int i = 0; i < 4; ++i) {
        if (Children[i] && Children[i]->BoundingSphere.Radius > 0.0f) {
            sphere_list.push_back(Children[i]->BoundingSphere);
            if (MaxFadeDistance < Children[i]->MaxFadeDistance)
                MaxFadeDistance = Children[i]->MaxFadeDistance;
            PrimitiveTypeMask |= Children[i]->PrimitiveTypeMask;
        }
    }
    for (CObjectCacheGroupCollection* collection = ObjectCacheGroupCollections; collection;
         collection = collection->Next) {
        if (collection->BoundingSphere.Radius > 0.0f) {
            if (MaxFadeDistance < collection->MaxFadeDistance)
                MaxFadeDistance = collection->MaxFadeDistance;
            PrimitiveTypeMask |= collection->PrimitiveTypeMask;
            sphere_list.push_back_inline(collection->BoundingSphere);
        }
    }
    if (sphere_list.size() > 0)
        BoundingSphere.SetFromSpheres(sphere_list._M_start, sphere_list.size());
    else
        BoundingSphere.Radius = 0;
}
