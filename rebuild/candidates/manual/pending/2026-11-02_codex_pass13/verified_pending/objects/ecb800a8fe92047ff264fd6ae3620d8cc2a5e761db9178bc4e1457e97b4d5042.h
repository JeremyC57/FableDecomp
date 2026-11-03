#ifndef FABLE_ENGINE_C2DEXTENTSF_RENDER2D_H
#define FABLE_ENGINE_C2DEXTENTSF_RENDER2D_H
#include "rebuild_abi.h"

// Original Ego_r and FableWin declarations: evidence/009da9f0_*_types.json and *_dependencies.json.
class C2DExtentsF {
public: float X;
public: float Y;
public: C2DExtentsF(const C2DExtentsF &);
public: C2DExtentsF(float,float);
public: C2DExtentsF();
public: void Initialise(float,float);
};
#endif
