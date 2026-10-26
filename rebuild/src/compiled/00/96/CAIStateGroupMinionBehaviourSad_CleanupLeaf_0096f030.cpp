#include "rebuild_abi.h"
class CTCCreatureModeManager { public: public:
    void RemoveMode(unsigned long);   // 0x0073ABA0
};
struct C { char p24[0x24]; CTCCreatureModeManager* f24; char p28[0x3c-0x28]; char f3c; void Cleanup(); };
void C::Cleanup(){ this->f24->RemoveMode(0x14); this->f3c=0; }