#ifndef FABLE_ENGINE_DIRECTINPUTTYPES_H
#define FABLE_ENGINE_DIRECTINPUTTYPES_H
// Microsoft DirectInput8 dinput.h and both original PDB DIDEVICEOBJECTDATA records: 20 bytes on Win32.
struct IDirectInputDevice8W;
struct DIDEVICEOBJECTDATA { unsigned long dwOfs,dwData,dwTimeStamp,dwSequence,uAppData; };
#endif
