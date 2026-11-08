// NUISystem::CTable::ConstructSpritesToDraw() -- size the table's tiled border from its def, then add the four corner sprites as children and draw the four edges and the separation lines
// Leaf: ?ConstructSpritesToDraw@CTable@NUISystem@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00551EA0, 3,849 bytes (0x00551EA0..0x00552DA8 `ret`, int3 to 0x00552DAF, next function
//   0x00552DB0). CTable vtable 0x0124A224 slot 142 (rtti-vtables.tsv); Ego_r.pdb / FableWin.pdb ?ConstructSpritesToDraw@CTable@NUISystem@@MAEXXZ.
// What it does: the def's ExpansionType says whether the table stretches horizontally (TOP_MIDDLE sprite width) and/or vertically
//   (MIDDLE_LEFT sprite height); the table Size is cut into whole tiles and the separation positions are rescaled to tile units.
//   Each corner sprite is created through the UI manager from its def index, initialised, placed (SetPosition and state 0),
//   added to Children, and the running edge positions advance by its size. Then each edge line is built by the CTable virtuals
//   ConstructHorizontalLine (slot 143, 0x005518E0) / ConstructVerticalLine (slot 144, 0x00551BC0) with the matching separation
//   sprite (or a null pointer), and every separation gets a line from the far edge with the CROSS sprite at the crossings.
// Callees: LoadAndReturnDef (slot 108), CComponent::GetSize (slot 23, hidden C2DVector), GetDefIndex (slot 107), Initialise (slot 43),
//   SetPosition (slot 15), GetStates (slot 82), map<unsigned long,CUIState>::operator[] 0x0042D5B1, CManager::GetInstance 0x0041E5F2,
//   CManager::CreateComponent(long) 0x0041D21B, _Rb_tree<ETableSprites,...>::find<ETableSprites> 0x00553520, CCountedPointer<CComponent>
//   (CComponent*) 0x00429C15, vector<CCountedPointer<CComponent> >::_M_insert_overflow 0x005359D0, operator new 0x00BFEA1A,
//   operator delete 0x00BFE9BC, __ftol2 0x00BFEA70.
// Shape (each found with the lane probes; TRACE.md E1-E8):
//   - The line calls are if / else pairs (one call per arm, cross-jumped by VC7.1 where the arms' registers agree); a `?:` argument
//     adds a conditional-temporary flag and a destroy of the null temporary that retail does not have.
//   - VC7.1 inlines a call site by its static frequency (c2 0x1078D1AA: entry 1, an `if` halves it, a loop body x5; after a `for`
//     loop the frequency stays at the loop body's, after a `while (i < n) { ...; ++i; }` it returns to the loop entry's). Retail's
//     inline split -- tree find called in the two size blocks and for SEPARATION_TOP, the CCountedPointer(T*) constructor
//     expanded for the first corner only -- needs the corners at frequency 2, i.e. exactly one of the two size loops in the while
//     form (either gives the same bytes; the first is written so).
//   - Declarations: the four positions before the integers (their zero stores stay immediates), and nbHorizontal, nbVertical,
//     horizontalSize, verticalSize in that order (retail's zero-store order and `imul ebp,[nbVertical]`).
//   - The edge totals are written inline (`X += horizontalSize * nbHorizontal`): VC7.1 keeps the converted product on the x87 stack
//     (`fst` + `fadd [X]`) and reuses it for the later `-=`; a named float local is stored and reloaded first.
//   - FABLE_RETAIL_STLP_MAP: the visible map<unsigned long,CUIState>::operator[] body lets VC7.1 schedule the state-0 key store after
//     the argument push, as retail (the call stays out of line).
//   - The horizontal separation position is `position.X = bottomLeftPosition.X + horizontalSize` (retail loads the converted width
//     first; `position.X += horizontalSize` loads the copied X first).
// Header: engine/NUISystem_CTable.h opt-in FABLE_RETAIL_CTABLE_CONSTRUCT (the three CTable virtuals, slots 142-144) and
//   stlport/fable_map.h opt-in FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE (D1: the non-const tree find as a member template), hdr/README.md.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTABLE_CONSTRUCT
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE
#include "engine/NUISystem_CTable.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CUIState.h"
#include "engine/CDefPointer.h"
namespace NUISystem {
void CTable::ConstructSpritesToDraw()
{
    typedef std::map<ETableSprites, CCountedPointer<CComponent> >::iterator SpriteIterator;
    C2DVector topLeftPosition(0.0f, 0.0f);
    C2DVector topRightPosition(0.0f, 0.0f);
    C2DVector bottomLeftPosition(0.0f, 0.0f);
    C2DVector bottomRightPosition(0.0f, 0.0f);
    unsigned long nbHorizontal = 0;
    unsigned long nbVertical = 0;
    unsigned long horizontalSize = 0;
    unsigned long verticalSize = 0;
    CDefPointer<const CUIDef> def = LoadAndReturnDef();

    // Tile sizes: the table stretches by whole edge sprites; separations are given in pixels and become tile counts.
    if (def->ExpansionType & TABLE_EXPANSION_HORIZONTAL) {
        SpriteIterator it = Sprites.find(TABLE_SPRITES_TOP_MIDDLE);
        if (it != Sprites.end()) {
            horizontalSize = (unsigned long)(*it).second->GetSize().X;
            nbHorizontal = (unsigned long)(Size.X / horizontalSize);
            unsigned long i = 0;
            while (i < PVerticalSeparations->size()) {
                (*PVerticalSeparations)[i] /= horizontalSize;
                ++i;
            }
        }
    }
    if (def->ExpansionType & TABLE_EXPANSION_VERTICAL) {
        SpriteIterator it = Sprites.find(TABLE_SPRITES_MIDDLE_LEFT);
        if (it != Sprites.end()) {
            verticalSize = (unsigned long)(*it).second->GetSize().Y;
            nbVertical = (unsigned long)(Size.Y / verticalSize);
            for (unsigned long i = 0; i < PHorizontalSeparations->size(); ++i)
                (*PHorizontalSeparations)[i] /= verticalSize;
        }
    }
    topRightPosition.X += horizontalSize * nbHorizontal;
    bottomLeftPosition.Y += verticalSize * nbVertical;
    bottomRightPosition.X = topRightPosition.X;
    bottomRightPosition.Y = bottomLeftPosition.Y;

    // Corners: each one moves the start of the two edges that touch it.
    SpriteIterator it = Sprites.find(TABLE_SPRITES_TOP_LEFT);
    if (it != Sprites.end()) {
        CCountedPointer<CComponent> sprite(CManager::GetInstance()->CreateComponent((*it).second->GetDefIndex()));
        sprite->Initialise();
        sprite->SetPosition(topLeftPosition);
        sprite->GetStates()[0].Position = topLeftPosition;
        Children.push_back(sprite);
        topLeftPosition.X += (*it).second->GetSize().X;
        bottomLeftPosition.Y += (*it).second->GetSize().Y;
        topRightPosition.X += (*it).second->GetSize().X;
    }
    it = Sprites.find(TABLE_SPRITES_TOP_RIGHT);
    if (it != Sprites.end() && topRightPosition.IsNonZero()) {
        CCountedPointer<CComponent> sprite(CManager::GetInstance()->CreateComponent((*it).second->GetDefIndex()));
        sprite->Initialise();
        sprite->SetPosition(topRightPosition);
        sprite->GetStates()[0].Position = topRightPosition;
        Children.push_back(sprite);
        topRightPosition += (*it).second->GetSize();
        topRightPosition.X -= horizontalSize;
        bottomRightPosition.Y += (*it).second->GetSize().Y;
    }
    it = Sprites.find(TABLE_SPRITES_BOTTOM_LEFT);
    if (it != Sprites.end() && bottomLeftPosition.IsNonZero()) {
        CCountedPointer<CComponent> sprite(CManager::GetInstance()->CreateComponent((*it).second->GetDefIndex()));
        sprite->Initialise();
        sprite->SetPosition(bottomLeftPosition);
        sprite->GetStates()[0].Position = bottomLeftPosition;
        Children.push_back(sprite);
        bottomLeftPosition.Y -= verticalSize * nbVertical;
        bottomRightPosition.X += (*it).second->GetSize().X;
    }
    it = Sprites.find(TABLE_SPRITES_BOTTOM_RIGHT);
    if (it != Sprites.end() && bottomRightPosition.IsNonZero()) {
        CCountedPointer<CComponent> sprite(CManager::GetInstance()->CreateComponent((*it).second->GetDefIndex()));
        sprite->Initialise();
        sprite->SetPosition(bottomRightPosition);
        sprite->GetStates()[0].Position = bottomRightPosition;
        Children.push_back(sprite);
        bottomRightPosition.Y += (*it).second->GetSize().Y;
        bottomRightPosition.X -= horizontalSize * nbHorizontal;
        bottomRightPosition.Y -= verticalSize;
    }

    // Edges, and one line per separation starting after the first tile (the CROSS sprite at the crossings).
    it = Sprites.find(TABLE_SPRITES_TOP_MIDDLE);
    if (it != Sprites.end()) {
        SpriteIterator separation = Sprites.find(TABLE_SPRITES_SEPARATION_TOP);
        if (separation != Sprites.end())
            ConstructHorizontalLine((*it).second, (*separation).second, topLeftPosition, nbHorizontal, 0);
        else
            ConstructHorizontalLine((*it).second, CCountedPointer<CComponent>(), topLeftPosition, nbHorizontal, 0);
        for (unsigned long i = 0; i < PHorizontalSeparations->size(); ++i) {
            C2DVector position = bottomLeftPosition;
            position.Y += (*PHorizontalSeparations)[i] * verticalSize;
            position.X = bottomLeftPosition.X + horizontalSize;
            unsigned long count = (unsigned long)((topRightPosition.X - position.X) / horizontalSize);
            unsigned long offset = 0;
            SpriteIterator corner = Sprites.find(TABLE_SPRITES_TOP_LEFT);
            if (corner != Sprites.end())
                offset = (unsigned long)((*corner).second->GetSize().X / horizontalSize);
            SpriteIterator cross = Sprites.find(TABLE_SPRITES_CROSS);
            if (cross != Sprites.end())
                ConstructHorizontalLine((*it).second, (*cross).second, position, count, offset - 1);
            else
                ConstructHorizontalLine((*it).second, CCountedPointer<CComponent>(), position, count, offset - 1);
        }
    }
    it = Sprites.find(TABLE_SPRITES_BOTTOM_MIDDLE);
    if (it != Sprites.end()) {
        SpriteIterator separation = Sprites.find(TABLE_SPRITES_SEPARATION_BOTTOM);
        if (separation != Sprites.end())
            ConstructHorizontalLine((*it).second, (*separation).second, bottomRightPosition, nbHorizontal, 0);
        else
            ConstructHorizontalLine((*it).second, CCountedPointer<CComponent>(), bottomRightPosition, nbHorizontal, 0);
    }
    it = Sprites.find(TABLE_SPRITES_MIDDLE_LEFT);
    if (it != Sprites.end()) {
        SpriteIterator separation = Sprites.find(TABLE_SPRITES_SEPARATION_LEFT);
        if (separation != Sprites.end())
            ConstructVerticalLine((*it).second, (*separation).second, bottomLeftPosition, nbVertical, 0);
        else
            ConstructVerticalLine((*it).second, CCountedPointer<CComponent>(), bottomLeftPosition, nbVertical, 0);
        for (unsigned long i = 0; i < PVerticalSeparations->size(); ++i) {
            C2DVector position = topLeftPosition;
            position.X += (*PVerticalSeparations)[i] * horizontalSize;
            position.Y += verticalSize;
            unsigned long count = (unsigned long)((bottomRightPosition.Y - position.Y) / verticalSize);
            unsigned long offset = 0;
            SpriteIterator corner = Sprites.find(TABLE_SPRITES_TOP_LEFT);
            if (corner != Sprites.end())
                offset = (unsigned long)((*corner).second->GetSize().Y / verticalSize);
            SpriteIterator cross = Sprites.find(TABLE_SPRITES_CROSS);
            if (cross != Sprites.end())
                ConstructVerticalLine((*it).second, (*cross).second, position, count, offset - 1);
            else
                ConstructVerticalLine((*it).second, CCountedPointer<CComponent>(), position, count, offset - 1);
        }
    }
    it = Sprites.find(TABLE_SPRITES_MIDDLE_RIGHT);
    if (it != Sprites.end()) {
        SpriteIterator separation = Sprites.find(TABLE_SPRITES_SEPARATION_RIGHT);
        if (separation != Sprites.end())
            ConstructVerticalLine((*it).second, (*separation).second, topRightPosition, nbVertical, 0);
        else
            ConstructVerticalLine((*it).second, CCountedPointer<CComponent>(), topRightPosition, nbVertical, 0);
    }
}
}