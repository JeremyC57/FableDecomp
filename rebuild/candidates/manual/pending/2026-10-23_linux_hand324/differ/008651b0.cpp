// CTCCreatureHitNotification::SetHitNotification(CThing* attacker) -- record a hit marker: from the creature towards the attacker (flattened, normalised) build two edges of a chevron at the creature's radius (side vector and the def's marker height), time-stamp them in seconds, and push the (edge list, frame) pair onto the notification list
// Grade: DIFFER(685v1161) -- first draft: retail re-derives every edge point through by-value C3DVector temporaries (x87 spill/copy chains, two inline point copies per edge); control (malloc head, insert x2, inline vector push_back with list copy) as retail
// Leaf: SetHitNotification
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008651B0, 1161 bytes, `ret 4`, no EH frame.
// Callees: CThing::GetPosition 0x004C73D0, C3DVector::Normalise 0x00A14440, malloc 0x00BFEA0E, CTCBase::GetWorld 0x00686A70,
// CWorld::GetFrame (static) 0x0049D870, list<SHitEdge>::insert 0x0076FC60, list range insert 0x0076FD40,
// vector<pair<list,long>>::_Insert_overflow 0x008657E0, ~list 0x004DB5BB.
// Layout: ParentThing +0x04, Notifications vector<pair<list<SHitEdge>, long>> +0x0c (8-byte elements, VC6 list = head node only);
// CThing +0x60 the bound info (radius +0x34), +0x70 the def (marker height +0x5c).
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);               // 0x00BFEA0E
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void Normalise();                                        // 0x00A14440
    __forceinline C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    __forceinline C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
inline void* operator new(size_t, void* p) { return p; }
extern long g_FramesPerSecond;                               // 0x01375550
struct SHitEdge {
    C3DVector Start;
    C3DVector End;
    float Time;
};
struct SListNode {
    SListNode* Next;
    SListNode* Prev;
    SHitEdge Value;
};
struct CListIterator { SListNode* Node; };
struct list_tag { list_tag() { } };
struct CHitEdgeList {
    SListNode* Head;
    CHitEdgeList() { Head = (SListNode*)malloc(sizeof(SListNode)); Head->Next = Head; Head->Prev = Head; }
    CHitEdgeList(const CHitEdgeList& other)
    {
        Head = (SListNode*)malloc(sizeof(SListNode));
        Head->Next = Head;
        Head->Prev = Head;
        InsertRange(end(), other.begin(), other.end(), list_tag());
    }
    ~CHitEdgeList();                                         // 0x004DB5BB
    CListIterator begin() const { CListIterator it; it.Node = Head->Next; return it; }
    CListIterator end() const { CListIterator it; it.Node = Head; return it; }
    CListIterator insert(CListIterator where, const SHitEdge& value);   // 0x0076FC60
    void InsertRange(CListIterator where, CListIterator first, CListIterator last, const list_tag& tag);   // 0x0076FD40
    void push_back(const SHitEdge& value) { insert(end(), value); }
};
struct SNotification {
    CHitEdgeList Edges;
    long Frame;
};
struct CNotificationVector {
    SNotification* _M_start;
    SNotification* _M_finish;
    SNotification* _M_end_of_storage;
    void InsertOverflow(SNotification* where, const SNotification& value, const list_tag& tag, unsigned long count, bool atEnd);   // 0x008657E0
    void push_back(const SNotification& value)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                new (_M_finish) SNotification(value);
            ++_M_finish;
        } else
            InsertOverflow(_M_finish, value, list_tag(), 1, true);
    }
};
struct CBoundInfo { unsigned char _pad[0x34]; float Radius; };
struct CThingDef { unsigned char _pad[0x5c]; float MarkerHeight; };
struct CThing {
    unsigned char _pad00[0x60];
    CBoundInfo* m_pBounds;                                   // +0x60
    unsigned char _pad64[0x70 - 0x64];
    CThingDef* m_pDef;                                       // +0x70
    const C3DVector& GetPosition() const;                    // 0x004C73D0
};
struct CWorld { static long GetFrame(); };                  // 0x0049D870
struct CTCCreatureHitNotification {
    virtual void v00();
    CThing* ParentThing;                                     // +0x04
    unsigned char _pad08[0x0c - 0x08];
    CNotificationVector Notifications;                       // +0x0c
    CWorld* GetWorld();                                      // 0x00686A70
    void SetHitNotification(CThing* attacker);
};

void CTCCreatureHitNotification::SetHitNotification(CThing* attacker)
{
    const C3DVector& myPos = ParentThing->GetPosition();
    const C3DVector& otherPos = attacker->GetPosition();
    C3DVector dir;
    dir.Y = otherPos.Y - myPos.Y;
    dir.X = otherPos.X - myPos.X;
    long framesPerSecond = g_FramesPerSecond;
    CBoundInfo* bounds = ParentThing->m_pBounds;
    dir.Z = 0.0f;
    dir.Normalise();
    C3DVector side(dir.Y, -dir.X, 0.0f);
    SNotification entry;
    entry.Frame = GetWorld()->GetFrame();
    float time = (float)entry.Frame / framesPerSecond;
    float radius = bounds->Radius;
    float height = ParentThing->m_pDef->MarkerHeight;
    C3DVector up(0.0f, 0.0f, height);
    SHitEdge edge;
    edge.Start = myPos + dir * radius + side * radius;
    edge.End = edge.Start + up;
    edge.Time = time;
    entry.Edges.push_back(edge);
    edge.Start = myPos + dir * radius - side * radius;
    edge.End = edge.Start + up;
    entry.Edges.push_back(edge);
    Notifications.push_back(entry);
}
