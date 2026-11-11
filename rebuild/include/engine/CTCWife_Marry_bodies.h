#ifndef FABLE_ENGINE_CTCWIFE_MARRY_BODIES_H
#define FABLE_ENGINE_CTCWIFE_MARRY_BODIES_H
// Original FableWin02485690,0248a070 and native006d03e0 inline owner helpers.
inline void CTCWife::SendPauseEvent(bool pause)
{
 CGameEvent event(9);
 event.SetPlayer(PeekWorld().PeekMyPlayer()->GetNumber());
 event.AddUBYTE(pause?1:0);
 CMainGameComponent::Get()->AddGameEvent(event);
}
inline void CTCWife::InitWifeNonpersistentState()
{
 if(FrameGotMarriedToThePlayer<=CWorld::GetFrame()){
  SetAttractionIcon(true,0.5f,255);
  NeedsToChangeBrain=true;
 }
}
#endif
