// Grade: DIFFER(705v818) -- first draft: loop byte offsets not strength-reduced into frame words, erase/push_back blocks placed differently, def release null test elided
// CTCTavernGameCardPairs::ShuffleCards() -- size the game to the level's card count, shuffle the pack, pull a pair for every card of the first half to the second half, shuffle the dealt cards and stack the whole pack face down at the pack position
// Leaf: ?ShuffleCards@CTCTavernGameCardPairs
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D71E0, 818 bytes (+ int3 padding), plain `ret`.
#include <stdlib.h>
#include <string.h>
struct CThingObject;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
};
struct CRightHandedSet { float m[6]; };
void __fastcall HideMesh(CThingObject* thing);  // 0x005E9B10
void __fastcall PositionMesh(CThingObject* thing, const C3DVector& position, const CRightHandedSet& rhset, const C3DVector& offset, float a, float b);   // 0x005EABC0
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CCardPairLevel {                         // 0x48 bytes
    unsigned char _pad00[0x3c];
    struct SPair { long a, b; };
    SPair* CardsStart;                          // +0x3c
    SPair* CardsFinish;                         // +0x40
    unsigned char _pad44[0x48 - 0x44];
};
struct CPackSlot { unsigned char _pad[0x44]; };
struct CTavernGameCardBaseDef : CDefVirtual, CRefCounted {
    unsigned char _pad08[0x5c - 0x08];
    float PackCardSpacing;                      // +0x5c
    float PackBaseHeight;                       // +0x60
    unsigned char _pad64[0x6c - 0x64];
    CCardPairLevel* Levels;                     // +0x6c
    unsigned char _pad70[0x78 - 0x70];
    CPackSlot* PackStart;                       // +0x78
    CPackSlot* PackFinish;                      // +0x7c
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {
    CTavernGameCardBaseDef* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    CTavernGameCardBaseDef* operator->() const { return Object; }
};
struct CCard {
    CThingObject* Mesh;                         // +0x00
    unsigned char _pad04[4];
    long  Value;                                // +0x08
    unsigned char _pad0c[4];
    float Scale;                                // +0x10
    unsigned char _pad14[0x25 - 0x14];
    bool  FaceDown;                             // +0x25
    void Move(const C3DVector& position, bool immediately);   // 0x008F0660
};
void __fastcall random_shuffle(CCard** first, CCard** last);   // 0x007D7860
template <class T> struct CVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    void _M_insert_overflow(T* position, const T& x, const bool& trivial, unsigned int n, bool atend);   // 0x007D5D60
    T* insert(T* position, const T& x);         // 0x007D78A0
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        } else {
            bool trivial;
            _M_insert_overflow(_M_finish, x, trivial, 1, true);
        }
    }
    T* erase(T* position)
    {
        T* next = position + 1;
        if (next != _M_finish)
            memmove(position, next, (char*)_M_finish - (char*)next);
        --_M_finish;
        return position;
    }
};
template <class T> inline T* Advance(T* p, long n)
{
    if (n > 0)
        p += n;
    return p;
}
template <class T> inline void RandomShuffle(T* first, T* last)
{
    if (first == last)
        return;
    for (T* i = first + 1; i != last; ++i) {
        T* j = first + rand() % ((i - first) + 1);
        T tmp = *i;
        *i = *j;
        *j = tmp;
    }
}
struct CTCTavernGameCardPairs {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    unsigned char _body04[0xf0 - 0x04];
    CRightHandedSet GamePositionRHSet;          // +0xf0
    C3DVector GamePositionPos;                  // +0x108
    unsigned char _pad114[0x134 - 0x114];
    CVector<CCard*> Pack;                       // +0x134
    unsigned char _pad140[0x2fc - 0x140];
    long NumCards;                              // +0x2fc
    virtual long GetLevel();                    // slot 57 (+0xe4)
    void GetCardDef(CDefPointer* out);          // 0x008F0B20
    C3DVector GetPackPosition();                // 0x008F0B80
    void ShuffleCards();
};

void CTCTavernGameCardPairs::ShuffleCards()
{
    CDefPointer def;
    GetCardDef(&def);
    CCardPairLevel& level = def->Levels[GetLevel()];
    NumCards = level.CardsFinish - level.CardsStart;
    random_shuffle(Pack.begin(), Pack.end());
    for (long i = 0; i < NumCards / 2; ) {
        CCard** it = Advance(Pack.begin(), i);
        CCard** p = Pack.begin();
        for (; p != it; ++p)
            if ((*p)->Value == (*it)->Value)
                break;
        if (p != it) {
            CCard* card = *it;
            Pack.erase(it);
            Pack.push_back(card);
        } else {
            ++i;
        }
    }
    for (long j = NumCards / 2; j < NumCards; ++j) {
        CCard** it = Advance(Pack.begin(), j);
        long value = Pack.begin()[j - NumCards / 2]->Value;
        CCard** p = it;
        while ((*p)->Value != value)
            ++p;
        if (p != it) {
            CCard* card = *p;
            Pack.erase(p);
            Pack.insert(it, card);
        }
    }
    RandomShuffle(Pack.begin(), Advance(Pack.begin(), NumCards));
    for (unsigned long k = 0; k < (unsigned long)(def->PackFinish - def->PackStart); ++k) {
        C3DVector pos(0.0f, 0.0f, (float)(long)(0x35 - k) * def->PackCardSpacing + def->PackBaseHeight);
        pos += GetPackPosition();
        CCard* card = Pack.begin()[k];
        HideMesh(card->Mesh);
        PositionMesh(card->Mesh, GamePositionPos, GamePositionRHSet, pos, 0.5f, 0.5f);
        card->Move(pos, true);
        card->Scale = 0.5f;
        card->FaceDown = true;
    }
}
