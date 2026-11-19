def rep(s, a, b): assert a in s, a; return s.replace(a, b, 1)
A='    C2DVector thingPos;\n    GetRelativePosOnMap(position, thingPos);\n'
M='    C2DVector mapSize;\n    GetRelativeMapSize(mapSize);\n'
def mapsize_first(s):
    s=rep(s, M, '    GetRelativeMapSize(mapSize);\n')
    return rep(s, A, '    C2DVector mapSize;\n'+A)
def both_top(s):
    s=rep(s, M, '    GetRelativeMapSize(mapSize);\n')
    return rep(s, A, '    C2DVector thingPos;\n    C2DVector mapSize;\n    GetRelativePosOnMap(position, thingPos);\n')
def box_before_mapsize(s):
    B='    C2DBoxF local_render_box(renderBox);\n'
    s=rep(s, B, '')
    return rep(s, M, B+M)
def renderpos_named_assign(s):
    s=rep(s,'    C2DVector renderPos((float)','    C2DVector renderPos;\n    renderPos = C2DVector((float)')
    return s
VARIANTS = [("mapsize_first", mapsize_first), ("both_top", both_top), ("box_before_mapsize", box_before_mapsize)]
