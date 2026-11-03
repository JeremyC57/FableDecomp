#ifndef FABLE_ENGINE_C3DBOUNDINGBOX_H
#define FABLE_ENGINE_C3DBOUNDINGBOX_H
#include "engine/C3DVector.h"
// Layout: Ego_r TPI C3DBoundingBox has precisely two C3DVector members;
// ReadMapInfoBlock writes all 24 bytes at block+0x40, caller copies six words.
class C3DBoundingBox {
#ifdef FABLE_RETAIL_MESH_PARTITION
public:
 void BuildFromPoints(const std::vector<C3DVector>&);
 const C3DVector GetCenter()const{return (MinCoord+MaxCoord)*0.5f;}
#endif
private:
    C3DVector MinCoord;
    C3DVector MaxCoord;
};
#endif
