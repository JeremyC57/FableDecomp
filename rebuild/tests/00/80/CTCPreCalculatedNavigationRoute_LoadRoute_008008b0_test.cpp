// Exercise the production loader, its serialized key order, coordinate conversion, clear and append.
#include <stdio.h>
#include <string>
#include <vector>
#include <utility>
struct C2DVector {float X,Y;C2DVector(float x=0,float y=0):X(x),Y(y){}};
struct Map {
    mutable int calls; C2DVector ConvertLocalCoordsToWorldMapCoords(long index,const C2DVector& pos) const{
        ++calls;return C2DVector(pos.X+index,pos.Y-index);
    }
};
struct Thing {long GetCurrentMapNumber()const{return 4;}};
struct CCharString:std::string {CCharString(const char* s):std::string(s){}operator const char*()const{return c_str();}};
static CCharString GFIntToCharString(long n){char b[24];sprintf(b,"%ld",n);return CCharString(b);}
static CCharString operator+(const char* a,const CCharString& b){return CCharString((std::string(a)+b.c_str()).c_str());}
struct CPersistContext {
    long steps;int errors;int visits;
    void Transfer(const char* name,long& value){if(std::string(name)!="NumberOfStepsOnRoute"||visits!=0)++errors;++visits;value=steps;}
    void Transfer(const char* name,C2DVector& value){
        int index=(visits-1)/2;char b[32];sprintf(b,"NavPosition%d",index);
        if(std::string(name)!=b||visits%2!=1)++errors;++visits;value=C2DVector(float(index*2),float(-index));
    }
    void Transfer(const char* name,unsigned char& value){
        int index=(visits-2)/2;char b[32];sprintf(b,"NavLayer%d",index);
        if(std::string(name)!=b||visits%2!=0)++errors;++visits;value=static_cast<unsigned char>(253+index%3);
    }
};
class CTCPreCalculatedNavigationRoute {
public:
    Map map;Thing owner;std::vector<std::pair<C2DVector,unsigned char> > Route;
    Thing& GetParentThing(){return owner;}const Map& PeekWorldMap()const{return map;}
    void LoadRoute(CPersistContext&,long);
};

void CTCPreCalculatedNavigationRoute::LoadRoute(CPersistContext& context, long version)
{
    long map_index = GetParentThing().GetCurrentMapNumber();
    long number_of_steps_on_route;
    context.Transfer("NumberOfStepsOnRoute", number_of_steps_on_route);
    Route.clear();
    Route.reserve(number_of_steps_on_route);
    for (long c1 = 0; c1 < number_of_steps_on_route; ++c1) {
        C2DVector position;
        context.Transfer("NavPosition" + GFIntToCharString(c1), position);
        if (version >= 2)
            position = PeekWorldMap().ConvertLocalCoordsToWorldMapCoords(map_index, position);
        unsigned char layer;
        context.Transfer("NavLayer" + GFIntToCharString(c1), layer);
        Route.push_back(std::make_pair(position, layer));
    }
}

int main(){
    int counts[]={0,1,3,12};int versions[]={0,1,2,3};
    for(int v=0;v<4;++v)for(int n=0;n<4;++n){
        CTCPreCalculatedNavigationRoute route;route.map.calls=0;
        route.Route.push_back(std::make_pair(C2DVector(32,32),static_cast<unsigned char>(17)));
        CPersistContext ctx;ctx.steps=counts[n];ctx.visits=ctx.errors=0;
        route.LoadRoute(ctx,versions[v]);
        if(ctx.errors||ctx.visits!=1+2*counts[n]||int(route.Route.size())!=counts[n])return 1;
        if(route.map.calls!=(versions[v]>=2?counts[n]:0))return 2;
        for(int i=0;i<counts[n];++i){
            int adjust=versions[v]>=2?4:0;
            if(route.Route[i].first.X!=float(i*2+adjust)||route.Route[i].first.Y!=float(-i-adjust)||
                route.Route[i].second!=253+i%3)return 3;
        }
    }
    puts("008008b0_TEST PASS");return 0;
}