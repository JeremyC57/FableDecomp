#ifndef FABLE_ENGINE_C3DBOUNDINGBOX_H
#define FABLE_ENGINE_C3DBOUNDINGBOX_H
#include "engine/C3DVector.h"
// Layout: Ego_r TPI C3DBoundingBox has precisely two C3DVector members;
// ReadMapInfoBlock writes all 24 bytes at block+0x40, caller copies six words.
class C3DBoundingBox {
private:
    C3DVector MinCoord;
    C3DVector MaxCoord;
};
#endif
