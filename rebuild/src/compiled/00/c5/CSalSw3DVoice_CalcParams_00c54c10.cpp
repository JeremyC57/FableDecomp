// CSalSw3DVoice::CalcParams(const CCoda3DSource* pcSource, unsigned int ffUpdate) -- GameCODA software 3D voice: push a 3D source's changed parameters into the voice: pitch (source pitch x Doppler), play-buffer release once the level envelope has released, listener-relative azimuth / elevation, the direct and environment (reverb send) high-frequency filters, the 16-bit environment gain (when the platform has reverb; EAX-software sources send at 0 mB), the MacroFX near-field gains, the renderer position and direct gain, and the two filter cutoffs
// Grade: RELOCATION_MATCH (651 bytes)
// Leaf: ?CalcParams@CSalSw3DVoice@@QAEXPBVCCoda3DSource@@I@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C54C10, 651 bytes, thiscall `ret 8`. Constants 32767.0f (0x0129C820), 2^32 (0x0122DCB4).
// Retail-asm: 0x00C54CFF `fstp dword ptr [esp+14h]` + 0x00C54D03 `fld dword ptr [esp+14h]` + 0x00C54D07 `fistp dword ptr [esp+18h]`
//   -- the environment gain's float-to-int is GameCODA's inline-assembly rounding (SDK name sal_round, the Dbg object's
//   ?sal_round@@YAHM@Z call; the same fld / bare dword fistp block as the shared libpk/FtoL.h, used here): the product homed in
//   the dead pcSource slot and reloaded, stored by fistp into the dead ffUpdate slot; every C++ spelling calls __ftol2, emits
//   fistp qword or cvt(t)ss2si.
// Identity: GameCODA 2.10 CODAApi.lib member SalSw3DVoice.obj, ?CalcParams@CSalSw3DVoice@@QAEXPBVCCoda3DSource@@I@Z (also
//   Codex pass 22 NAME_FIXES line 283): the release object's body has the same 651 bytes and instruction sequence; only member
//   displacements, globals and call targets differ (this native GameCODA fork: CSalBaseVoice +8 (m_pEAXSource), six-channel
//   history +0x80, CCodaContext / CCodaEAX without the object-list links), so the row is authored, not a library copy. Caller:
//   CSalSwBaseVoice::CalcParams 0x00C5ACB0 (landed). Names, access and the statement order: the CODAApiDbg.lib object's CodeView
//   types and its call relocations (GetParams, Get3DParams, SetPitch, IsLevelEnvReleased, ReleasePlayBuffer, CalcAzimuthElevation,
//   SetLowPass, GetReverb, GetEAX, IsHardwareEAX, GetCaps, sal_levelToGain, sal_round, GetHrtfMode, GetMacroFxFactor,
//   GetDistanceUnits, SAL_CalcMacroFxGains, IsStarting, SetLP, ClearHistory, SetEQ); the operand order of each product is the
//   Dbg object's evaluation order. Overlay opt-in FABLE_RETAIL_CSALSW3D_CALCPARAMS (accessors and declarations, hdr/README.md).
// Callees: CSalSwBaseVoice::SetPitch 0x00C5A500, CSalSwBuffer::Release 0x00C5CE40, CSalBaseVoice::CalcAzimuthElevation 0x00C599C0,
//   CSalIIRFilter::SetLowPass 0x00C6C000 (x2), sal_levelToGain 0x00C54AE0 (x2), SAL_CalcMacroFxGains 0x00C6BE60, CSalRender::
//   SetPosition (slot +0x10), CSalSwFilter::SetLP 0x00C6BC80 / SetEQ 0x00C6BD00 / ClearHistory 0x00C6B720 (x2). Globals: g_pSal
//   0x0143BC2C, CODA 0x0143BB58 (m_EAX +0x18 = 0x0143BB70, m_fDistanceUnits 0x0143BBE8, m_fMacroFxFactor 0x0143BBF4).
// Inferred: the local names (the Dbg object carries no S_ symbol records); the HRTF-mode value 0x2000 (the voice's internal
//   m_ffParams mode field, C_HRTF_VOLPAN >> 9; no SDK header names it); the Dbg build's three SAL_ASSERTs (pcSource, g_pSal,
//   m_pRender), compiled out of the release, are not written.
#define FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS
#define FABLE_RETAIL_PASS22_CCODA3D_CREATE
#define FABLE_RETAIL_CSALPLATFORM_CREATE_SW_PASS23
#define FABLE_RETAIL_CSALSW3D_CALCPARAMS
#include "libpk/FtoL.h"
#include "engine/CSalSw3DVoice.h"
#include "engine/CCoda3DSource.h"
#include "engine/CCodaContext.h"
#include "engine/CSalPlatform.h"
#include "engine/CCodaEAX.h"
#include "engine/CSalRender.h"
#include "engine/CodaCaps.h"

float __cdecl sal_levelToGain(int iLevel);                                                          // 0x00C54AE0
void __cdecl SAL_CalcMacroFxGains(float fAzimuth, float fElevation, float fDistance, float fMacroFx, float* pfGains);   // 0x00C6BE60

void CSalSw3DVoice::CalcParams(const CCoda3DSource* pcSource, unsigned int ffUpdate)
{
    const CODASOURCE& params = pcSource->GetParams();
    const CODA3DSOURCE& params3D = pcSource->Get3DParams();
    if (ffUpdate & 0x10074)
        SetPitch(params.fPitch * params3D.fDoppler, params);
    if (IsLevelEnvReleased())
        ReleasePlayBuffer();
    if (ffUpdate & 0x8000)
        CalcAzimuthElevation(m_fAzimuth, m_fElevation, params3D.vDirection);
    if (ffUpdate & 0x400000)
        m_filterDirect.SetLowPass(params3D.iDirectHF, (float)params3D.uiDirectHFReference);
    if (g_pSal->GetReverb()) {
        CCodaEAX* pEAX = CODA.GetEAX();
        float fGain;
        if (pEAX && !pEAX->IsHardwareEAX() && (pcSource->GetCaps() & C_CAPS_EAX))
            fGain = params.fGain * m_fLevelEnvGain * m_fMultiLFOGain * sal_levelToGain(0);
        else
            fGain = params.fGain * m_fLevelEnvGain * m_fMultiLFOGain * sal_levelToGain(params3D.iEnvFx);
        unsigned short gain = (unsigned short)FtoL(fGain * 32767.0f);
        if (gain != m_gainEnvironmentNew) {
            m_gainEnvironmentNew = gain;
            m_bUpdateEnvironmentGain = true;
        }
        if (ffUpdate & 0x1000000)
            m_filterEnvironment.SetLowPass(params3D.iEnvFxHF, (float)params3D.uiEnvFxHFReference);
    }
    if ((ffUpdate & 0x800000) && GetHrtfMode() != 0x2000)
        SAL_CalcMacroFxGains(m_fAzimuth, m_fElevation, CODA.GetDistanceUnits() * params3D.fDistance, CODA.GetMacroFxFactor() * params3D.fMacroFx, m_fMacroFXGain);
    float fGain = params.fGain * m_fLevelEnvGain * m_fMultiLFOGain * params3D.fConeGain * params3D.fDistanceGain * sal_levelToGain(params3D.iDirect);
    m_pRender->SetPosition(m_fAzimuth, m_fElevation, CODA.GetDistanceUnits() * params3D.fDistance, fGain, m_fMacroFXGain, !IsStarting(), false);
    if (ffUpdate & 0x1d0) {
        float fCutOff = m_fMultiEnvCutoff * m_fMultiLFOCutoff;
        m_ffStatus &= ~0x1e0000;
        if (params.pLPFilter) {
            m_Filter[0].SetLP(params.pLPFilter, fCutOff);
            m_ffStatus |= 0x20000;
        } else
            m_Filter[0].ClearHistory();
        if (params.pEQFilter) {
            m_Filter[1].SetEQ(params.pEQFilter, fCutOff);
            m_ffStatus |= 0x100000;
        } else
            m_Filter[1].ClearHistory();
    }
}