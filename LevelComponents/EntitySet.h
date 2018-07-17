#ifndef ENTITYSET_H
#define ENTITYSET_H

#include "ROMUtils.h"
#include "Tile.h"

#include <QVector>
#include <QColor>

namespace LevelComponents
{
    class EntitySet
    {
    private:
        int EntitySetID;  //maximun 89 (from 0 to 89)
        QVector<QRgb> palettes[16];
        Tile8x8 *tile8x8data[0x400];
        void LoadSubPalettes(int startPaletteId, int paletteNum, int paletteSetPtr);
        void LoadSpritesTiles(int tileaddress, int datalength, int startrow);
    public:
        EntitySet(int _EntitySetID, int basicElementPalettePtr);
    };
}

#endif // ENTITYSET_H