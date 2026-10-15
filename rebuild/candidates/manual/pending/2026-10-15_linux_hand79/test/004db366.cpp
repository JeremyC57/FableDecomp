// Behaviour test for CTCCarriedActionUseRead::CTCCarriedActionUseRead @ 0x004DB366: the owner reaches the
// carried-use base, the readable def starts null and the item starts unread.
#include <cstdio>
#include <new>

struct CThing { int unused; };
struct CCarriedReadableDef { int unused; };

struct CTCCarriedActionUseBase {
    virtual ~CTCCarriedActionUseBase() {}
    CThing* ParentThing;
    CTCCarriedActionUseBase(CThing* owner) : ParentThing(owner) {}
};

template<class T>
struct CDefPointer {
    const T* m_pDef;
    CDefPointer() : m_pDef(0) { }
};

struct CTCCarriedActionUseRead : CTCCarriedActionUseBase {
    CDefPointer<CCarriedReadableDef> PDef;
    bool                             AlreadyRead;
    CTCCarriedActionUseRead(CThing* owner) : CTCCarriedActionUseBase(owner), AlreadyRead(false) {}
};

int main()
{
    CThing owner;
    unsigned char storage[sizeof(CTCCarriedActionUseRead)];
    for (unsigned i = 0; i < sizeof(storage); ++i) storage[i] = 0xCD;
    CTCCarriedActionUseRead* p = new (storage) CTCCarriedActionUseRead(&owner);
    bool ok = p->ParentThing == &owner && p->PDef.m_pDef == 0 && !p->AlreadyRead;
    p->~CTCCarriedActionUseRead();
    if (!ok) { std::printf("004db366_TEST FAIL\n"); return 1; }
    std::printf("004db366_TEST PASS\n");
    return 0;
}
