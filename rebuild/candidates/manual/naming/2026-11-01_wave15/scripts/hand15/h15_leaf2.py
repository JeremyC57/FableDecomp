# own-row respells fam_leaf skipped (parameter pointer -> reference, const return, free stand-in -> member):
# 0x0049EAF0 / 0x009FC020 / 0x006BBF40 by edit; 0x00A03B40 / 0x00A03B50 / 0x00A03B70 re-modelled (text/<addr>.cpp).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all, path
for a in ('00a03b40', '00a03b50', '00a03b70'):
    p, rel = path(a, 'src'); new = open(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'text', a + '.cpp')).read()
    if open(p).read() == new: print('already', a, 'src')
    else: open(p, 'w').write(new); print('ok', a, 'src')
# Ego_r ?HandleMoveHeroToRegionGameEvent@CWorld@@QAEXABVCGameEvent@@@Z (const reference)
a = '0049eaf0'
edit(a, 'src', '    void HandleMoveHeroToRegionGameEvent(const CGameEvent* event);', '    void HandleMoveHeroToRegionGameEvent(const CGameEvent& event);')
edit(a, 'src', 'void CWorld::HandleMoveHeroToRegionGameEvent(const CGameEvent* event)\n{', 'void CWorld::HandleMoveHeroToRegionGameEvent(const CGameEvent& event)\n{')
edit(a, 'src', '    event->GetData(buffer);', '    event.GetData(buffer);')
# Ego_r ?RemoveResource@CResourceList@@QAEXAAVCResource@@@Z (the real class, reference parameter)
a = '009fc020'
edit(a, 'src', '#include "engine/CResourceList.h"\n', '#define CResourceList CResourceListFields\n#include "engine/CResourceList.h"\n#undef CResourceList\n')
edit(a, 'src', 'struct CResourceList_Methods : CResourceList {\n    void RemoveResource(CResource* resource);\n};\n\nvoid CResourceList_Methods::RemoveResource(CResource* resource)\n{\n    resource->ResourceList = 0;\n    static_cast<CResource_Methods*>(resource)->ListRemove();\n    this->ResourceCount--;\n    this->AllocatedMemory -= resource->ResourceSize;',
     'class CResourceList : public CResourceListFields { public:\n    void RemoveResource(CResource& resource);\n};\n\nvoid CResourceList::RemoveResource(CResource& resource)\n{\n    resource.ResourceList = 0;\n    static_cast<CResource_Methods*>(&resource)->ListRemove();\n    this->ResourceCount--;\n    this->AllocatedMemory -= resource.ResourceSize;')
# Ego_r ?GetWorldMapOffset@CRegion@@QBE?BVC2DVector@@XZ (const return value)
a = '006bbf40'
edit(a, 'src', '    C2DVector GetWorldMapOffset() const;', '    const C2DVector GetWorldMapOffset() const;')
edit(a, 'src', 'C2DVector CRegion::GetWorldMapOffset() const\n{', 'const C2DVector CRegion::GetWorldMapOffset() const\n{')
edit('009fc020', 'src', '// CResourceList::RemoveResource(CResource* resource) --', '// CResourceList::RemoveResource(CResource& resource) --')
edit('0049eaf0', 'src', '// CWorld::HandleMoveHeroToRegionGameEvent(const CGameEvent*) --', '// CWorld::HandleMoveHeroToRegionGameEvent(const CGameEvent&) --')
edit('0049eaf0', 'src', '// Leaf: HandleMoveHeroToRegionGameEvent\n', '// Leaf: ?HandleMoveHeroToRegionGameEvent@CWorld@@QAEXABVCGameEvent@@@Z\n')
