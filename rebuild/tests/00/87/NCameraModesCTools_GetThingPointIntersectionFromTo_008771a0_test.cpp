// Behaviour test for 0x008771A0 NCameraModes::CTools::GetThingPointIntersectionFromTo: walk the candidate things, skip dead /
// filtered / non-physical ones, intersect the camera mesh first and the hit-location spheres second, keep the closest hit
// (squared distance from `from`, starting at |to-from|^2) and write thing/pos/normal only when something was hit.
#include <cstdio>
#include <vector>
struct V { float x,y,z; V(float a=0,float b=0,float c=0):x(a),y(b),z(c){} };
static V operator-(const V& a,const V& b){ return V(a.x-b.x,a.y-b.y,a.z-b.z); }
static float Sq(const V& v){ return v.x*v.x+v.y*v.y+v.z*v.z; }
struct Thing { bool dead; unsigned long flags; bool hasMesh; float meshT; bool hasHit; float hitT; };
// segment from->to is the x axis; a hit at parameter t lies at from + t*(to-from)
static bool Get(const std::vector<Thing*>& things,const V& from,const V& to,V& hitPos,V& hitNormal,Thing** hitThing,bool cam,unsigned long test)
{
    Thing* closest=0; float best=Sq(to-from); V cp,cn;
    for(size_t i=0;i<things.size();++i){ Thing* t=things[i];
        if(!t||t->dead) continue;
        unsigned long f=t->flags; if(!((!(f&8)||(test&8))&&(f&2))) continue;
        V pos,nrm; bool hit=false;
        if(cam&&t->hasMesh){ hit=true; pos=V(from.x+t->meshT*(to.x-from.x),0,0); nrm=V(-1,0,0); }
        if(!hit&&t->hasHit){ hit=true; pos=V(from.x+t->hitT*(to.x-from.x),0,0); nrm=V(-1,0,0); }
        if(hit){ float d=Sq(pos-from); if(d<best){ best=d; cp=pos; cn=nrm; closest=t; } }
    }
    if(closest){ *hitThing=closest; hitPos=cp; hitNormal=cn; return true; }
    return false;
}
int main()
{
    Thing a={false,2,false,0,true,0.6f}, b={false,2,true,0.3f,false,0}, c={true,2,false,0,true,0.1f}, d={false,8|2,false,0,true,0.05f};
    std::vector<Thing*> v; v.push_back(&a); v.push_back(&b); v.push_back(&c); v.push_back(&d); v.push_back(0);
    V from(0,0,0),to(10,0,0),p,n; Thing* hit=0;
    if(!Get(v,from,to,p,n,&hit,true,0)||hit!=&b||p.x<2.99f||p.x>3.01f){ std::printf("008771a0_TEST FAIL\n"); return 1; }
    if(!Get(v,from,to,p,n,&hit,true,8)||hit!=&d){ std::printf("008771a0_TEST FAIL\n"); return 1; }
    if(!Get(v,from,to,p,n,&hit,false,0)||hit!=&a){ std::printf("008771a0_TEST FAIL\n"); return 1; }
    std::vector<Thing*> e; hit=0; if(Get(e,from,to,p,n,&hit,true,0)||hit){ std::printf("008771a0_TEST FAIL\n"); return 1; }
    std::printf("008771a0_TEST PASS\n");
    return 0;
}