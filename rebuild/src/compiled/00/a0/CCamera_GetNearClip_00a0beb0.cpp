#include "rebuild_abi.h"
// CCamera::GetNearClip @ 0x00a0beb0
// fld [ecx+0x2c]; ret  -> return this->f2c (float)
struct CCamera { char pad00[0x2c]; float m_seaLevel; float GetNearClip(); };
float CCamera::GetNearClip(){ return m_seaLevel; }