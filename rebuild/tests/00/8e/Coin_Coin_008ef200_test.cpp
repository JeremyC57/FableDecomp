#include <stdio.h>
#include <math.h>
struct C2DVector{float X,Y;C2DVector(float x=0,float y=0):X(x),Y(y){}};
class CThing{};
class CIntelligentPointer {
public:
    CIntelligentPointer(CThing* p=0):item(p){}
    CThing* item;
};
class Mass {
public:
    C2DVector Pos,Vel;
    float IMass, APos, AVel, AIMass;
    Mass();
};
class Coin : public Mass {
public:
    Coin(float,float,CThing*,bool);
    bool Owned;
    CIntelligentPointer Thing;
    float Radius;
    int Owner;
    unsigned long Sound;
};
extern const unsigned long g_InvalidSoundHandle=0xffffffffUL;
Mass::Mass()
{
    Pos = C2DVector(0.0f, 0.0f);
    Vel = C2DVector(0.0f, 0.0f);
    APos = 0.0f;
    AVel = 0.0f;
}

Coin::Coin(float radius, float idensity, CThing* thing, bool owned)
{
    Owned = owned;
    Thing = thing;
    Radius = radius;
    IMass = (float)(idensity / (6.283185307 * Radius * Radius));
    AIMass = (float)(2.5 * IMass / (Radius * Radius));
    Sound = g_InvalidSoundHandle;
}

int main() {
    CThing thing;
    struct Case {float radius, density; bool owned; CThing* pointer;};
    const Case cases[]={{1,1,true,&thing},{2,3,false,0},{-4,0,true,0},{0.5f,-2,false,&thing}};
    for(unsigned i=0;i<sizeof(cases)/sizeof(cases[0]);++i) {
        const Case& c=cases[i];
        Coin coin(c.radius,c.density,c.pointer,c.owned);
        double r2=(double)c.radius*c.radius;
        double inv=c.density/(r2*6.283185307);
        if (coin.Owned!=c.owned||coin.Thing.item!=c.pointer||coin.Radius!=c.radius||
            coin.Sound!=0xffffffffUL||coin.Pos.X!=0||coin.Pos.Y!=0||coin.Vel.X!=0||coin.Vel.Y!=0||
            coin.APos!=0||coin.AVel!=0||fabs(coin.IMass-inv)>0.0000002||
            fabs(coin.AIMass-inv*2.5/r2)>0.000002)return 1;
    }
    puts("008ef200_TEST PASS");
}