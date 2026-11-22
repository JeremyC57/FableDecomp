#include <list>
#include <string.h>
#include <stdio.h>
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_INITIALISE_SUB
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/ThingAlive.h"
#define FABLE_RETAIL_ASSERT(x) ((void)0)

class AxisFixture : public CRightHandedSet {
public:
    AxisFixture(bool rotated) {
        Forward = rotated ? C3DVector(-1.0f,0.0f,0.0f) : C3DVector(0.0f,1.0f,0.0f);
        Up = C3DVector(0.0f,0.0f,1.0f);
    }
};
struct CTavernGameShoveHaPennyDef { long BoardDefIndex; };
template<class T> class CDefPointer {
public:
    CDefPointer() : Ptr(0) {}
    const T* operator->() const { return Ptr; }
    const T* Ptr;
};
template<class T> struct CIntelligentPointer {
    CIntelligentPointer(T* thing) : Ptr(thing) {}
    T* Get() const { return Ptr; }
    T* Ptr;
};
namespace NGraphicAppearance {
struct CTCGraphicAppearance {
    CTCGraphicAppearance(bool rotated) : Axes(rotated), MinReads(0), MaxReads(0), Valid(true) {}
    bool GetDummyObject(const char* name, long index, unsigned long flags, C3DVector& output,
                        CRightHandedSet& rhset, float scale) const {
        if (index || flags || scale!=1.0f)
            Valid = false;
        rhset = Axes;
        if (!strcmp(name,"DUMMY_OBJECT_MIN")) {
            ++MinReads;
            output = Minimum;
            return true;
        }
        if (!strcmp(name,"DUMMY_OBJECT_MAX")) {
            ++MaxReads;
            output = Maximum;
            return true;
        }
        Valid = false;
        return false;
    }
    AxisFixture Axes;
    C3DVector Minimum, Maximum;
    mutable int MinReads, MaxReads;
    mutable bool Valid;
};
}
struct CThing;
struct CTCThingOwner {
    const std::list<CIntelligentPointer<CThing> >& GetObjectList() const { return Children; }
    std::list<CIntelligentPointer<CThing> > Children;
};
struct CThing {
    CThing() : DefLookups(0), TCRequests(0), Index(0), Dead(false), Owner(0), Appearance(0) {}
    CThing& PeekBaseDef() { return *this; }
    bool GetSubDef(CDefPointer<const CTavernGameShoveHaPennyDef>* out) { ++DefLookups; out->Ptr = &Definition; return true; }
    bool PeekTC(const CTCThingOwner** out) { ++TCRequests; *out = Owner; return Owner != 0; }
    bool IsDead() const { return Dead; }
    unsigned long GetDefGlobalIndex() const { return Index; }
    NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance() { return Appearance; }
    CTavernGameShoveHaPennyDef Definition;
    int DefLookups, TCRequests;
    unsigned long Index;
    bool Dead;
    CTCThingOwner* Owner;
    NGraphicAppearance::CTCGraphicAppearance* Appearance;
};
struct CTCTavernGame {
    CTCTavernGame() : BaseActivationCalls(0) {}
    void OnInitialActivate() { ++BaseActivationCalls; }
    CThing& GetParentThing() { return *ParentThing; }
    CThing* ParentThing;
    int BaseActivationCalls;
};
struct CTCTavernGameShoveHaPenny : CTCTavernGame {
    void OnInitialActivate();
    C3DVector BoardPos, BoardSize;
    CRightHandedSet BoardRHSet;
};

void CTCTavernGameShoveHaPenny::OnInitialActivate()
{
    CTCTavernGame::OnInitialActivate();
    CDefPointer<const CTavernGameShoveHaPennyDef> pdef;
    GetParentThing().PeekBaseDef().GetSubDef(&pdef);
    CThing* board = 0;
    const CTCThingOwner* owner = 0;
    if (ParentThing->PeekTC(&owner)) {
        const std::list<CIntelligentPointer<CThing> >& children = owner->GetObjectList();
        for (std::list<CIntelligentPointer<CThing> >::const_iterator i = children.begin(); i != children.end(); ++i) {
            CThing* thing = i->Get();
            if (IS_THING_ALIVE(thing) && thing->GetDefGlobalIndex() == pdef->BoardDefIndex) {
                board = thing;
                break;
            }
        }
    }
    FABLE_RETAIL_ASSERT(board);
    if (board) {
        const NGraphicAppearance::CTCGraphicAppearance* graphic_appearance = board->GetTCGraphicAppearance();
        C3DVector min, max;
        CRightHandedSet rhset;
        graphic_appearance->GetDummyObject("DUMMY_OBJECT_MIN", 0, 0, min, rhset, 1.0f);
        graphic_appearance->GetDummyObject("DUMMY_OBJECT_MAX", 0, 0, max, rhset, 1.0f);
        C3DVector size = max - min;
        BoardPos = min;
        BoardRHSet = rhset;
        BoardSize = C3DVector(rhset.GetRight().Dot(size), rhset.GetForward().Dot(size), rhset.GetUp().Dot(size));
        if (BoardSize.X < 0.0f) {
            BoardPos += rhset.GetRight() * BoardSize.X;
            BoardSize.X = -BoardSize.X;
        }
        if (BoardSize.Y < 0.0f) {
            BoardPos += rhset.GetForward() * BoardSize.Y;
            BoardSize.Y = -BoardSize.Y;
        }
        if (BoardSize.Z < 0.0f) {
            BoardPos += rhset.GetUp() * BoardSize.Z;
            BoardSize.Z = -BoardSize.Z;
        }
        BoardPos.Z += BoardSize.Z;
    }
}

static bool same(const C3DVector& a, const C3DVector& b) { return a.X==b.X && a.Y==b.Y && a.Z==b.Z; }
int main() {
    int cases=0;
    for (int rotated=0;rotated!=2;++rotated) {
        for (int signs=0;signs!=8;++signs) {
            NGraphicAppearance::CTCGraphicAppearance shape(rotated!=0), secondShape(rotated!=0);
            const float x=(signs&1)?-2.0f:2.0f, y=(signs&2)?-3.0f:3.0f, z=(signs&4)?-5.0f:5.0f;
            shape.Minimum=C3DVector(10.0f,20.0f,30.0f);
            shape.Maximum=shape.Minimum+(rotated ? C3DVector(-y,x,z) : C3DVector(x,y,z));
            CThing parent, dead, unrelated, firstBoard, secondBoard;
            CTCThingOwner owner;
            parent.Definition.BoardDefIndex=27;
            parent.Owner=&owner;
            dead.Index=27;
            dead.Dead=true;
            unrelated.Index=26;
            firstBoard.Index=secondBoard.Index=27;
            firstBoard.Appearance=&shape;
            secondBoard.Appearance=&secondShape;
            owner.Children.push_back(0);
            owner.Children.push_back(&dead);
            owner.Children.push_back(&unrelated);
            owner.Children.push_back(&firstBoard);
            owner.Children.push_back(&secondBoard);
            CTCTavernGameShoveHaPenny setup;
            setup.ParentThing=&parent;
            setup.OnInitialActivate();
            const float adjustX=x<0.0f?x:0.0f, adjustY=y<0.0f?y:0.0f, adjustZ=z<0.0f?z:0.0f;
            const C3DVector expected=shape.Minimum+
                (rotated ? C3DVector(-adjustY,adjustX,adjustZ+5.0f) : C3DVector(adjustX,adjustY,adjustZ+5.0f));
            if (shape.MinReads!=1 || shape.MaxReads!=1 || !shape.Valid || secondShape.MinReads || secondShape.MaxReads
                || setup.BaseActivationCalls!=1 || parent.DefLookups!=1 || parent.TCRequests!=1
                || !same(setup.BoardPos,expected) || !same(setup.BoardSize,C3DVector(2.0f,3.0f,5.0f))
                || !same(setup.BoardRHSet.GetForward(),shape.Axes.GetForward()) || !same(setup.BoardRHSet.GetUp(),shape.Axes.GetUp())) {
                printf("FAIL rotated=%d signs=%d\n",rotated,signs);
                return 1;
            }
            ++cases;
        }
    }
    for (int empty=0;empty!=2;++empty) {
        CThing parent;
        CTCThingOwner emptyOwner;
        parent.Owner=empty?&emptyOwner:0;
        parent.Definition.BoardDefIndex=27;
        CTCTavernGameShoveHaPenny setup;
        setup.ParentThing=&parent;
        setup.BoardPos=C3DVector(4.0f,5.0f,6.0f);
        setup.BoardSize=C3DVector(2.0f,3.0f,4.0f);
        setup.OnInitialActivate();
        if (!same(setup.BoardPos,C3DVector(4.0f,5.0f,6.0f)) || !same(setup.BoardSize,C3DVector(2.0f,3.0f,4.0f))
            || setup.BaseActivationCalls!=1 || parent.DefLookups!=1 || parent.TCRequests!=1)
            return 2;
        ++cases;
    }
    if (cases!=18)
        return 3;
    puts("007d04f0_TEST PASS");
    return 0;
}