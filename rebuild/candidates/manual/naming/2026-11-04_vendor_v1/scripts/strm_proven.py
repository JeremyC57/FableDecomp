# strm_proven.py: CBaseRenderer / CBaseVideoRenderer primary-vtable slot names (renbase.h + amfilter.h declaration order) -> proven.tsv rows
import sys,os
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from vcommon import *
sys.path.insert(0,'/home/user/FableDecomp/rebuild/candidates/manual/naming/2026-11-03_wave33/scripts')
from pe import PE
p=PE(os.environ['FABLE_EXE']); cat=catalog()
IMS='PAUIMediaSample@@'
R={ # slot: (method, CBaseRenderer-level owner, args-mangle)
 4:('StreamTime','CBaseFilter','UAEJAAVCRefTime@@@Z'),5:('GetPinVersion','CBaseFilter','UAEJXZ'),6:('GetPinCount','CBaseRenderer','UAEHXZ'),
 7:('GetPin','CBaseRenderer','UAEPAVCBasePin@@H@Z'),9:('GetMediaPositionInterface','CBaseRenderer','UAEJABU_GUID@@PAPAX@Z'),
 10:('SourceThreadCanWait','CBaseRenderer','UAEJH@Z'),11:('WaitForRenderTime','CBaseRenderer','UAEJXZ'),12:('CompleteStateChange','CBaseRenderer','UAEJW4_FilterState@@@Z'),
 14:('OnRenderStart','CBaseRenderer','UAEX'+IMS+'@Z'),15:('OnRenderEnd','CBaseRenderer','UAEX'+IMS+'@Z'),16:('OnStartStreaming','CBaseRenderer','UAEJXZ'),
 17:('OnStopStreaming','CBaseRenderer','UAEJXZ'),18:('OnWaitStart','CBaseRenderer','UAEXXZ'),19:('OnWaitEnd','CBaseRenderer','UAEXXZ'),
 21:('ScheduleSample','CBaseRenderer','UAEH'+IMS+'@Z'),22:('GetSampleTimes','CBaseRenderer','UAEJ'+IMS+'PA_J1@Z'),23:('ShouldDrawSampleNow','CBaseRenderer','UAEJ'+IMS+'PA_J1@Z'),
 24:('SendEndOfStream','CBaseRenderer','UAEJXZ'),25:('ResetEndOfStream','CBaseRenderer','UAEJXZ'),26:('EndOfStream','CBaseRenderer','UAEJXZ'),
 27:('CancelNotification','CBaseRenderer','UAEJXZ'),28:('ClearPendingSample','CBaseRenderer','UAEJXZ'),29:('Active','CBaseRenderer','UAEJXZ'),
 30:('Inactive','CBaseRenderer','UAEJXZ'),31:('StartStreaming','CBaseRenderer','UAEJXZ'),32:('StopStreaming','CBaseRenderer','UAEJXZ'),
 33:('BeginFlush','CBaseRenderer','UAEJXZ'),34:('EndFlush','CBaseRenderer','UAEJXZ'),35:('BreakConnect','CBaseRenderer','UAEJXZ'),
 36:('SetMediaType','CBaseRenderer','UAEJPBVCMediaType@@@Z'),37:('CompleteConnect','CBaseRenderer','UAEJPAUIPin@@@Z'),38:('PrepareReceive','CBaseRenderer','UAEJ'+IMS+'@Z'),
 39:('Receive','CBaseRenderer','UAEJ'+IMS+'@Z'),40:('HaveCurrentSample','CBaseRenderer','UAEHXZ'),41:('GetCurrentSample','CBaseRenderer','UAEPAUIMediaSample@@XZ'),
 42:('Render','CBaseRenderer','UAEJ'+IMS+'@Z'),
 45:('RecordFrameLateness','CBaseVideoRenderer','UAEXHH@Z'),46:('OnDirectRender','CBaseVideoRenderer','UAEX'+IMS+'@Z'),47:('ResetStreamingTimes','CBaseVideoRenderer','UAEJXZ'),48:('SendQuality','CBaseVideoRenderer','UAEJ_J0@Z')}
VR,VV=0x012BF498,0x012BF5D8
out=[]
for k,(m,owner,tail) in sorted(R.items()):
    ar=p.u32(VR+4*k) if k<45 else None; av=p.u32(VV+4*k)
    if ar is not None:
        out.append(('%08x'%ar,'?%s@%s@@%s'%(m,owner,tail),'BaseClasses %s.h declaration order: %s::%s is primary-vtable slot %d; retail CBaseRenderer vtable 0x012BF498 slot %d (its ??_G 0x00CA7340) and CTextureRenderer vtable 0x0129D08C (RTTI) slot %d hold 0x%08X'%('amfilter' if owner=='CBaseFilter' else 'renbase',owner,m,k,k,k,ar)))
    if av!=ar:
        o2='CBaseVideoRenderer'
        out.append(('%08x'%av,'?%s@%s@@%s'%(m,o2,tail),'BaseClasses renbase.h: CBaseVideoRenderer overrides %s (primary slot %d); retail CBaseVideoRenderer vtable 0x012BF5D8 slot %d (its ??_G 0x00CA73D0) holds 0x%08X, CBaseRenderer vtable 0x012BF498 slot %d holds %s'%(m,k,k,av,k,'0x%08X'%ar if ar else '-')))
seen=set()
with open(W+'/proven.tsv','w') as o:
    o.write('address\tsymbol\tevidence\n')
    for a,s,e in out:
        if a in seen or scope(a)=='out': continue
        seen.add(a); o.write('%s\t%s\t%s\n'%(a,s,e))
print(len(seen))
