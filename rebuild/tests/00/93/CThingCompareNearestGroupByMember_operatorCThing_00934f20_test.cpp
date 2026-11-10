#include <cstdio>
class C3DVector { public: float X,Y,Z; C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){} C3DVector operator-(const C3DVector& o)const{return C3DVector(X-o.X,Y-o.Y,Z-o.Z);} float GetSquaredMagnitude()const{return X*X+Y*Y+Z*Z;} };
class CTCCreatureGroup;
class CThing { public: CTCCreatureGroup* Group; C3DVector Position; CThing():Group(0){} template<class T>bool PeekTC(T** out)const {if(Group){*out=static_cast<T*>(Group);return true;}return false;} const C3DVector& GetPos()const{return Position;} };
class CThingAICreature:public CThing{};
class CTCCreatureGroup { public: const CThingAICreature* Sequence[2]; mutable int Calls; CTCCreatureGroup():Calls(0){} const CThingAICreature* GetNearestMember(const C3DVector&)const{return Sequence[(Calls++)%2];} };
class CThingCompare_NearestGroupByMember {
public:
    C3DVector Pos;                                                  // +0x00
    template <class T> T* operator()(T* a, T* b) const
    {
        if (!a)
            return b;
        if (!b)
            return a;
        const CTCCreatureGroup* groupA;
        if(a->PeekTC(&groupA)) {
            const CTCCreatureGroup* groupB;
            if(b->PeekTC(&groupB)) {
                const CThingAICreature* nearestA=groupA->GetNearestMember(Pos);
                const CThingAICreature* nearestB=groupA->GetNearestMember(Pos);
                if(!nearestA)return b;
                if(!nearestB)return a;
                if((nearestA->GetPos()-Pos).GetSquaredMagnitude() < (nearestB->GetPos()-Pos).GetSquaredMagnitude())return a;
                return b;
            }
            return a;
        }
        return b;
    }
};

static CThing* Reference(CThing* a,CThing* b,const CThingAICreature* x,const CThingAICreature* y){if(!a)return b;if(!b)return a;if(!a->Group)return b;if(!b->Group)return a;if(!x)return b;if(!y)return a;float da=x->Position.X*x->Position.X+x->Position.Y*x->Position.Y+x->Position.Z*x->Position.Z;float db=y->Position.X*y->Position.X+y->Position.Y*y->Position.Y+y->Position.Z*y->Position.Z;return da<db?a:b;}
int main(){CThingAICreature nearMember,farMember;nearMember.Position=C3DVector(1,0,0);farMember.Position=C3DVector(0,0,4);const CThingAICreature* choices[3]={0,&nearMember,&farMember};int ok=1,cases=0;
for(int na=0;na<2;++na)for(int nb=0;nb<2;++nb)for(int ga=0;ga<2;++ga)for(int gb=0;gb<2;++gb)for(int x=0;x<3;++x)for(int y=0;y<3;++y){CTCCreatureGroup groupA,groupB;groupA.Sequence[0]=choices[x];groupA.Sequence[1]=choices[y];groupB.Sequence[0]=&farMember;groupB.Sequence[1]=&nearMember;CThing a,b;a.Group=ga?&groupA:0;b.Group=gb?&groupB:0;CThing* first=na?&a:0;CThing* second=nb?&b:0;CThingCompare_NearestGroupByMember compare;compare.Pos=C3DVector(0,0,0);ok &= compare(first,second)==Reference(first,second,choices[x],choices[y]);ok &= groupB.Calls==0;ok &= groupA.Calls==((na&&nb&&ga&&gb)?2:0);++cases;}
ok &= cases==144;std::printf(ok?"00934f20_TEST PASS\n":"00934f20_TEST FAIL\n");return ok?0:1;}