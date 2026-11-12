// Behaviour test for CTCTavernGameCardBase::ShuffleCards 0x008F10B0, through a copy of the model: the pack is shuffled (a
// permutation of the same cards), every pack card is hidden and positioned once at the pack position with the height
// (53 - i) * CardSeparation + TableSeparation, both stored positions take that value, the card is turned face down with a final
// rotation of 0.5, and every def reference taken (one for the function, one per GetPackPosition) is released again.
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    const C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
};

static int g_destroyed = 0;
class CTavernGameCardBaseDef { public:
    long  IVRefCount;
    float CardWidth, CardHeight, CardSeparation, TableSeparation, PackOffset_X, PackOffset_Y;
    std::vector<int> CardPack;                 // one entry per card of the pack
    void Delete() { ++g_destroyed; }
};

struct CDefPointer {
    CTavernGameCardBaseDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { Release(); }
    void Release()
    {
        if (Object) {
            CTavernGameCardBaseDef* object = Object;
            if (--Object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object) ++Object->IVRefCount;
        }
        return *this;
    }
    CTavernGameCardBaseDef* operator->() const { return Object; }
};

class CThing { public: int hidden; int positioned; C3DVector at; float forward, up; };
class CCard { public:
    CThing*   Thing;
    float     FinalRotation;
    bool      FaceDown;
    C3DVector LocalPos, NewLocalPos;
};

static void HideMesh(CThing* thing) { ++thing->hidden; }
static void PositionMesh(CThing* thing, const C3DVector&, const C3DVector& offset, float forwardAngle, float upAngle)
{
    ++thing->positioned;
    thing->at = offset;
    thing->forward = forwardAngle;
    thing->up = upAngle;
}

struct CTCTavernGameCardBase {
    CTavernGameCardBaseDef* ParentDef;
    C3DVector               GamePositionPos;
    std::vector<CCard*>     Pack;

    bool GetSubDef(CDefPointer* out)           // the def manager hands out a counted reference
    {
        CDefPointer base;
        base.Object = ParentDef;
        ++ParentDef->IVRefCount;
        *out = base;
        return true;
    }
    void GetCardDef(CDefPointer* pdef)
    {
        CDefPointer def;
        GetSubDef(&def);
        *pdef = def;
    }
    C3DVector GetPackPosition()
    {
        CDefPointer def;
        GetCardDef(&def);
        return C3DVector(def->CardWidth * def->PackOffset_X, def->CardHeight * def->PackOffset_Y, 0.0f);
    }
    void ShuffleCards()
    {
        CDefPointer pdef;
        GetCardDef(&pdef);
        if (Pack.begin() != Pack.end())          // std::random_shuffle(Pack.begin(), Pack.end())
            for (std::vector<CCard*>::iterator it = Pack.begin() + 1; it != Pack.end(); ++it) {
                std::vector<CCard*>::iterator other = Pack.begin() + std::rand() % ((it - Pack.begin()) + 1);
                CCard* tmp = *it; *it = *other; *other = tmp;
            }
        for (unsigned int i = 0; i < pdef->CardPack.size(); ++i) {
            C3DVector pos(0.0f, 0.0f, (float)(53 - (int)i) * pdef->CardSeparation + pdef->TableSeparation);
            pos += GetPackPosition();
            CCard* card = Pack[i];
            HideMesh(card->Thing);
            PositionMesh(card->Thing, GamePositionPos, pos, 0.5f, 0.5f);
            card->LocalPos = pos;
            card->NewLocalPos = pos;
            card->FinalRotation = 0.5f;
            card->FaceDown = true;
        }
    }
};

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }

int main()
{
    const int N = 52;
    CTavernGameCardBaseDef def;
    def.IVRefCount = 1;
    def.CardWidth = 0.25f; def.CardHeight = 0.5f; def.CardSeparation = 0.01f; def.TableSeparation = 0.75f;
    def.PackOffset_X = 2.0f; def.PackOffset_Y = -3.0f;
    def.CardPack.assign(N, 0);

    static CThing things[N];
    static CCard  cards[N];
    CTCTavernGameCardBase game;
    game.ParentDef = &def;
    game.GamePositionPos = C3DVector(10.0f, 20.0f, 30.0f);
    for (int i = 0; i < N; ++i) {
        things[i].hidden = things[i].positioned = 0;
        cards[i].Thing = &things[i];
        cards[i].FinalRotation = 0.0f;
        cards[i].FaceDown = false;
        cards[i].LocalPos = cards[i].NewLocalPos = C3DVector(-1.0f, -1.0f, -1.0f);
        game.Pack.push_back(&cards[i]);
    }

    bool ok = true;
    std::srand(1234);
    game.ShuffleCards();

    ok = ok && def.IVRefCount == 1 && g_destroyed == 0;            // every reference released, the def survives
    ok = ok && (int)game.Pack.size() == N;
    int moved = 0;
    static int seen[N];
    for (int i = 0; ok && i < N; ++i) {
        CCard* card = game.Pack[i];
        int index = (int)(card - cards);
        if (index < 0 || index >= N || seen[index]++) { ok = false; break; }   // a permutation of the same cards
        if (index != i) ++moved;
        float z = (float)(53 - i) * def.CardSeparation + def.TableSeparation;
        ok = ok && card->Thing->hidden == 1 && card->Thing->positioned == 1;
        ok = ok && Near(card->LocalPos.X, 0.5f) && Near(card->LocalPos.Y, -1.5f) && Near(card->LocalPos.Z, z);
        ok = ok && card->NewLocalPos.X == card->LocalPos.X && card->NewLocalPos.Y == card->LocalPos.Y && card->NewLocalPos.Z == card->LocalPos.Z;
        ok = ok && card->Thing->at.X == card->LocalPos.X && card->Thing->at.Y == card->LocalPos.Y && card->Thing->at.Z == card->LocalPos.Z;
        ok = ok && card->Thing->forward == 0.5f && card->Thing->up == 0.5f;
        ok = ok && card->FinalRotation == 0.5f && card->FaceDown;
    }
    ok = ok && moved > 0;                                           // the shuffle did reorder the pack
    ok = ok && Near(game.Pack[0]->LocalPos.Z, 1.28f) && Near(game.Pack[N - 1]->LocalPos.Z, 0.77f);   // first card 53 separations up, last card 2

    // an empty card pack positions nothing, and the last reference destroys the def
    CTavernGameCardBaseDef empty;
    empty.IVRefCount = 0;
    empty.CardWidth = empty.CardHeight = empty.CardSeparation = empty.TableSeparation = empty.PackOffset_X = empty.PackOffset_Y = 0.0f;
    CTCTavernGameCardBase none;
    none.ParentDef = &empty;
    none.GamePositionPos = C3DVector(0.0f, 0.0f, 0.0f);
    none.ShuffleCards();
    ok = ok && empty.IVRefCount == 0 && g_destroyed == 1;

    if (!ok) { std::printf("008f10b0_TEST FAIL\n"); return 1; }
    std::printf("008f10b0_TEST PASS\n");
    return 0;
}