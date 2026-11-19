#ifndef FABLE_CCODAEAX_H
#define FABLE_CCODAEAX_H
#include "engine/CCodaObjectBase.h"
#include "engine/CodaEnvironment.h"
#include "engine/EAXNativeTypes.h"
// GameCODA 2.10 EAX context (hand4c w24, row 0x00C54C10). Class, member, enum and method names: the SDK CODAApiDbg.lib
// CodeView (SalSw3DVoice object). Native layout (0xf0 bytes, CodaMalloc(0F0h) at 0x00C3DF28): the SDK class less the
// CCodaObject list links, as the native CCodaContext: ctor 0x00C4E2A0 (vtable 0x012B87F4) stores m_pEnvFX at +0x04,
// m_iMode at +0xe8 and m_pSources at +0xec; 0x00C52840 copies the 0x6c-byte _CENVIRONMENT to +0x08; 0x00C4E2D0 releases
// m_pEAXListener at +0xe4. The SDK release object has the same members 8 bytes higher.
class CCodaEnvironment;
class CEAXListener;
struct EAXSOURCE_tag;
class CCodaEAX:public CCodaObjectBase {
public:
 enum EAX_MODE { EAX_SW, EAX_HW };
#ifdef FABLE_RETAIL_CSALSW3D_CALCPARAMS
 // SDK public ?IsHardwareEAX@CCodaEAX@@QAE_NXZ; release 0x00C54CC9 `cmp dword ptr [eax+0E8h],1`.
 bool IsHardwareEAX(){return m_iMode==EAX_HW;}
#endif
private:
 CCodaEnvironment* m_pEnvFX;                  // +0x04
 _CENVIRONMENT m_EnvState;                    // +0x08
 EAX30LISTENERPROPERTIES m_EAX3State;         // +0x74
 CEAXListener* m_pEAXListener;                // +0xe4
 EAX_MODE m_iMode;                            // +0xe8
 EAXSOURCE_tag* m_pSources;                   // +0xec
};
#endif
