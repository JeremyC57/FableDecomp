#include <stdio.h>
static int bounds_calls,map_calls,failures;
class C3DVector{public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}};
class C2DVector{public:float X,Y;C2DVector(float x,float y):X(x),Y(y){}C2DVector(const C3DVector& p):X(p.X),Y(p.Y){}};
class C2DCoordI{public:long X,Y;C2DCoordI(long x,long y):X(x),Y(y){}C2DVector ToC2DCoordF()const{return C2DVector((float)X,(float)Y);}};
class C2DBoxI{public:long TLX,TLY,BRX,BRY;C2DBoxI(){}C2DBoxI(long a,long b,long c,long d):TLX(a),TLY(b),BRX(c),BRY(d){}C2DBoxI(const C2DBoxI& p):TLX(p.TLX),TLY(p.TLY),BRX(p.BRX),BRY(p.BRY){}C2DCoordI GetTLPos()const{return C2DCoordI(TLX,TLY);}C2DCoordI GetBRPos()const{return C2DCoordI(BRX,BRY);}};
class C2DBoxF{public:float TLX,TLY,BRX,BRY;C2DBoxF(const C2DVector& a,const C2DVector& b):TLX(a.X),TLY(a.Y),BRX(b.X),BRY(b.Y){}C2DBoxF(const C2DVector& p,float r):TLX(p.X-r),TLY(p.Y-r),BRX(p.X+r),BRY(p.Y+r){}};
class CTCPhysicsBase{public:C3DVector Centre;float Radius;void GetBoundingInfo(C3DVector& c,float& r)const{++bounds_calls;c=Centre;r=Radius;}};
class CThing{public:const CTCPhysicsBase* PhysicsTC;const CTCPhysicsBase* GetPhysicsTC()const{return PhysicsTC;}};
class CWorldMap{public:C2DBoxI Boxes[3];const C2DBoxI& GetMapBox(long n)const{++map_calls;if(bounds_calls!=1)++failures;return Boxes[n];}};
class CThingFilter_BoundingBoxOverlapsMap{public:CThingFilter_BoundingBoxOverlapsMap(const CWorldMap& w,long n):WorldMap(w),MapNo(n){}bool operator()(const CThing& test_thing)const;private:const CWorldMap& WorldMap;long MapNo;};
bool CThingFilter_BoundingBoxOverlapsMap::operator()(const CThing& test_thing)const
{
    const CTCPhysicsBase* ptc_physics=test_thing.GetPhysicsTC();
    if(ptc_physics) {
        C3DVector centre;
        float radius;
        ptc_physics->GetBoundingInfo(centre,radius);
        C2DBoxI map_area_i=WorldMap.GetMapBox(MapNo);
        C2DBoxF map_area(map_area_i.GetTLPos().ToC2DCoordF(),map_area_i.GetBRPos().ToC2DCoordF());
        C2DBoxF bounding_box(C2DVector(centre),radius);
        if(map_area.TLX>bounding_box.BRX)return false;
        if(map_area.BRX<bounding_box.TLX)return false;
        if(map_area.TLY>bounding_box.BRY)return false;
        if(map_area.BRY<bounding_box.TLY)return false;
        return true;
    }
    return false;
}
static bool expected(const C2DBoxI& m,float x,float y,float r){return !((float)m.TLX>x+r||(float)m.BRX<x-r||(float)m.TLY>y+r||(float)m.BRY<y-r);}
int main(){
 CWorldMap world;world.Boxes[0]=C2DBoxI(0,0,10,10);world.Boxes[1]=C2DBoxI(-10,-5,0,0);world.Boxes[2]=C2DBoxI(2,2,2,2);
 const float positions[]={-12,-10,-5,-1,0,1,2,3,5,9,10,11,12};const float radii[]={0,0.5f,1,2};
 CTCPhysicsBase physics;CThing thing;
 for(int present=0;present<2;++present)for(int m=0;m<3;++m)for(int x=0;x<13;++x)for(int y=0;y<13;++y)for(int r=0;r<4;++r){
  physics.Centre=C3DVector(positions[x],positions[y],999);physics.Radius=radii[r];thing.PhysicsTC=present?&physics:0;CThingFilter_BoundingBoxOverlapsMap filter(world,m);bounds_calls=map_calls=0;
  bool got=filter(thing);if(got!=(present&&expected(world.Boxes[m],positions[x],positions[y],radii[r]))||bounds_calls!=present||map_calls!=present||failures)return 1;
 }
 union FloatBits{unsigned int bits;float value;};FloatBits special;special.bits=0x7fc00000U;physics.Centre=C3DVector(special.value,5,0);physics.Radius=1;thing.PhysicsTC=&physics;CThingFilter_BoundingBoxOverlapsMap filter(world,0);bounds_calls=map_calls=0;if(!filter(thing))return 1;
 special.bits=0x7f800000U;physics.Centre.X=special.value;bounds_calls=map_calls=0;if(filter(thing))return 1;
 puts("006a7fb0_TEST PASS");return 0;
}