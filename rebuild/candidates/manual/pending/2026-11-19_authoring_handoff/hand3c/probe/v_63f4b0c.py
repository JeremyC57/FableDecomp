H='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand2c/hdr/engine/CThingSearchTools.h'
R=open('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand2c/tmp/CThingSearchTools.repo.h').read()
k=R.index('template<class T,class Filter,class Proc> bool ForEachInRoughArea(const C2DBoxI& area,const Filter& filter,const Proc& proc) const {')
OLDY='''                for(long y=cells.TLY;y<cells.BRY;++y) {
                    const NThingMap::CThingMapCell* cell=&map.PeekCell((unsigned char)layer,cells.TLX,y);
                    for(long x=cells.TLX;x<cells.BRX;++x,++cell) {'''
j=R.index(OLDY,k)
def hv(newy, close_extra=True, empty=None):
    def f(s):
        body=R[:j]+newy+R[j+len(OLDY):]
        if close_extra:
            # add one more closing brace after the y loop: find the y loop's end ("                }\n            }\n        }\n        maps.clear();\n        return result;") after k
            old='''                }
            }
        }
        maps.clear();
        return result;'''
            m=body.index(old,k)
            body=body[:m]+'''                }
                }
            }
        }
        maps.clear();
        return result;'''+body[m+len(old):]
        return (s,{H:body})
    return f
G1='''                long y=cells.TLY;
                if(y<cells.BRY) {
                for(;y<cells.BRY;++y) {
                    const NThingMap::CThingMapCell* cell=&map.PeekCell((unsigned char)layer,cells.TLX,y);
                    for(long x=cells.TLX;x<cells.BRX;++x,++cell) {'''
G2='''                long y=cells.TLY;
                if(y<cells.BRY) {
                long offset=(unsigned char)layer*(long)sizeof(NThingMap::CMapwhoLevelInfo);
                long cells_offset=(unsigned char)layer*(long)sizeof(std::vector<NThingMap::CThingMapCell>);
                for(;y<cells.BRY;++y) {
                    const NThingMap::CThingMapCell* cell=&(*(const std::vector<NThingMap::CThingMapCell>*)((size_t)map.Cells.begin()+cells_offset)).begin()[y*(*(const long*)((size_t)offset+(size_t)map.MapwhoLevelInfo.begin()+offsetof(NThingMap::CMapwhoLevelInfo,Extents)))+cells.TLX];
                    for(long x=cells.TLX;x<cells.BRX;++x,++cell) {'''
G3='''                long y=cells.TLY;
                if(y<cells.BRY) {
                long offset=(unsigned char)layer*(long)sizeof(NThingMap::CMapwhoLevelInfo);
                long cells_offset=(unsigned char)layer*(long)sizeof(std::vector<NThingMap::CThingMapCell>);
                for(;y<cells.BRY;++y) {
                    const NThingMap::CThingMapCell* cell=&(*(const std::vector<NThingMap::CThingMapCell>*)((size_t)map.Cells.begin()+cells_offset)).begin()[y*(*(const long*)((size_t)offset+(size_t)map.MapwhoLevelInfo.begin()+offsetof(NThingMap::CMapwhoLevelInfo,Extents)))+cells.TLX];
                    for(long x=cells.TLX;x<cells.BRX;++x,++cell) {'''
VARIANTS=[('guard_peek',hv(G1)),('guard_byteoff',hv(G2))]
