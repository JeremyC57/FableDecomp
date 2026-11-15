#ifndef CONTINUE_A_EXTERNAL_DEF_POINTEE_CONTRACT_H
#define CONTINUE_A_EXTERNAL_DEF_POINTEE_CONTRACT_H
// Controlled external resource base. Actual shared CIV count, constructor, AddRef,
// Release and CDefPointer execute unchanged. Delete is the external lifetime boundary.
#include "engine/CIVCountedPointeeBase.h"
void CIVCountedPointeeBase::Delete() const { delete this; }
class CDefPointeeBase : public CIVCountedPointeeBase { };
#endif
