// 00871fd0 NCameraModes::CStringAndCageBase::UpdateXYVars
// Codex parked-50 best attempt: DIFFER(124v124); residue: 124/124 bytes; desired-radius FLD and prior smoothing-result MOV are interchanged.
// Flags:
// Leaf: ?UpdateXYVars@CStringAndCageBase@NCameraModes@@AAEXXZ
namespace NCameraModes {
class CTools {
public:
 template<class T> T GetRollingAverage(const T& current,const T& target,float weight)const {
  return (current*(weight-1.0f)+target)/weight;
 }
};
// Layout: native CBase size0xa4, donor0xc8 before container shrink; unread body retained.
class CBase {
 unsigned char unreadBase[0xa0];
public:
 virtual ~CBase();
protected:
 const CTools& GetCamTools()const;
};
// Layout: donor private fields +0xc8..0xe8 shift down0x24 in native ctor0069de30.
class CStringAndCageBase:public CBase {
 float DesiredHeightOffset,DesiredStringLength,DesiredCageRadius;
 float CurrentHeightOffset,CurrentStringLength,CurrentCageRadius;
 long NextAutoGoBehindWF,TimerForExpansion;
 bool DriftToMaxStringLength;
 void UpdateXYVars();
};
void CStringAndCageBase::UpdateXYVars() {
 const float& length=GetCamTools().GetRollingAverage(CurrentStringLength,DesiredStringLength,4.0f);
 float target=DesiredCageRadius;
 CurrentStringLength=length;
 if(TimerForExpansion>0) { --TimerForExpansion;target=DesiredStringLength; }
 CurrentCageRadius=GetCamTools().GetRollingAverage(CurrentCageRadius,target,4.0f);
}
}
