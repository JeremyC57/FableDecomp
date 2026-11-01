#include "rebuild_abi.h"
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
class CAIStateGroupBase { protected:   // engine/CAIStateGroupBase.h flattens the vptr: the getter only
    CWorld& GetWorld() const;   // 0x008FCFC0 (Ego_r.pdb ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ)
};
struct C : public CAIStateGroupBase { char p[0x3c]; int f3c; char Valid();  };
char C::Valid(){ GetWorld(); int b=CWorld::GetFrame(); return (char)(b <= this->f3c); }