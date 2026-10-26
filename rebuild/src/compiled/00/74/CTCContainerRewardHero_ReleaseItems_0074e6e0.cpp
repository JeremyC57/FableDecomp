// CTCContainerRewardHero::ReleaseItems(float speed) -- pop every reward item out of the container: each item is created as a thing object a little above the container (0.1 up) and launched along a popout vector laid out in rings of up to eight, then every created object is told to ignore its siblings and the item list is cleared
// Leaf: ReleaseItems
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0074E6E0, 384 bytes (`ret 4`).
// Callees: CThing::GetPosition() const @ 0x004C73D0, GeneratePopoutVectorFromVars(long, long, long, float) const @ 0x0074E140
// (C3DVector by hidden pointer), CreateNewObject(long, const C3DVector&, const C3DVector&, const CCharString&) @ 0x0074E560,
// vector<CThingObject*>::_M_insert_overflow(pos, x, __true_type, 1, true) @ 0x0074F900, std::for_each<CThingObject**, functor>
// @ 0x0074FBB0 (the functor returned by hidden pointer; ecx = return slot, edx = first: modelled as __fastcall),
// vector<pair<long, CCharString>>::erase(first, last) @ 0x0074FB20, free @ 0x00BFEA14 (the local vector's storage).
// Globals: 0.1f @ 0x0122EE58.
// Layout: Ego_r.pdb CTCContainerRewardHero (names from the PDB symbols ?ReleaseItems@...@@IAEXM@Z etc.); retail ContainerItems
// (STLport vector<pair<long, CCharString>>, 8-byte elements) at +0x0c as the landed HasReward 0x0074E500.
// Shape: the per-ring count is min(size, 8) written as a preset 8; the local vector is pushed inline (null-tested placement
// store, overflow call with the tag temporary in the dead popout-return slot); the functor's return slot is the dead `speed`
// argument slot.
class CCharString { public: void* m_pData; };
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CThing { public:
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
class CThingObject;
struct SContainerItem {                                 // pair<long, CCharString>
    long        first;
    CCharString second;
};
struct CContainerItemVector {                           // STLport vector<pair<long, CCharString>>
    SContainerItem* _M_start;
    SContainerItem* _M_finish;
    SContainerItem* _M_end_of_storage;
    SContainerItem* begin() { return _M_start; }
    SContainerItem* end() { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
    long size() const { return _M_finish - _M_start; }
    SContainerItem* erase(SContainerItem* first, SContainerItem* last);   // 0x0074FB20
    void clear() { erase(begin(), end()); }
};
extern "C" void __cdecl free(void*);
struct __true_type { __true_type() { } };
struct CThingObjectVector {                             // STLport vector<CThingObject*>
    CThingObject** _M_start;
    CThingObject** _M_finish;
    CThingObject** _M_end_of_storage;
    CThingObjectVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CThingObjectVector() { if (_M_start) free(_M_start); }
    CThingObject** begin() { return _M_start; }
    CThingObject** end() { return _M_finish; }
    void _M_insert_overflow(CThingObject** pos, CThingObject* const& x, const __true_type&,
                            unsigned long fill_len, bool atend);         // 0x0074F900
    void push_back(CThingObject* const& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
CThingObject** GrowThingObjectVector(CThingObjectVector* vector, unsigned long count);   // stand-in: the reallocation
#pragma auto_inline(off)
// Same-TU body of the growth instantiation 0x0074F900: store the value through its const reference into the new storage.
__declspec(noinline) void CThingObjectVector::_M_insert_overflow(CThingObject** pos, CThingObject* const& x, const __true_type&,
                                                                 unsigned long fill_len, bool atend)
{
    CThingObject** p = GrowThingObjectVector(this, fill_len);
    if (p)
        *p = x;
}
#pragma auto_inline(on)
struct CMakeObjectsIgnoreEachOther {                    // the for_each functor: each object's physics ignores the others
    CThingObjectVector* Objects;
    CMakeObjectsIgnoreEachOther(CThingObjectVector* objects) : Objects(objects) { }
    void operator()(CThingObject* object) const;
};
CMakeObjectsIgnoreEachOther __fastcall for_each(CThingObject** first, CThingObject** last,
                                                CMakeObjectsIgnoreEachOther f);   // 0x0074FBB0
class CTCContainerRewardHero { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    CContainerItemVector ContainerItems;                // +0x0c
    C3DVector GeneratePopoutVectorFromVars(long index, long perRing, long ring, float speed) const;   // 0x0074E140
    CThingObject* CreateNewObject(long defIndex, const C3DVector& pos, const C3DVector& velocity,
                                  const CCharString& name);                                       // 0x0074E560
    void ReleaseItems(float speed);
};

void CTCContainerRewardHero::ReleaseItems(float speed)
{
    if (ContainerItems.empty())
        return;
    const C3DVector& position = ParentThing->GetPosition();
    C3DVector pos;
    pos.X = position.X;
    pos.Y = position.Y;
    pos.Z = position.Z + 0.1f;
    long count = ContainerItems.size();
    C3DVector velocity(0.0f, 0.0f, 0.0f);
    long perRing = 8;
    if (count < 8)
        perRing = count;
    long index = 0;
    long ring = 0;
    CThingObjectVector objects;
    for (SContainerItem* it = ContainerItems.begin(); it != ContainerItems.end(); ++it) {
        velocity = GeneratePopoutVectorFromVars(index, perRing, ring, speed);
        if (++index == perRing) {
            index = 0;
            ++ring;
        }
        CThingObject* object = CreateNewObject(it->first, pos, velocity, it->second);
        objects.push_back(object);
    }
    for_each(objects.begin(), objects.end(), CMakeObjectsIgnoreEachOther(&objects));
    ContainerItems.clear();
}