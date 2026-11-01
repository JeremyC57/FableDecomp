#include "rebuild_abi.h"
class CThing { public:
    void Kill(bool a0);   // 0x004C9B80 (Ego_r.pdb ?Kill@CThing@@QAEX_N@Z)
};
struct C {  };
extern "C" void FABLE_STDCALL Dest_5e9a60(C* p){ ((CThing*)p)->Kill(1); }