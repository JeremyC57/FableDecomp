#include <stdio.h>
#include <math.h>
struct C3DVector {
    float X,Y,Z;
    C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
    C3DVector operator+(const C3DVector& b) const{return C3DVector(X+b.X,Y+b.Y,Z+b.Z);}
    C3DVector operator-(const C3DVector& b) const{return C3DVector(X-b.X,Y-b.Y,Z-b.Z);}
    C3DVector operator*(float k) const{return C3DVector(X*k,Y*k,Z*k);}
    float Dot(const C3DVector& v) const{return X*v.X+Y*v.Y+Z*v.Z;}
    float GetAccurateMagnitude() const{return (float)sqrt(Dot(*this));}
};
const float DELTA_NOTIONAL_ZERO=0.0001f;
struct CCombatTypeDef {float PreferredCombatDistance, FrontFarLimit, FrontMiddleLimit; int edge;};
struct CThing {C3DVector pos; bool dead; const C3DVector& GetPos() const{return pos;}};
struct CTCPhysicsBase{float radius;float GetRadius()const{return radius;}};
struct CThingCreatureBase: CThing {
    CCombatTypeDef def; CTCPhysicsBase phys;
    const CCombatTypeDef& GetCombatTypeDef()const{return def;}
    const CTCPhysicsBase* GetPhysicsTC()const{return &phys;}
};
struct CDefendingCombatantInfo {
    struct CProng {
        CThingCreatureBase* occupant;C3DVector direction;
        CThingCreatureBase* GetPOccupyingAttacker()const{return occupant;}
        C3DVector GetProngVector(float)const{return direction;}
    };
    struct CCombatRing {
        int index;bool IsOuterRing(const CCombatTypeDef& d)const{return index>=d.edge;}
        int GetIndex()const{return index;}float GetRotationAngle()const{return 0;}
    };
    struct CCombatWheel {float GetRingRadius(int n)const{return (float)(n+5);}};
};
struct CAttackingCombatantInfo {bool PreferredAttackDollySet; C3DVector PreferredAttackDollyVector;};
struct CWorldMap {};
bool IS_THING_ALIVE(const CThingCreatureBase* m){return m && !m->dead;}
double GFCos(float a){return cos(a*(double)6.2831855f);}
float GetAttackerProngSelectionCurrentWeaponBaseCompareValue(const CThing&, bool outer){return outer?0.75f:0.25f;}
struct CGetBestFreeProngForAttacker {
    const CWorldMap& WorldMap;const CThingCreatureBase& Attacker;const CThingCreatureBase& Defender;
    const CAttackingCombatantInfo& AttackerCombatInfo;const CDefendingCombatantInfo::CCombatRing& Ring;
    const CDefendingCombatantInfo::CCombatWheel& Wheel;float BestCompareVal; CDefendingCombatantInfo::CProng* PBestProng;
    void operator()(CDefendingCombatantInfo::CProng&);
};
float GetPreferredCombatDistanceForAttacker(const CThingCreatureBase& defender, const CThingCreatureBase& attacker)
{
    float separation = defender.GetPhysicsTC()->GetRadius();
    const CCombatTypeDef& combat_def = attacker.GetCombatTypeDef();
    float distance = combat_def.PreferredCombatDistance;
    if (distance < DELTA_NOTIONAL_ZERO) {
        float far_range = combat_def.FrontFarLimit;
        float middle_range = combat_def.FrontMiddleLimit;
        distance = (middle_range + far_range) / 2.0f;
    }
    separation += distance;
    return separation;
}
// Fidelity: same-TU strong definition of 0x006E7F00 (retail TU-mate)
float GetAttackerProngSelectionCompareValue(const C3DVector& attackerPos, const C3DVector& defenderPos,
                                                       const C3DVector& prongVector, const C3DVector& placementVector)
{
    C3DVector flatPlacement(placementVector.X, placementVector.Y, 0.0f);
    C3DVector flatProng(prongVector.X, prongVector.Y, 0.0f);
    float lengthSquared = flatPlacement.X * flatPlacement.X + flatPlacement.Y * flatPlacement.Y + flatPlacement.Z * flatPlacement.Z;
    if (lengthSquared < 0.0001f)
        return 0.0001f;
    float inverse = 1.0f / (float)sqrt(lengthSquared);
    flatPlacement.X = inverse * flatPlacement.X;
    flatPlacement.Y = inverse * flatPlacement.Y;
    flatPlacement.Z = inverse * flatPlacement.Z;
    lengthSquared = flatProng.X * flatProng.X + flatProng.Y * flatProng.Y + flatProng.Z * flatProng.Z;
    if (lengthSquared < 0.0001f)
        return 0.0001f;
    inverse = 1.0f / (float)sqrt(lengthSquared);
    flatProng.X = inverse * flatProng.X;
    flatProng.Y = inverse * flatProng.Y;
    flatProng.Z = inverse * flatProng.Z;
    return flatProng.Z * flatPlacement.Z + (flatProng.X * flatPlacement.X + flatProng.Y * flatPlacement.Y) + 1.0f;
}
bool IsPosValid(const CWorldMap&, const CThing&, const CThing&, const C3DVector&)
{
    return true;
}
float GetAttackerProngSelectionNavigtionBasedCompareValue(
    const CThing&, const CThing&, const C3DVector&)
{
    return 0.0f;
}
void CGetBestFreeProngForAttacker::operator()(CDefendingCombatantInfo::CProng& prong)
{
    if (IS_THING_ALIVE(prong.GetPOccupyingAttacker()))
        return;
    bool outer_ring = false;
    float preferred_distance = GetPreferredCombatDistanceForAttacker(Defender, Attacker);
    if (Ring.IsOuterRing(Defender.GetCombatTypeDef())) {
        outer_ring = true;
        preferred_distance = Wheel.GetRingRadius(Ring.GetIndex());
    }
    C3DVector my_line = Attacker.GetPos() - Defender.GetPos();
    my_line.Z = 0.0f;
    float separation = my_line.GetAccurateMagnitude();
    C3DVector prong_line = prong.GetProngVector(Ring.GetRotationAngle()) * preferred_distance;
    prong_line.Z = 0.0f;
    C3DVector prong_pos = Defender.GetPos() + prong_line;
    bool my_prong = false;
    if (!AttackerCombatInfo.PreferredAttackDollySet &&
        prong_line.Dot(my_line) / (separation * preferred_distance) > GFCos(0.25f / 360.0f) &&
        separation < preferred_distance)
        my_prong = true;
    if (IsPosValid(WorldMap, Attacker, Defender, prong_pos) || my_prong) {
        if (my_prong) {
            BestCompareVal = 1.0e10f;
            PBestProng = &prong;
        } else {
            if (AttackerCombatInfo.PreferredAttackDollySet) {
                my_line.X = AttackerCombatInfo.PreferredAttackDollyVector.X;
                my_line.Y = AttackerCombatInfo.PreferredAttackDollyVector.Y;
            }
            float compare_val = 0.0f;
            compare_val += GetAttackerProngSelectionCompareValue(Defender.GetPos(), Attacker.GetPos(), prong_line, my_line);
            compare_val += GetAttackerProngSelectionNavigtionBasedCompareValue(Attacker, Defender, prong_pos);
            compare_val += GetAttackerProngSelectionCurrentWeaponBaseCompareValue(Attacker, outer_ring);
            if (compare_val > BestCompareVal) {
                BestCompareVal = compare_val;
                PBestProng = &prong;
            }
        }
    }
}

int main(){
    CWorldMap world;CThingCreatureBase attacker,defender,occupant;CDefendingCombatantInfo::CCombatRing ring;
    CDefendingCombatantInfo::CCombatWheel wheel; CDefendingCombatantInfo::CProng prong,old;
    CAttackingCombatantInfo info;
    defender.pos=C3DVector(-2,1,2); defender.phys.radius=1;defender.def.edge=1;
    attacker.def.FrontFarLimit=4;attacker.def.FrontMiddleLimit=2;
    const float best_values[]={-1.0f,0.5f,2.25f,3.0f,2.0e10f};
    const float distances[]={0.0f,0.5f,2.0f,3.0f,4.0f,6.0f,8.0f};
    for(int occupied=0;occupied<3;++occupied)for(int outer=0;outer<2;++outer)
    for(int fallback=0;fallback<2;++fallback)for(int dolly=0;dolly<3;++dolly)
    for(int dir=0;dir<4;++dir)for(int d=0;d<7;++d)for(int b=0;b<5;++b) {
        occupant.dead=occupied==2;prong.occupant=occupied?&occupant:0;ring.index=outer;
        attacker.def.PreferredCombatDistance=fallback?0.0f:2.0f;
        attacker.pos=defender.pos+C3DVector(distances[d],0,7);
        prong.direction=C3DVector(dir==0?1.0f:dir==1?-1.0f:0.0f,dir==2?1.0f:0.0f,5);
        info.PreferredAttackDollySet=dolly!=0;
        info.PreferredAttackDollyVector=C3DVector(-1.0f,0,0)*(float)(dolly==1);
        CGetBestFreeProngForAttacker score={world,attacker,defender,info,ring,wheel,best_values[b],&old};
        score(prong);
        float expected=best_values[b];bool change=false;
        if(occupied!=1) {
            float range=outer?6.0f:fallback?4.0f:3.0f;
            if(!dolly && d!=0 && dir==0 && distances[d]<range) {
                change=true;expected=1.0e10f;
            } else {
                float cosine;
                if(dir==3 || dolly==2 || (dolly==0 && d==0))cosine=0.0001f;
                else if(dir==2)cosine=1.0f;
                else cosine=1.0f+(dir==0?1.0f:-1.0f)*(dolly==0?1.0f:-1.0f);
                float value=cosine+(outer?0.75f:0.25f);
                if(value>expected){expected=value;change=true;}
            }
        }
        if(score.PBestProng!=(change?&prong:&old) || fabs(score.BestCompareVal-expected)>0.00001)return 1;
    }
    puts("006f2c00_TEST PASS");return 0;
}