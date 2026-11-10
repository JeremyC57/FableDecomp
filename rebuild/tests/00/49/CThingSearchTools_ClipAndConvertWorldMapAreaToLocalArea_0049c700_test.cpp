#include <stdio.h>
#include <stdlib.h>
class C2DBoxI {public:
 long TLX,TLY,BRX,BRY;
 C2DBoxI(long a,long b,long c,long d):TLX(a),TLY(b),BRX(c),BRY(d){}
 C2DBoxI(const C2DBoxI& o):TLX(o.TLX),TLY(o.TLY),BRX(o.BRX),BRY(o.BRY){}
 void ClipTo(const C2DBoxI& b) {
  if(TLX<b.TLX) TLX=b.TLX; else if(TLX>b.BRX) TLX=b.BRX;
  if(TLY<b.TLY) TLY=b.TLY; else if(TLY>b.BRY) TLY=b.BRY;
  if(BRX<b.TLX) BRX=b.TLX; else if(BRX>b.BRX) BRX=b.BRX;
  if(BRY<b.TLY) BRY=b.TLY; else if(BRY>b.BRY) BRY=b.BRY;
 }
};
namespace NThingMap {class CThingMap {public:
 C2DBoxI Area;
 CThingMap(const C2DBoxI& a):Area(a){}
 const C2DBoxI& GetArea()const{return Area;}
};}
class CThingSearchTools {public:
 C2DBoxI ClipAndConvertWorldMapAreaToLocalArea(const NThingMap::CThingMap&,const C2DBoxI&) const;
};
C2DBoxI CThingSearchTools::ClipAndConvertWorldMapAreaToLocalArea(const NThingMap::CThingMap& map, const C2DBoxI& area) const
{
    const C2DBoxI& clip_area = map.GetArea();
    C2DBoxI clipped_area(area);
    clipped_area.ClipTo(clip_area);
    clipped_area.TLX -= clip_area.TLX;
    clipped_area.BRX -= clip_area.TLX;
    clipped_area.TLY -= clip_area.TLY;
    clipped_area.BRY -= clip_area.TLY;
    return clipped_area;
}

static long clamp(long x,long lo,long hi){return x<lo?lo:x>hi?hi:x;}
int main(){
 CThingSearchTools tools;
 const long vals[]={-32,-16,-8,0,8,16,32};
 for(int m=0;m<3;++m){
  NThingMap::CThingMap map(C2DBoxI(-8+m*8,-16+m*8,8+m*8,16+m*8));
  for(int a=0;a<7;++a)for(int b=0;b<7;++b)for(int c=0;c<7;++c)for(int e=0;e<7;++e){
   C2DBoxI input(vals[a],vals[b],vals[c],vals[e]);
   C2DBoxI got=tools.ClipAndConvertWorldMapAreaToLocalArea(map,input);
   if(got.TLX!=clamp(input.TLX,map.Area.TLX,map.Area.BRX)-map.Area.TLX ||
      got.TLY!=clamp(input.TLY,map.Area.TLY,map.Area.BRY)-map.Area.TLY ||
      got.BRX!=clamp(input.BRX,map.Area.TLX,map.Area.BRX)-map.Area.TLX ||
      got.BRY!=clamp(input.BRY,map.Area.TLY,map.Area.BRY)-map.Area.TLY) return 1;
  }
  C2DBoxI same=tools.ClipAndConvertWorldMapAreaToLocalArea(map,map.Area);
  if(same.TLX||same.TLY||same.BRX!=16||same.BRY!=32)return 2;
 }
 puts("0049c700_TEST PASS");return 0;
}