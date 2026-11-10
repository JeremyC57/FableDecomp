#include <cstdio>
#include <list>
#include <utility>
class C3DVector { public:float X,Y,Z;};
class CEnginePrimitiveWeaponTrail {public:class CWeaponTrailEntry {public:C3DVector Top,Bottom;float CreationTime;};};
class CTCWeaponTrail {public:std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> ProjectileWeaponTrail;void SetLastProjectileWeaponTrailPoint(const C3DVector& point);};
static int calls;
static std::pair<C3DVector,C3DVector> GetProjectileTrailElementFromPoint(const C3DVector& p){++calls;C3DVector a=p,b=p;a.Y-=0.0625f;b.Y+=0.0625f;return std::make_pair(a,b);}
void CTCWeaponTrail::SetLastProjectileWeaponTrailPoint(const C3DVector& point)
{
    std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>::reverse_iterator rit=ProjectileWeaponTrail.rbegin();
    if(rit!=ProjectileWeaponTrail.rend()) {
        CEnginePrimitiveWeaponTrail::CWeaponTrailEntry& trail_entry=*rit;
        std::pair<C3DVector,C3DVector> desired_element=GetProjectileTrailElementFromPoint(point);
        trail_entry.Top=desired_element.first;
        trail_entry.Bottom=desired_element.second;
    }
}

static int same(const C3DVector& a,const C3DVector& b){return a.X==b.X&&a.Y==b.Y&&a.Z==b.Z;}
int main(){int ok=1,cases=0;for(int n=0;n<7;++n)for(int sample=0;sample<9;++sample)for(int alias=0;alias<2;++alias){CTCWeaponTrail trail;for(int i=0;i<n;++i){CEnginePrimitiveWeaponTrail::CWeaponTrailEntry e;e.Top.X=(float)(i+1);e.Top.Y=(float)(i+2);e.Top.Z=(float)(i+3);e.Bottom=e.Top;e.Bottom.Z=-e.Top.Z;e.CreationTime=(float)(30+i);trail.ProjectileWeaponTrail.push_back(e);}std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> before=trail.ProjectileWeaponTrail;C3DVector point;point.X=(float)(sample-4);point.Y=(float)(sample*2);point.Z=(float)(sample-7);const C3DVector& argument=(alias&&n)?trail.ProjectileWeaponTrail.back().Top:point;C3DVector expected=argument;int old_calls=calls;trail.SetLastProjectileWeaponTrailPoint(argument);ok &= calls==old_calls+(n!=0);ok &= trail.ProjectileWeaponTrail.size()==before.size();std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>::const_iterator it=trail.ProjectileWeaponTrail.begin(),old=before.begin();for(int i=0;i<n;++i,++it,++old){ok &= it->CreationTime==old->CreationTime;if(i+1==n){C3DVector a=expected,b=expected;a.Y-=0.0625f;b.Y+=0.0625f;ok &= same(it->Top,a)&&same(it->Bottom,b);}else ok &= same(it->Top,old->Top)&&same(it->Bottom,old->Bottom);}++cases;}ok &= cases==126;std::printf(ok?"0076e290_TEST PASS\n":"0076e290_TEST FAIL\n");return ok?0:1;}