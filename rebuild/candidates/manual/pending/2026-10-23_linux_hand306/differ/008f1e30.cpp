// Grade: DIFFER(676v692) -- frame 0x38 v 0x34 (card/value slots not shared), value pair spilled vs Suit in ebx, push_back via lea ecx base, def null test at exit dropped by ours
// CTCTavernGameCardBase::EnterGame() -- start a card game: build one face-down card piece per card def, stacked on the pack position, then run the game's start-up virtual, point the camera at the game's camera point and reset the counter
// Leaf: ?EnterGame@CTCTavernGameCardBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F1E30, 692 bytes (+ int3 padding), plain `ret`; vtable 0x01288294 slot 29.
// Callees: CTCTavernGame::EnterGame 0x005EC0F0; CTCTavernGameCardBase::GetCardDef 0x008F0B20 (same-TU stand-in);
//   CDefClassBase::GetSubDef<CTavernGameCardBaseDef> 0x008F27A0 (inside the inlined GetPackPosition); operator new
//   0x00BFEA1A; CTCTavernGame::CreateGamePiece(long) 0x005EA9B0; PositionMesh 0x005EABC0 (`__fastcall`);
//   vector<CCard*>::_M_insert_overflow 0x007D5D60 (same-TU template stand-in); virtual slot 60; CTCTavernGame::
//   GetTavernGameDef 0x005EA000 / CameraUseCameraPoint(const CCharString&) 0x005EBFA0; CDefStringTable::GetString 0x009D49B0
//   on the string table 0x013CA828; ~CCharString 0x0099EAE0.
// Layout (retail): GamePositionRHSet +0xf0, GamePositionPos +0x108, Pack +0x134, Counter +0x154, NumCardsDealt +0x304;
//   CTavernGameCardBaseDef CardWidth/CardHeight +0x2c/+0x30, +0x40/+0x44, HeightScale/HeightOffset +0x5c/+0x60,
//   PackOffset +0x64, card defs +0x78 (0x44 bytes, value pair at +0x3c); CTavernGameDef camera point name id +0x90;
//   CCard (0x58, pack(1)).
struct C2DVector { float X, Y; };
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector& operator+=(const C3DVector& v)
    {
        X += v.X;
        Y += v.Y;
        Z += v.Z;
        return *this;
    }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
inline C3DVector operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
struct CRightHandedSet { C3DVector Up, Forward; };

struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EB90
    ~CCharString();                                     // 0x0099EAE0
};
struct CDefStringTable {
    CCharString GetString(long id);                     // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828

struct CDefPointee {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long m_refCount;                                    // +0x04
    void DecRef() { --m_refCount; }
    long GetRefCount() const { return m_refCount; }
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct SCardValue { long Suit, Value; };
struct CCardDef {                                       // 0x44 bytes
    unsigned char _pad0[0x3c];
    SCardValue Value;                                   // +0x3c
};
struct CTavernGameCardBaseDef : CDefPointee {
    unsigned char _pad08[0x2c - 0x08];
    float CardWidth;                                    // +0x2c
    float CardHeight;                                   // +0x30
    unsigned char _pad34[0x40 - 0x34];
    long Field40;                                       // +0x40
    long Field44;                                       // +0x44
    unsigned char _pad48[0x5c - 0x48];
    float HeightScale;                                  // +0x5c
    float HeightOffset;                                 // +0x60
    C2DVector PackOffset;                               // +0x64
    unsigned char _pad6c[0x78 - 0x6c];
    CCardDef* CardDefsStart;                            // +0x78
    CCardDef* CardDefsFinish;                           // +0x7c
    unsigned long NumCardDefs() const { return CardDefsFinish - CardDefsStart; }
};
struct CTavernGameDef : CDefPointee {
    unsigned char _pad08[0x90 - 0x08];
    long CameraPointName;                               // +0x90
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        T* object = Object;
        if (object) {
            if (--object->m_refCount == 0)
                object->Destroy();
            Object = 0;
        }
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        T* old = Object;
        T* object = other.Object;
        if (old != object) {
            if (old) {
                if (--old->m_refCount == 0)
                    old->Destroy();
                Object = 0;
            }
            Object = object;
            if (object)
                ++object->m_refCount;
        }
        return *this;
    }
    T* operator->() const { return Object; }
};
struct CGameDefPointer {
    CTavernGameDef* Object;
    CGameDefPointer() : Object(0) { }
    ~CGameDefPointer()
    {
        if (Object) {
            Object->ReleaseTemporary();
            Object = 0;
        }
    }
    CTavernGameDef* operator->() const { return Object; }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CTavernGameCardBaseDef>* out);   // 0x008F27A0
};
void* FindDef_CDefClassBase_GetSubDef_0();
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CTavernGameCardBaseDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad0[0x70];
    CDefClassBase* PDef;                                // +0x70
};
struct CThingObject;
void __fastcall PositionMesh(CThingObject* thing, const C3DVector* gamePos, const CRightHandedSet* set, const C3DVector* pos,
                             float a, float b);         // 0x005EABC0

struct CTCTavernGameCardBase;
#pragma pack(push, 1)
struct CCard {
    CThingObject* Thing;                                // +0x00
    SCardValue Value;                                   // +0x04
    unsigned char _pad0c[4];
    float Scale;                                        // +0x10
    long Field14;                                       // +0x14
    unsigned char _pad18[4];
    long Field1c;                                       // +0x1c
    long State;                                         // +0x20
    bool Turning;                                       // +0x24
    bool FaceDown;                                      // +0x25
    bool Placed;                                        // +0x26
    C3DVector OldPosition;                              // +0x27
    C3DVector Target;                                   // +0x33
    C3DVector Position;                                 // +0x3f
    unsigned char _pad4b;
    float Width;                                        // +0x4c
    float Height;                                       // +0x50
    long Index;                                         // +0x54
    inline CCard(CTCTavernGameCardBase* game, SCardValue value, const C3DVector& pos, long index);
};
#pragma pack(pop)

inline void* operator new(unsigned int, void* p) { return p; }
struct __true_type { __true_type() { } };
template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }
template <class T> struct vector {                      // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fillLength, bool atEnd);   // 0x007D5D60
    __forceinline void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1UL, true);
    }
};
void* GrowVectorStorage_007d5d60(void* vector, unsigned long count);
#pragma auto_inline(off)
template <class T>
__declspec(noinline) void vector<T>::_M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fillLength, bool atEnd)
{
    T* p = (T*)GrowVectorStorage_007d5d60(this, fillLength);
    if (p)
        *p = x;
}
#pragma auto_inline(on)

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
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void StartGame();                           // slot 60
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0xf0 - 0x08];
    CRightHandedSet GamePositionRHSet;                  // +0xf0
    C3DVector GamePositionPos;                          // +0x108
    unsigned char _pad114[0x134 - 0x114];
    vector<CCard*> Pack;                                // +0x134
    unsigned char _pad140[0x154 - 0x140];
    float Counter;                                      // +0x154
    unsigned char _pad158[0x304 - 0x158];
    long NumCardsDealt;                                 // +0x304
    void BaseEnterGame();                               // CTCTavernGame::EnterGame 0x005EC0F0
    void GetTavernGameDef(CGameDefPointer* out);        // 0x005EA000
    CThingObject* CreateGamePiece(long type);           // 0x005EA9B0
    void CameraUseCameraPoint(const CCharString& name); // 0x005EBFA0
    void GetCardDef(CDefPointer<CTavernGameCardBaseDef>* out);   // 0x008F0B20
    __forceinline void GetCardDefInline(CDefPointer<CTavernGameCardBaseDef>* out)
    {
        CDefPointer<CTavernGameCardBaseDef> def;
        ParentThing->PDef->GetSubDef(&def);
        *out = def;
    }
    __forceinline C3DVector GetPackPosition()
    {
        CDefPointer<CTavernGameCardBaseDef> def;
        GetCardDefInline(&def);
        return C3DVector(def->CardWidth * def->PackOffset.X, def->CardHeight * def->PackOffset.Y, 0.0f);
    }
    void EnterGame();
};
void* FindRef_CTCTavernGameCardBase_GetCardDef_0();
#pragma auto_inline(off)
__declspec(noinline) void CTCTavernGameCardBase::GetCardDef(CDefPointer<CTavernGameCardBaseDef>* out)
{
    void* p = FindRef_CTCTavernGameCardBase_GetCardDef_0();
    if (!p)
        return;
    *(void**)out = p;
}
#pragma auto_inline(on)

inline CCard::CCard(CTCTavernGameCardBase* game, SCardValue value, const C3DVector& pos, long index)
{
    Thing = game->CreateGamePiece(value.Suit);
    Value = value;
    FaceDown = false;
    Scale = 0.0f;
    Turning = false;
    Placed = false;
    OldPosition = pos;
    Width = 0.0f;
    Height = 0.0f;
    Index = index;
}

void CTCTavernGameCardBase::EnterGame()
{
    BaseEnterGame();
    CDefPointer<CTavernGameCardBaseDef> def;
    GetCardDef(&def);
    NumCardsDealt = 0;
    for (long i = 0; i < def->NumCardDefs(); ++i) {
        C3DVector pos(0.0f, 0.0f, (float)(0x35 - i) * def->HeightScale + def->HeightOffset);
        pos += GetPackPosition();
        CCard* card = new CCard(this, def->CardDefsStart[i].Value, pos, i);
        card->Field1c = def->Field40;
        card->Field14 = def->Field44;
        PositionMesh(card->Thing, &GamePositionPos, &GamePositionRHSet, &pos, 0.5f, 0.5f);
        card->Scale = 0.5f;
        card->FaceDown = true;
        card->Width = def->CardWidth;
        card->Height = def->CardHeight;
        Pack.push_back(card);
    }
    StartGame();
    CGameDefPointer gameDef;
    GetTavernGameDef(&gameDef);
    {
        CCharString name = g_DefStringTable.GetString(gameDef->CameraPointName);
        CameraUseCameraPoint(name);
    }
    Counter = 0.0f;
}
