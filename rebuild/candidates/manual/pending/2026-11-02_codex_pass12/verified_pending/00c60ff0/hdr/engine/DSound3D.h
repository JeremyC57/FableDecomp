#ifndef FABLE_DSOUND3D_H
#define FABLE_DSOUND3D_H
// Public DirectSound SDK POD types; optional D3D_OVERLOADS are disabled.
typedef float D3DVALUE;
typedef struct _D3DVECTOR {
 union { D3DVALUE x; D3DVALUE dvX; };
 union { D3DVALUE y; D3DVALUE dvY; };
 union { D3DVALUE z; D3DVALUE dvZ; };
} D3DVECTOR, *LPD3DVECTOR;
typedef struct _DS3DBUFFER {
 unsigned long dwSize;
 D3DVECTOR vPosition,vVelocity;
 unsigned long dwInsideConeAngle,dwOutsideConeAngle;
 D3DVECTOR vConeOrientation;
 long lConeOutsideVolume;
 D3DVALUE flMinDistance,flMaxDistance;
 unsigned long dwMode;
} DS3DBUFFER,*LPDS3DBUFFER;
#endif
