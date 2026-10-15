#include <cstdio>

// Model of CTCFlammable::CTCFlammable(CThing*) @ 0x00780820: null def, no fire points, not burning, every counter and
// the damage caused zeroed, both per-point flags cleared.
struct CThing { int id; };
struct CTCBase { CThing* ParentThing; CTCBase(CThing* t) : ParentThing(t) { } virtual ~CTCBase() { } };
struct CDefPointer { void* Object; CDefPointer() : Object(0) { } };
struct CFirePoint { float x; };
template<class T> struct CRetailVector { T* m_pFirst; T* m_pLast; T* m_pEnd; CRetailVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { } };
struct CTCFlammable : CTCBase {
    CDefPointer PDef; CRetailVector<CFirePoint> FirePoints; bool Burning; long FireDamageAfterFrame; long FireResistance;
    long FireStrength; float DamageCaused; bool DamagePerPoint; bool HasFirePoint;
    CTCFlammable(CThing* thing)
        : CTCBase(thing), Burning(false), FireDamageAfterFrame(0), FireResistance(0), FireStrength(0),
          DamageCaused(0.0f), DamagePerPoint(false), HasFirePoint(false) { }
};

int main()
{
    CThing t; t.id = 5;
    CTCFlammable* f = new CTCFlammable(&t);
    bool ok = f->ParentThing == &t && f->PDef.Object == 0 && f->FirePoints.m_pFirst == 0 && f->FirePoints.m_pLast == 0 &&
              f->FirePoints.m_pEnd == 0 && !f->Burning && f->FireDamageAfterFrame == 0 && f->FireResistance == 0 &&
              f->FireStrength == 0 && f->DamageCaused == 0.0f && !f->DamagePerPoint && !f->HasFirePoint;
    delete f;
    if (!ok) { std::printf("00780820_TEST FAIL\n"); return 1; }
    std::printf("00780820_TEST PASS\n");
    return 0;
}