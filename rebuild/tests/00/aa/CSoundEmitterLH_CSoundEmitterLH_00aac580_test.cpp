#include <cstdio>
#include <cstdlib>
#include <cstring>
struct C3DVector { float X, Y, Z; };
struct CSoundManagerLH { long EmitterCounter; long TakeNextEmitterNumber() { return EmitterCounter++; } };
namespace N_0099a2d0 { struct CBase { virtual void v00() { } CBase() { } }; } using N_0099a2d0::CBase;
struct SListNode_00aac580 { SListNode_00aac580* m_pNext; SListNode_00aac580* m_pPrev; long m_a; long m_b; };
struct CRetailList_00aac580 { SListNode_00aac580* m_pHead; CRetailList_00aac580() { m_pHead = 0; SListNode_00aac580* n = (SListNode_00aac580*)std::malloc(sizeof(SListNode_00aac580)); n->m_pNext = n; n->m_pPrev = n; m_pHead = n; } };
struct CSoundEmitterLH : CBase { CSoundManagerLH* m_pManager; float m_positionX, m_positionY, m_positionZ; CRetailList_00aac580 m_sounds; bool m_active, m_paused, m_finished; long m_number;
    CSoundEmitterLH(CSoundManagerLH* t, const C3DVector& p) : CBase(), m_pManager(t), m_positionX(p.X), m_positionY(p.Y), m_positionZ(p.Z), m_sounds(), m_active(true), m_paused(false), m_finished(false), m_number(m_pManager->TakeNextEmitterNumber()) { } };
int main() {
    CSoundManagerLH t; t.EmitterCounter = 77; C3DVector v = { 4, 5, 6 }; CSoundEmitterLH e(&t, v);
    if (e.m_pManager != &t || t.EmitterCounter != 78 || e.m_positionY != 5 || !e.m_sounds.m_pHead || e.m_sounds.m_pHead->m_pNext != e.m_sounds.m_pHead || e.m_sounds.m_pHead->m_pPrev != e.m_sounds.m_pHead || !e.m_active || e.m_paused || e.m_finished || e.m_number != 77) { std::printf("00aac580_TEST FAIL\n"); return 1; }
    std::free(e.m_sounds.m_pHead);
    std::printf("00aac580_TEST PASS\n"); return 0;
}