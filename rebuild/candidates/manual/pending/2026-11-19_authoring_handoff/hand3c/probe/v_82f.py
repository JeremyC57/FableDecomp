FX='(float)GFFloatToLongFloor(mapSize.X * thingPos.X * local_render_box.GetExtents().X + local_render_box.TLX)'
FY='(float)GFFloatToLongFloor(local_render_box.TLY + local_render_box.GetExtents().Y - mapSize.Y * thingPos.Y * local_render_box.GetExtents().Y)'
BX='(local_render_box.BRX - local_render_box.TLX) * region.GetMiniMapScale() + local_render_box.TLX'
BY='(local_render_box.BRY - local_render_box.TLY) * region.GetMiniMapScale() + local_render_box.TLY'
def rep(s, a, b): assert a in s, a; return s.replace(a, b, 1)
VARIANTS=[
 ("ctor_args_both", lambda s: rep(rep(s,FX,'('+FX+')'),FY,'('+FY+')')),
 ("box_products_both", lambda s: rep(rep(s,BX,'('+BX.replace(' + local_render_box.TLX',')+ local_render_box.TLX')[0:0]+'((local_render_box.BRX - local_render_box.TLX) * region.GetMiniMapScale()) + local_render_box.TLX'),BY,'((local_render_box.BRY - local_render_box.TLY) * region.GetMiniMapScale()) + local_render_box.TLY')),
 ("y_head_x_tail", lambda s: rep(rep(s,'local_render_box.TLY + local_render_box.GetExtents().Y - mapSize.Y','(local_render_box.TLY + local_render_box.GetExtents().Y) - mapSize.Y'),'mapSize.X * thingPos.X * local_render_box.GetExtents().X + local_render_box.TLX','(mapSize.X * thingPos.X * local_render_box.GetExtents().X) + local_render_box.TLX')),
 ("floor_whole_both", lambda s: rep(rep(s,'GFFloatToLongFloor(mapSize.X * thingPos.X * local_render_box.GetExtents().X + local_render_box.TLX)','GFFloatToLongFloor((mapSize.X * thingPos.X * local_render_box.GetExtents().X + local_render_box.TLX))'),'GFFloatToLongFloor(local_render_box.TLY + local_render_box.GetExtents().Y - mapSize.Y * thingPos.Y * local_render_box.GetExtents().Y)','GFFloatToLongFloor((local_render_box.TLY + local_render_box.GetExtents().Y - mapSize.Y * thingPos.Y * local_render_box.GetExtents().Y))')),
]
