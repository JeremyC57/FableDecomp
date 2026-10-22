#pragma optimize("s",on)

struct CMemoryArea
{
    int unused0;
    CMemoryArea* parent;
    CMemoryArea* right;
    CMemoryArea* left;
};

void RotateLeft(CMemoryArea* area, CMemoryArea** root)
{
    CMemoryArea* y = area->right;
    area->right = y->left;
    if (y->left)
    {
        y->left->parent = area;
    }
    y->parent = area->parent;
    if (area == *root)
    {
        *root = y;
    }
    else if (area == area->parent->left)
    {
        area->parent->left = y;
    }
    else
    {
        area->parent->right = y;
    }
    y->left = area;
    area->parent = y;
}