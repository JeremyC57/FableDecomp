#include <stdio.h>
#include <string.h>
// CEaxSource::SetRoomHF(long value) -- Validate an EAX source level and mark the current properties dirty when changed.
// Leaf: ?SetRoomHF@CEaxSource@@QAEJJ@Z
// Identity: own callers00c67e44 / 00c6864f select SDK EAXSOURCEPROPERTIES.lRoomHF, current field36. Original SDK property name recovered; libpk owner name descriptive established binding.
// Layout: no active shared header. Actual dirty4, context20, SDK properties24; current field36. Error field980 in owning context. Deferred properties520 remain untouched here.
struct CEaxContext {                           // the owning EAX context (landed 0x00C630D0 models its body)
    unsigned char _pad000[0x3d4];
    long          m_lastError;                   // +0x3d4
};
struct SEAXSourceProperties {                    // EAX 4.0 EAXSOURCEPROPERTIES, 0x48 bytes
    long          lDirect;                       // +0x00 -10000..1000 mB
    long          lDirectHF;                     // +0x04 -10000..0
    long          lRoom;                         // +0x08 -10000..1000
    long          lRoomHF;                       // +0x0c -10000..0
    long          lObstruction;                  // +0x10 -10000..0
    float         flObstructionLFRatio;          // +0x14
    long          lOcclusion;                    // +0x18
    float         flOcclusionLFRatio;            // +0x1c
    float         flOcclusionRoomRatio;          // +0x20
    float         flOcclusionDirectRatio;        // +0x24
    long          lExclusion;                    // +0x28
    float         flExclusionLFRatio;            // +0x2c
    long          lOutsideVolumeHF;              // +0x30 -10000..0
    float         flDopplerFactor;               // +0x34
    float         flRolloffFactor;               // +0x38
    float         flRoomRolloffFactor;           // +0x3c
    float         flAirAbsorptionFactor;         // +0x40
    unsigned long ulFlags;                       // +0x44 EAXSOURCEFLAGS_* (bits 0..2)
};
struct CEaxSource {
    unsigned char        _pad00[0x4];
    unsigned long        m_dirty;                // +0x04 one bit per changed property
    unsigned char        _pad08[0x14 - 0x8];
    CEaxContext*         m_pContext;             // +0x14 (last error)
    SEAXSourceProperties m_current;              // +0x18
    unsigned char        _pad60[0x208 - 0x60];
    SEAXSourceProperties m_deferred;             // +0x208 (copy the update pass reads)
    long SetRoomHF(long);long SetOcclusion(long);long SetExclusion(long);
};

long CEaxSource::SetRoomHF(long value){if(value < -10000 || value > 0){m_pContext->m_lastError=-2;return -3;}if(value!=m_current.lRoomHF){m_current.lRoomHF=value;m_dirty|=0x400000;}return 0;}
int main(){CEaxContext context;CEaxSource source;memset(&source,0,sizeof(source));source.m_pContext=&context;source.m_current.lRoomHF=-500;source.m_deferred.lRoomHF=-250;source.m_dirty=0x20;context.m_lastError=13;bool ok=source.SetRoomHF(-500)==0&&source.m_dirty==0x20&&context.m_lastError==13;ok=ok&&source.SetRoomHF(-10001)==-3&&context.m_lastError==-2&&source.m_current.lRoomHF==-500&&source.m_dirty==0x20;context.m_lastError=17;ok=ok&&source.SetRoomHF(1)==-3&&context.m_lastError==-2&&source.m_current.lRoomHF==-500;context.m_lastError=19;ok=ok&&source.SetRoomHF(-10000)==0&&source.m_current.lRoomHF==-10000&&source.m_dirty==0x400020&&context.m_lastError==19;source.m_dirty=0;ok=ok&&source.SetRoomHF(0)==0&&source.m_current.lRoomHF==0&&source.m_dirty==0x400000&&source.m_deferred.lRoomHF==-250;puts(ok?"00c64a80_TEST PASS":"00c64a80_TEST FAIL");return ok?0:1;}