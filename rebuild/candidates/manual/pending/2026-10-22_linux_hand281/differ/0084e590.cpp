// Grade: DIFFER(593v593) -- NCameraModes::CBase::UpdateTrackMeleeOpponents: same length; four local spots: begin/end of the thing vector loaded via eax/ecx then copied (retail straight into edi/ebp), the GetCentre vtable fetch not hoisted above the Max-Min subtraction (retail eax then mov ecx,eax), the scheduler-commuted X-sum fadd, and the vector free through eax (retail edi); filter order fixed by AdvB3 (enemies declared first)
// Leaf: UpdateTrackMeleeOpponents
extern "C" void __cdecl free(void*);
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    void Normalise();                                   // 0x00A14440
    float Dot(const C3DVector& o) const { return Z * o.Z + Y * o.Y + X * o.X; }
};
struct C2DCoordI {
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
    C2DCoordI(const C3DVector& v) : X((long)v.X), Y((long)v.Y) { }
};
struct CRectI {
    long Left, Top, Right, Bottom;
    CRectI(long l, long t, long r, long b) : Left(l), Top(t), Right(r), Bottom(b) { }
};
struct CTCGraphicAppearance {
    C3DVector GetCameraFocalPos(float scale);           // 0x004C12D0
};
struct CBoundingObject {
    unsigned char _pad04[8];
    C3DVector Max;                                      // +0x0c
    C3DVector Min;                                      // +0x18
    virtual void b00();
    virtual void b01();
    virtual void b02();
    virtual void b03();
    virtual void b04();
    virtual void b05();
    virtual void b06();
    virtual void b07();
    virtual void b08();
    virtual void b09();
    virtual void b10();
    virtual void b11();
    virtual void b12();
    virtual void b13();
    virtual void b14();
    virtual void b15();
    virtual void b16();
    virtual void b17();
    virtual void b18();
    virtual void b19();
    virtual void b20();
    virtual void b21();
    virtual void b22();
    virtual void b23();
    virtual void b24();
    virtual void b25();
    virtual void b26();
    virtual void b27();
    virtual void b28();
    virtual void b29();
    virtual void b30();
    virtual void b31();
    virtual void b32();
    virtual void b33();
    virtual void b34();
    virtual void b35();
    virtual void b36();
    virtual void b37();
    virtual void b38();
    virtual void b39();
    virtual void b40();
    virtual void b41();
    virtual void b42();
    virtual void b43();
    virtual void b44();
    virtual void b45();
    virtual void b46();
    virtual void b47();
    virtual void b48();
    virtual void b49();
    virtual void b50();
    virtual void b51();
    virtual void b52();
    virtual void b53();
    virtual void b54();
    virtual void b55();
    virtual void b56();
    virtual void b57();
    virtual void b58();
    virtual void b59();
    virtual void b60();
    virtual void b61();
    virtual void b62();
    virtual void b63();
    virtual void b64();
    virtual void b65();
    virtual void b66();
    virtual void b67();
    virtual void b68();
    virtual void b69();
    virtual C3DVector GetCentre();                      // slot 70 (+0x118)
};
struct CThing {
    unsigned char _pad04[0x5c];
    CBoundingObject*      PBounds;                      // +0x60
    CTCGraphicAppearance* PGraphicAppearance;           // +0x64
    unsigned char         _pad68[4];
    unsigned char         m_meshFlags;                  // +0x6c bit 3
    unsigned char         _pad6d[0x91 - 0x6d];
    unsigned char         m_lifeFlags;                  // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;               // 0x004C73D0
    virtual void t00();
    virtual void t01();
    virtual void t02();
    virtual void t03();
    virtual void t04();
    virtual void t05();
    virtual void t06();
    virtual void t07();
    virtual void t08();
    virtual void t09();
    virtual void t10();
    virtual void t11();
    virtual void t12();
    virtual void t13();
    virtual void t14();
    virtual void t15();
    virtual void t16();
    virtual void t17();
    virtual void t18();
    virtual void t19();
    virtual void t20();
    virtual void t21();
    virtual void t22();
    virtual void t23();
    virtual void t24();
    virtual void t25();
    virtual void t26();
    virtual void t27();
    virtual void t28();
    virtual void t29();
    virtual void t30();
    virtual void t31();
    virtual void t32();
    virtual void t33();
    virtual void t34();
    virtual void t35();
    virtual void t36();
    virtual void t37();
    virtual void t38();
    virtual void t39();
    virtual void t40();
    virtual void t41();
    virtual void t42();
    virtual void t43();
    virtual void t44();
    virtual void t45();
    virtual void t46();
    virtual void t47();
    virtual void t48();
    virtual void t49();
    virtual void t50();
    virtual void t51();
    virtual void t52();
    virtual void t53();
    virtual void t54();
    virtual void t55();
    virtual void t56();
    virtual void t57();
    virtual void t58();
    virtual void t59();
    virtual void t60();
    virtual void t61();
    virtual void t62();
    virtual void t63();
    virtual void t64();
    virtual void t65();
    virtual void t66();
    virtual void t67();
    virtual void t68();
    virtual void t69();
    virtual void t70();
    virtual void t71();
    virtual void t72();
    virtual void t73();
    virtual void t74();
    virtual void t75();
    virtual void t76();
    virtual void t77();
    virtual void t78();
    virtual void t79();
    virtual void t80();
    virtual float GetMeleeRange(const C3DVector& position);   // slot 81 (+0x144)
};
struct CThingVector {
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CThingVector() { if (_M_start) free(_M_start); }
};
struct CFilterNotThing { CThing* Thing; CFilterNotThing(CThing* t) : Thing(t) { } };
struct CFilterEnemyOf { CThing* Thing; CFilterEnemyOf(CThing* t) : Thing(t) { } };
struct CFilterAndNotEnemy {
    const CFilterNotThing* First; const CFilterEnemyOf* Second;
    CFilterAndNotEnemy(const CFilterNotThing& a, const CFilterEnemyOf& b) : First(&a), Second(&b) { }
};
struct CThingSearch {
    void GetThingsInRect(const CRectI& rect, const CFilterAndNotEnemy& filter, CThingVector& out);   // 0x0084F850
};
struct CWorldMap {
    unsigned char _pad[0x20];
    CThingSearch* Search;                               // +0x20
};
struct CBaseIntelligentPointer {
    CThing* GetPItem();                                 // 0x00A01B50
};
struct CCameraManager {
    unsigned char           _pad00[0x74];
    CWorldMap*              WorldMap;                   // +0x74
    unsigned char           _pad78[0x100 - 0x78];
    CBaseIntelligentPointer m_observedThing;            // +0x100
    CThing* GetObservedThing() { return m_observedThing.GetPItem(); }
};
struct SAxisPoint { C3DVector Position; float Weight; };
struct CAxisPointList {
    C3DVector   Origin;                                 // +0x00
    C3DVector   Axis;                                   // +0x0c
    SAxisPoint* m_pFirst;                               // +0x18
    SAxisPoint* m_pLast;                                // +0x1c
    SAxisPoint* m_pEnd;                                 // +0x20
    long        m_count;                                // +0x24
    C3DVector   Centre;                                 // +0x28
    CAxisPointList(const C3DVector& origin, const C3DVector& axis);   // 0x0084F4D0
    ~CAxisPointList() { if (m_pFirst) free(m_pFirst); }
    void Add(const C3DVector& point, float weight);     // 0x0084E390
    void Compute();                                     // 0x0084D0E0
};
namespace NCameraModes {
class CBase {
public:
    virtual void v00();
    unsigned char   _pad04[0x3c];
    CCameraManager* CameraManager;                      // +0x40
    void UpdateTrackMeleeOpponents(const C3DVector& origin, float unused);
};
}

void NCameraModes::CBase::UpdateTrackMeleeOpponents(const C3DVector& origin, float)
{
    CThing* hero = CameraManager->GetObservedThing();
    if (hero == 0 || (hero->m_lifeFlags & 1) || !(hero->m_meshFlags & 8))
        return;
    C3DVector position = hero->GetPosition();
    CThingVector things;
    {
        CFilterEnemyOf enemies(hero);
        CFilterNotThing notHero(hero);
        CFilterAndNotEnemy filter(notHero, enemies);
        C2DCoordI centre((long)position.X, (long)position.Y);
        CRectI rect(centre.X - 20, centre.Y - 20, centre.X + 20, centre.Y + 20);
        CameraManager->WorldMap->Search->GetThingsInRect(rect, filter, things);
    }
    CThing** end = things._M_finish;
    CThing** it = things._M_start;
    if (it != end) {
        CAxisPointList list(origin, C3DVector(0.0f, 0.0f, 1.0f));
        for (; it != end; ++it) {
            CThing* thing = *it;
            if (thing != 0 && !(thing->m_lifeFlags & 1)) {
                C3DVector focal = thing->PGraphicAppearance->GetCameraFocalPos(1.0f);
                list.Add(focal, 1.0f);
            }
        }
        list.Compute();
        C3DVector heroFocal = hero->PGraphicAppearance->GetCameraFocalPos(1.0f);
        C3DVector direction = list.Centre - heroFocal;
        direction.Normalise();
        CBoundingObject* bounds = hero->PBounds;
        float extent = (bounds->Max - bounds->Min).Dot(direction);
        C3DVector centre = hero->GetPosition() + bounds->GetCentre();
        if (!(hero->GetMeleeRange(centre) * -0.5f > extent)) {
            list.Add(heroFocal, 2.5f);
            list.Compute();
        }
    }
}
