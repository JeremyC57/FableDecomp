#include <vector>
#include <stdlib.h>
#include <stdio.h>
static void check(bool ok){if(!ok)abort();}
struct C3DVector { float X,Y,Z; };
class CTCCreatureNavigation { public:
    unsigned int mask; mutable unsigned int visits;
    bool IsAbleToNavigateFromPosToPos(const C3DVector& from,const C3DVector& to,float tolerance)const{
        check(from.X==99 && from.Y==-8 && from.Z==2 && tolerance==0.0f);
        unsigned int bit=1u<<(unsigned int)to.X;
        check((visits&bit)==0);visits|=bit;return (mask&bit)!=0;
    }
};
class CThingCreatureBase { public:
    CTCCreatureNavigation* nav;C3DVector pos;mutable int lookups;
    const C3DVector& GetPos()const{return pos;}
    bool PeekTC(const CTCCreatureNavigation** out)const{++lookups;*out=nav;return nav!=0;}
};
class CTCCreatureGenerator {public:
    std::vector<C3DVector> OpenPositions;CThingCreatureBase owner;
    const CThingCreatureBase& GetParentThing()const{return owner;}
    void DoNavCheck(const CThingCreatureBase&);
};
void CTCCreatureGenerator::DoNavCheck(const CThingCreatureBase& pCreatureToTest)
{
    const long pointsBeforeCheck = OpenPositions.size();
    if (pointsBeforeCheck > 0) {
        C3DVector startPos;
        startPos=GetParentThing().GetPos();
        const CTCCreatureNavigation* pTCNavigate;
        pCreatureToTest.PeekTC(&pTCNavigate);
        long numPoints = pointsBeforeCheck;
        for (long i = 0; i < numPoints;) {
            if (!pTCNavigate->IsAbleToNavigateFromPosToPos(startPos, OpenPositions[i], 0.0f)) {
                OpenPositions[i] = OpenPositions[--numPoints];
                OpenPositions.pop_back();
            } else {
                ++i;
            }
        }
    }
}

int main(){
    for(unsigned int n=0;n<=8;++n)for(unsigned int mask=0;mask<(1u<<n);++mask){
        CTCCreatureGenerator g;CTCCreatureNavigation nav;CThingCreatureBase actor;
        g.owner.pos.X=99;g.owner.pos.Y=-8;g.owner.pos.Z=2;actor.nav=&nav;actor.lookups=0;nav.mask=mask;nav.visits=0;
        for(unsigned int i=0;i<n;++i){C3DVector v={(float)i,8.0f,4.0f};g.OpenPositions.push_back(v);}
        g.DoNavCheck(actor);check(actor.lookups==(n?1:0) && nav.visits==(1u<<n)-1);
        unsigned int seen=0;for(unsigned int i=0;i<g.OpenPositions.size();++i){const C3DVector& v=g.OpenPositions[i];unsigned int b=1u<<(unsigned int)v.X;check((seen&b)==0 && v.Y==8 && v.Z==4);seen|=b;}
        check(seen==mask);
    }
    puts("0074a4e0_TEST PASS");return 0;
}
