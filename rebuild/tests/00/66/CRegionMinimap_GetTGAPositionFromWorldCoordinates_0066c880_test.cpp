// Selected method body with the genuine coordinate and box helper bodies; isolated value fixture.
#include <cstdio>
#include <cassert>
class C3DVector {public:float X,Y,Z;};
class C2DCoordI {public:long X,Y;C2DCoordI(){}C2DCoordI(long x,long y):X(x),Y(y){}};
class C2DExtentsI {public:long X,Y;C2DExtentsI(){}C2DExtentsI(long x,long y):X(x),Y(y){}};
class C2DBoxI {public:long TLX,TLY,BRX,BRY;
 C2DCoordI GetTLPos()const{return C2DCoordI(TLX,TLY);}
 C2DExtentsI GetExtents()const{return C2DExtentsI(BRX-TLX,BRY-TLY);}
};
class CRegionMinimap {public:long RegionMinimapSize;C2DBoxI RegionMapBox,RegionMapSquare;
 void GetRegionMapExtents(C2DCoordI& p,C2DExtentsI& e)const{p=RegionMapBox.GetTLPos();e=RegionMapBox.GetExtents();}
 long GetRegionMinimapSize()const{return RegionMinimapSize;}
 void GetTGAPositionFromWorldCoordinates(const C3DVector&,C2DCoordI&)const;
};
void CRegionMinimap::GetTGAPositionFromWorldCoordinates(const C3DVector& worldCoord,C2DCoordI& tgaPos) const
{
    C2DCoordI regionOrigin;
    C2DExtentsI regionExtents;
    GetRegionMapExtents(regionOrigin,regionExtents);
    long minimapPixelSize=GetRegionMinimapSize();
    tgaPos.X=(long)((worldCoord.X-regionOrigin.X)/RegionMapSquare.GetExtents().X*minimapPixelSize);
    tgaPos.Y=(long)((worldCoord.Y-regionOrigin.Y)/RegionMapSquare.GetExtents().Y*minimapPixelSize);
}

int main(){
 const long sizes[]={1,3,8,32};const long extents[]={2,4,8,16};
 const long origins[]={-16,0,8};
 for(int pixel=0;pixel<4;++pixel)for(int ex=0;ex<4;++ex)for(int ey=0;ey<4;++ey)
 for(int ox=0;ox<3;++ox)for(int oy=0;oy<3;++oy)for(int x=-5;x<=7;++x)for(int y=-5;y<=7;++y){
  CRegionMinimap m;m.RegionMinimapSize=sizes[pixel];
  m.RegionMapBox.TLX=origins[ox];m.RegionMapBox.TLY=origins[oy];
  m.RegionMapBox.BRX=origins[ox]+3;m.RegionMapBox.BRY=origins[oy]+5;
  m.RegionMapSquare.TLX=11;m.RegionMapSquare.TLY=-9;
  m.RegionMapSquare.BRX=11+extents[ex];m.RegionMapSquare.BRY=-9+extents[ey];
  C3DVector p;p.X=float(origins[ox])+float(x)*0.5f;p.Y=float(origins[oy])+float(y)*0.5f;p.Z=47.0f;
  C2DCoordI result;m.GetTGAPositionFromWorldCoordinates(p,result);
  assert(result.X==(long)(x*sizes[pixel])/(2*extents[ex]));
  assert(result.Y==(long)(y*sizes[pixel])/(2*extents[ey]));
  assert(p.X==float(origins[ox])+float(x)*0.5f&&p.Y==float(origins[oy])+float(y)*0.5f&&p.Z==47.0f);
  assert(m.RegionMinimapSize==sizes[pixel]&&m.RegionMapBox.TLX==origins[ox]&&m.RegionMapBox.TLY==origins[oy]);
 }
 std::puts("0066c880_TEST PASS");return 0;
}