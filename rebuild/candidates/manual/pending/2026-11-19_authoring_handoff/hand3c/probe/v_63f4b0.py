def a(s):
    s=s.replace("    CMapVector* maps = &TempMaps;\n    GetMapsInArea(area, maps);","    GetMapsInArea(area, &TempMaps);")
    s=s.replace("it = maps->_M_start;","it = TempMaps._M_start;").replace("    maps->clear();","    TempMaps.clear();")
    return s
def b(s):
    return s.replace("    maps->clear();","    TempMaps.clear();")
def c(s):
    return a(s).replace("const F& filter, G& func)","F filter, G& func)").replace("const CFilter_Bind2<CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox>& filter","CFilter_Bind2<CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox> filter")
def d(s):
    return a(s).replace("&& func(thing))","&& func((T*)thing))")
VARIANTS=[('landed_style',a),('clear_direct',b),('filter_byval',c),('func_cast',d)]
