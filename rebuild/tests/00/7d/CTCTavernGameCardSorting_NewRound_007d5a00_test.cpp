// Behaviour test for 007d5a00: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCTavernGameCardSorting::NewRound() -- card-base NewRound, reshuffles, then deals the round: as many cards as the current card-order layout has positions are moved from the pack onto the table list
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
struct CCard;
struct C2DVector { float X, Y; };

struct __true_type { __true_type() { } };
inline void* operator new(size_t, void* where) { return where; }
template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    unsigned int size() const { return end() - begin(); }
    T& operator[](unsigned int n) const { return *(begin() + n); }
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fill_len, bool atend);   // 0x007D5D60
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};

struct CCardPositionsDef {
    unsigned char _defbase[0x3c];               // CDefClassBase (retail 0x3c)
    CRetailVector<C2DVector> Offset;            // +0x3c
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CTavernGameCardBaseDef : CDefVirtual, CRefCounted {
    unsigned char _pad_0x08[0x64];
    CRetailVector<CCardPositionsDef> CardOrders;   // +0x6c
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {
    CTavernGameCardBaseDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    CTavernGameCardBaseDef* operator->() const { return Object; }
};

struct CTCTavernGameCardBase {
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
    virtual void NewRound();                    // slot 32
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
    virtual long GetCardOrder();                // slot 57
    virtual void v58();
    virtual void v59();
    virtual void ShuffleCards();                // slot 60
    unsigned char _pad_0x04[0x124];
    CRetailVector<CCard*> Cards;                // +0x128
    CRetailVector<CCard*> Pack;                 // +0x134
    unsigned char _pad_0x140[0x1bc];
    long          NumCards;                     // +0x2fc
    void GetCardDef(CDefPointer* out);          // 0x008F0B20
};
struct CTCTavernGameCardSorting : CTCTavernGameCardBase {
    virtual void NewRound();
};

void CTCTavernGameCardSorting::NewRound()
{
    CTCTavernGameCardBase::NewRound();
    ShuffleCards();
    CDefPointer def;
    GetCardDef(&def);
    long order = GetCardOrder();
    NumCards = def->CardOrders[order].Offset.size();
    for (long i = 0; i < NumCards; ++i) {
        CCard* card = Pack[i];
        Cards.push_back(card);
    }
}

// ---- stubs and driver ----
void CTCTavernGameCardBase::v00() {}
void CTCTavernGameCardBase::v01() {}
void CTCTavernGameCardBase::v02() {}
void CTCTavernGameCardBase::v03() {}
void CTCTavernGameCardBase::v04() {}
void CTCTavernGameCardBase::v05() {}
void CTCTavernGameCardBase::v06() {}
void CTCTavernGameCardBase::v07() {}
void CTCTavernGameCardBase::v08() {}
void CTCTavernGameCardBase::v09() {}
void CTCTavernGameCardBase::v10() {}
void CTCTavernGameCardBase::v11() {}
void CTCTavernGameCardBase::v12() {}
void CTCTavernGameCardBase::v13() {}
void CTCTavernGameCardBase::v14() {}
void CTCTavernGameCardBase::v15() {}
void CTCTavernGameCardBase::v16() {}
void CTCTavernGameCardBase::v17() {}
void CTCTavernGameCardBase::v18() {}
void CTCTavernGameCardBase::v19() {}
void CTCTavernGameCardBase::v20() {}
void CTCTavernGameCardBase::v21() {}
void CTCTavernGameCardBase::v22() {}
void CTCTavernGameCardBase::v23() {}
void CTCTavernGameCardBase::v24() {}
void CTCTavernGameCardBase::v25() {}
void CTCTavernGameCardBase::v26() {}
void CTCTavernGameCardBase::v27() {}
void CTCTavernGameCardBase::v28() {}
void CTCTavernGameCardBase::v29() {}
void CTCTavernGameCardBase::v30() {}
void CTCTavernGameCardBase::v31() {}
void CTCTavernGameCardBase::v33() {}
void CTCTavernGameCardBase::v34() {}
void CTCTavernGameCardBase::v35() {}
void CTCTavernGameCardBase::v36() {}
void CTCTavernGameCardBase::v37() {}
void CTCTavernGameCardBase::v38() {}
void CTCTavernGameCardBase::v39() {}
void CTCTavernGameCardBase::v40() {}
void CTCTavernGameCardBase::v41() {}
void CTCTavernGameCardBase::v42() {}
void CTCTavernGameCardBase::v43() {}
void CTCTavernGameCardBase::v44() {}
void CTCTavernGameCardBase::v45() {}
void CTCTavernGameCardBase::v46() {}
void CTCTavernGameCardBase::v47() {}
void CTCTavernGameCardBase::v48() {}
void CTCTavernGameCardBase::v49() {}
void CTCTavernGameCardBase::v50() {}
void CTCTavernGameCardBase::v51() {}
void CTCTavernGameCardBase::v52() {}
void CTCTavernGameCardBase::v53() {}
void CTCTavernGameCardBase::v54() {}
void CTCTavernGameCardBase::v55() {}
void CTCTavernGameCardBase::v56() {}
void CTCTavernGameCardBase::v58() {}
void CTCTavernGameCardBase::v59() {}

static char g_log[32]; static int g_n;
static void Log(char c) { if (g_n < 31) g_log[g_n++] = c; g_log[g_n] = 0; }
void CTCTavernGameCardBase::NewRound() { Log('N'); }
void CTCTavernGameCardBase::ShuffleCards() { Log('S'); }
long CTCTavernGameCardBase::GetCardOrder() { Log('O'); return 1; }
void CDefVirtual::v00() {}
static int g_destroyed;
void CDefVirtual::Destroy() { ++g_destroyed; }
static CTavernGameCardBaseDef g_def;
void CTCTavernGameCardBase::GetCardDef(CDefPointer* out) { Log('D'); out->Object = &g_def; ++g_def.m_count; }
static int g_overflows;
template<class T> void CRetailVector<T>::_M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fill_len, bool atend)
{ ++g_overflows; *position = x; _M_finish = position + fill_len; }

int main()
{
    static CTCTavernGameCardSorting game;
    static CCard* pack[5] = { (CCard*)0x10, (CCard*)0x20, (CCard*)0x30, (CCard*)0x40, (CCard*)0x50 };
    static CCard* table[8];
    static C2DVector offsets[3];
    static CCardPositionsDef orders[2];
    orders[1].Offset._M_start = offsets; orders[1].Offset._M_finish = offsets + 3; orders[1].Offset._M_end_of_storage = offsets + 3;
    g_def.CardOrders._M_start = orders; g_def.CardOrders._M_finish = orders + 2; g_def.CardOrders._M_end_of_storage = orders + 2;
    g_def.m_count = 1;
    game.Pack._M_start = pack; game.Pack._M_finish = pack + 5; game.Pack._M_end_of_storage = pack + 5;
    game.Cards._M_start = table; game.Cards._M_finish = table; game.Cards._M_end_of_storage = table + 2;   // third card overflows
    game.NewRound();
    if (std::strcmp(g_log, "NSDO") != 0 || game.NumCards != 3 || game.Cards._M_finish != table + 3 || table[0] != pack[0]
        || table[1] != pack[1] || table[2] != pack[2] || g_overflows != 1 || g_def.m_count != 1 || g_destroyed != 0)
        { std::printf("007d5a00_TEST FAIL (%s)\n", g_log); return 1; }
    std::printf("007d5a00_TEST PASS\n");
    return 0;
}