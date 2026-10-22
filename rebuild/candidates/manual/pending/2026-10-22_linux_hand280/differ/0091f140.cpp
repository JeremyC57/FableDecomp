// CAIStateGroup_TestTestTest::Update() -- debug state group: repeatedly gather the creatures in a 15-unit box around the creature that it has not visited yet, sort them nearest first, pick the nearest, find a wander position just beyond it and walk there, remembering it as visited; stops when nobody is left
// Grade: DIFFER(956v949) -- structure right (inline vector copy ctor and _M_insert_overflow, hoisted Bind2 stores); left: the zero register is re-materialised at the loop head (retail re-zeroes ebx at the end of both push_back arms), found.start lands in ebx not edi, the empty filter tag sits at +2 not +0x1e (frame offsets shift by 4), and the GetNearbyWanderPosition argument pushes schedule differently
// Leaf: ?Update@CAIStateGroup_TestTestTest
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);                          // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                                 // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, size_t count);   // 0x00BFEAE6
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};
struct C3DVector {
    float x, y, z;
    C3DVector() {}
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(x - o.x, y - o.y, z - o.z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(x + o.x, y + o.y, z + o.z); }
    C3DVector operator*(float s) const { return C3DVector(x * s, y * s, z * s); }
    C3DVector& Normalise();                     // 0x00A14440
};
struct C2DVector {
    float x, y;
    C2DVector(const C3DVector& v) : x(v.x), y(v.y) {}
};
struct CThingPhysicsBody {
    unsigned char _pad[0x34];
    float Radius;                               // +0x34
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CThingPhysicsBody* PBody;                   // +0x60
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CTCCreatureNavigation {                  // interface 7
    bool GetNearbyWanderPosition(const C3DVector& centre, float radius, C3DVector* result, bool onMap);   // 0x006CB6E0
};
struct C2DBoxI { long minX, minY, maxX, maxY; };
struct C2DBox {
    float minX, minY, maxX, maxY;
    C2DBox(float x, float y, float radius) { minX = x - radius; minY = y - radius; maxX = x + radius; maxY = y + radius; }
    C2DBoxI ToInteger() const;                  // 0x00A55A30
};
inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    if (last == first)
        return result;
    size_t n = (const char*)last - (const char*)first;
    return (char*)memmove(result, first, n) + n;
}
struct __true_type { __true_type() {} };
inline const size_t& MaxSize(const size_t& a, const size_t& b) { return a < b ? b : a; }
struct CThingVector {                           // STLport vector<CThing*> (12 bytes)
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    static CThing** allocate(size_t n) { return n != 0 ? (CThing**)malloc(n * sizeof(CThing*)) : 0; }
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    CThingVector(const CThingVector& x) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        size_t n = x._M_finish - x._M_start;
        _M_start = allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
        _M_finish = (CThing**)CopyTrivial(x._M_start, x._M_finish, _M_start);
    }
    ~CThingVector() { if (_M_start) free(_M_start); }
    size_t size() const { return _M_finish - _M_start; }
    void _M_insert_overflow(CThing** pos, CThing* const& x, const __true_type&, size_t fill_len, bool atend)
    {
        const size_t old_size = size();
        const size_t len = old_size + MaxSize(old_size, fill_len);
        CThing** new_start = allocate(len);
        CThing** new_finish = (CThing**)CopyTrivial(_M_start, pos, new_start);
        for (size_t i = fill_len; i != 0; --i, ++new_finish)
            *new_finish = x;
        if (!atend)
            new_finish = (CThing**)CopyTrivial(pos, _M_finish, new_finish);
        if (_M_start)
            free(_M_start);
        _M_start = new_start;
        _M_finish = new_finish;
        _M_end_of_storage = new_start + len;
    }
    void push_back(CThing* const& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
struct CThingFilter_IsCreature { CThingFilter_IsCreature() {} };
struct CThingFilter_NotInList {
    CThingVector List;
    CThingFilter_NotInList(const CThingVector& list) : List(list) {}
};
struct CFilter_Bind2 {
    const CThingFilter_IsCreature* A;
    const CThingFilter_NotInList* B;
    CFilter_Bind2(const CThingFilter_IsCreature& a, const CThingFilter_NotInList& b) : A(&a), B(&b) {}
};
struct CThingCompare_Nearest {
    C3DVector Pos;
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) {}
};
void __fastcall SortNearest(CThing** first, CThing** last, CThingCompare_Nearest compare);   // 0x009206A0
struct CThingSearchTools {
    void GetThingsInRoughArea(const C2DBoxI& box, const CFilter_Bind2& filter, CThingVector& out);   // 0x00920200
};
long AreaSink_920200(const void* self, long minX, const void* filter);
#pragma auto_inline(off)
__declspec(noinline) void CThingSearchTools::GetThingsInRoughArea(const C2DBoxI& box, const CFilter_Bind2& filter, CThingVector& out)
{
    long found = AreaSink_920200(this, box.minX, filter.B->List._M_start);
    out._M_finish = out._M_start + found;
}
#pragma auto_inline(on)
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool& moving, long anim, bool b1, bool b2,
        const CThing* target, bool b3, float f);   // 0x0097EC00
};
struct CAIStateGroup_TestTestTest : CAIStateGroupBase {
    CThing* GetCreature() const { return PBrain->PCreature; }
    virtual void Update();
};

void CAIStateGroup_TestTestTest::Update()
{
    CThingVector visited;
    CTCCreatureNavigation* navigation;
    {
        const CThing* creature = GetCreature();
        bool has = ((creature->m_presentBits[7 >> 5] >> (7 & 31)) & 1) != 0;
        if (has) {
            long type = 7;
            const CInterfaceMap* map = &creature->m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || 7 < it->type)
                it = map->last;
            navigation = (CTCCreatureNavigation*)it->component;
        }
    }
    bool done = false;
    do {
        CThingVector found;
        C2DVector pos(GetCreature()->GetPosition());
        PeekThingSearchTools()->GetThingsInRoughArea(C2DBox(pos.x, pos.y, 15.0f).ToInteger(),
            CFilter_Bind2(CThingFilter_IsCreature(), CThingFilter_NotInList(visited)), found);
        CThing** first = found._M_start;
        CThing** last = found._M_finish;
        if (first != last) {
            SortNearest(first, last, CThingCompare_Nearest(GetCreature()->GetPosition()));
            for (CThing** it = first; it != found._M_finish; ++it) {
                CThing* thing = *it;
                GetCreature()->GetPosition();
                thing->GetPosition();
            }
            CThing* nearest = *first;
            float reach = nearest->PBody->Radius * 2.0f;
            CThing* creature = GetCreature();
            C3DVector dir = creature->GetPosition() - nearest->GetPosition();
            dir.Normalise();
            C3DVector offset = dir * reach;
            C3DVector target = offset + nearest->GetPosition();
            navigation->GetNearbyWanderPosition(target, reach * 0.5f, &target, false);
            bool moving;
            AIAction_MoveToPos(1, target, moving, 5, true, true, 0, true, 0.0f);
            visited.push_back(nearest);
        } else {
            done = true;
        }
    } while (!done);
}
