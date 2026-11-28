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
#ifdef FABLE_RETAIL_CCODAEAX_SOURCE_PROPERTY
#include "engine/CodaProcessingTypes.h"
// GameCODA debug type EAXSOURCE_tag. Retail omits the SDK EAX20BP block.
struct EAXSOURCE_tag {
    EAXSOURCEPROPERTIES eax4SourceProp;
    EAXSOURCESENDPROPERTIES eax4SourceSendProp[4];
    EAXSOURCEOCCLUSIONSENDPROPERTIES eax4SourceOcclusionSendProp[4];
    EAXSOURCEEXCLUSIONSENDPROPERTIES eax4SourceExclusionSendProp[4];
    GUID eax4SourceActiveFXSlotID[2];
    EAX30BUFFERPROPERTIES EAX30BP;
    void* lpEAXBuffer;
    _C3DSOURCEENVFX CODA_Environment;
    void* CODA_hSource;
    EAXSOURCE_tag* pPrevious;
    EAXSOURCE_tag* pNext;
};
#endif
class CCodaEAX:public CCodaObjectBase {
#ifdef FABLE_RETAIL_CCODAEAX_VOICE_SET_PASS30
    // Inferred: the native CSalBaseVoice member calls private SetEAX4Source directly.
    friend class CSalBaseVoice;
#endif
public:
#ifdef FABLE_RETAIL_CCODAEAX_SOURCE_BINDERS
    int Create();
#endif
 enum EAX_MODE { EAX_SW, EAX_HW };
#ifdef FABLE_RETAIL_CSALSW3D_CALCPARAMS
 // SDK public ?IsHardwareEAX@CCodaEAX@@QAE_NXZ; release 0x00C54CC9 `cmp dword ptr [eax+0E8h],1`.
 bool IsHardwareEAX(){return m_iMode==EAX_HW;}
#endif
private:
#ifdef FABLE_RETAIL_CCODAEAX_SOURCE_PROPERTY
    // CODAApi and CODAApiDbg symbols, confirmed by the retail switch and helper calls.
    int SetEAX4Buffer(unsigned int, EAXSOURCE_tag*, void*, unsigned int, unsigned int);
    int SetEAX4Source(unsigned int, void*, unsigned int, EAXSOURCE_tag*);
    void ProcessChanges(_CHANGES*, EAXSOURCE_tag*);
#endif
#ifdef FABLE_RETAIL_CCODAEAX_SOURCE_BINDERS
    int CreateEAXListener();
    int EAXSet(const GUID*, unsigned int, EAXSOURCE_tag*, void*, unsigned int);
    int SetEAX4FXSlot(unsigned int, unsigned int, void*, unsigned int, EAXSOURCE_tag*);
#endif
 CCodaEnvironment* m_pEnvFX;                  // +0x04
 _CENVIRONMENT m_EnvState;                    // +0x08
 EAX30LISTENERPROPERTIES m_EAX3State;         // +0x74
 CEAXListener* m_pEAXListener;                // +0xe4
 EAX_MODE m_iMode;                            // +0xe8
 EAXSOURCE_tag* m_pSources;                   // +0xec
};
#ifdef FABLE_RETAIL_CODA_EAX_SET_BUFFER_INLINES_PASS30
#include "engine/CEAXBuffer.h"
inline int CCodaEAX::SetEAX4Source(unsigned int ulEAX4SourceProperty, void* lpValue,
                            unsigned int ulDataLength, EAXSOURCE_tag* source)
{
    int Error = -4;
    EAXSOURCE_tag* ALSource = source;
    if (ALSource) {
        _CHANGES* lpChanges;
        Error = static_cast<CEAXBuffer*>(ALSource->lpEAXBuffer)->EAX4Source(
            ulEAX4SourceProperty, lpValue, ulDataLength, &lpChanges);
        if (Error == 0)
            ProcessChanges(lpChanges, ALSource);
    }
    return Error;
}
#endif
#endif
